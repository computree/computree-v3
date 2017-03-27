#include "ct_reader_ascrgb.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QDebug>

#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_global/ct_context.h"
#include "ct_point.h"

#include <limits>

CT_Reader_ASCRGB::CT_Reader_ASCRGB() : CT_AbstractReader()
{
    m_filterRadius = 0;
    _zminFilter = -std::numeric_limits<double>::max();
    _zmaxFilter = std::numeric_limits<double>::max();
}

QString CT_Reader_ASCRGB::GetReaderName() const
{
    return tr("Points, fichier ASCII (XYZRGB, sans entête, RGB [0;1])");
}

CT_StepsMenu::LevelPredefined CT_Reader_ASCRGB::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_ASCRGB::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            stream.readLine();
            QString line = stream.readLine();
            if (!line.isNull())
            {
                QStringList values = line.split(" ");
                if (values.size() >= 3) {
                    f.close();
                    return CT_AbstractReader::setFilePath(filepath);
                } else {
                    PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'a pas le bon format (colonnes manquantes).")).arg(filepath));
                }
            } else {
                PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 est vide.")).arg(filepath));
            }

            f.close();
        } else {
            PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'est pas accessible.")).arg(filepath));
        }
    } else {
        PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'existe pas.")).arg(filepath));
    }

    return false;
}

void CT_Reader_ASCRGB::setRadiusFilter(const double &radius)
{
    m_filterRadius = radius;
}

void CT_Reader_ASCRGB::setRadiusFilter(const double &radius, const double &zmin, const double &zmax)
{
    m_filterRadius = radius;
    _zminFilter = zmin;
    _zmaxFilter = zmax;
}

CT_AbstractReader* CT_Reader_ASCRGB::copy() const
{
    return new CT_Reader_ASCRGB();
}

void CT_Reader_ASCRGB::protectedInit()
{
    addNewReadableFormat(FileFormat("asc", tr("Fichiers de points ASCII (XYZRGB, sans entête, RGB [0;1])")));

    setToolTip(tr("Charge un fichier de points au format ASCII, sans entête, ordonné, avec les champs suivants :<br>"
                  "- X  : Coordonnée X<br>"
                  "- Y  : Coordonnée Y<br>"
                  "- Z  : Coordonnée Z<br>"
                  "- R  : Composante rouge, valeur entre 0 et 1 (optionnel)<br>"
                  "- V  : Composante verte, valeur entre 0 et 1 (optionnel)<br>"
                  "- B  : Composante Bleue, valeur entre 0 et 1 (optionnel)<br>"));
}

void CT_Reader_ASCRGB::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_ASCRGB_sceneOut, new CT_Scene(), tr("Scène"));
    addOutItemDrawableModel(DEF_CT_Reader_ASCRGB_colorOut, new CT_PointsAttributesColor(), tr("Attribut de points (couleurs)"));
}

bool readPoint(QStringList &values, CT_Point &point)
{
    if (values.size() <= 2)
        return false;

    bool okX = false;
    bool okY = false;
    bool okZ = false;

    double x = values.at(0).toDouble(&okX);
    double y = values.at(1).toDouble(&okY);
    double z = values.at(2).toDouble(&okZ);

    if (!okX || !okY || !okZ)
        return false;

    point(0) = x;
    point(1) = y;
    point(2) = z;
    return true;
}

void readColor(QStringList &values, CT_Color &color)
{
    bool okR = false;
    bool okG = false;
    bool okB = false;

    double valueR = 1;
    double valueG = 1;
    double valueB = 1;

    if (values.size() > 3) {valueR = values.at(3).toDouble(&okR);}
    if (values.size() > 4) {valueG = values.at(4).toDouble(&okG);}
    if (values.size() > 5) {valueB = values.at(5).toDouble(&okB);}

    if (!okG) {valueG = valueR;}
    if (!okB) {valueB = valueR;}

    valueR = qBound(0., valueR, 1.);
    valueG = qBound(0., valueG, 1.);
    valueB = qBound(0., valueB, 1.);

    color(0) = 255*valueR;
    color(1) = 255*valueG;
    color(2) = 255*valueB;
    color(3) = 0;
}

/*
 * Check if the point is inside the bounds of a cylinder
 */
bool CT_Reader_ASCRGB::isInsideRadius(const CT_Point &point)
{
    if (m_filterRadius > 0) {
        double distance2D = sqrt(point.x() * point.x() + point.y() * point.y());
        return (distance2D <= m_filterRadius &&
                point.z() >= _zminFilter &&
                point.z() <= _zmaxFilter);
    }
    return true;
}

bool CT_Reader_ASCRGB::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            CT_AbstractUndefinedSizePointCloud* pointCloud = PS_REPOSITORY->createNewUndefinedSizePointCloud();
            CT_ColorCloudStdVector *colorCloud = new CT_ColorCloudStdVector(false);

            double xmin = std::numeric_limits<double>::max();
            double ymin = std::numeric_limits<double>::max();
            double zmin = std::numeric_limits<double>::max();

            double xmax = -std::numeric_limits<double>::max();
            double ymax = -std::numeric_limits<double>::max();
            double zmax = -std::numeric_limits<double>::max();

            QString line;
            CT_Point pReaded;

            // Getting the file size to set progress
            qint64 fileSize = f.size();
            qint64 currentSizeRead = 0;

            while(!stream.atEnd()
                  && !isStopped())
            {
                line = stream.readLine();
                currentSizeRead += line.size();
                setProgress((currentSizeRead*100)/fileSize);
                QStringList values = line.split(" ", QString::SkipEmptyParts);
                CT_Point point;
                CT_Color color;
                bool ok = readPoint(values, point);

                // FIXME: converted radius filter to lambda
                if (!ok || isFiltered(point) || !isInsideRadius(point))
                    continue;

                readColor(values, color);
                pointCloud->addPoint(pReaded);
                CT_Color &c = colorCloud->addColor();
                c.setColor(color);

                /* update bounding box */
                if (point.x() < xmin) {xmin = point.x();}
                if (point.x() > xmax) {xmax = point.x();}
                if (point.y() < ymin) {ymin = point.y();}
                if (point.y() > ymax) {ymax = point.y();}
                if (point.z() < zmin) {zmin = point.z();}
                if (point.z() > zmax) {zmax = point.z();}
            }

            f.close();

            if (colorCloud->size() > 0)
            {
                CT_NMPCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(pointCloud);

                CT_Scene *scene = new CT_Scene(NULL, NULL, pcir);
                scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

                CT_PointsAttributesColor *colors = new CT_PointsAttributesColor(NULL, NULL, pcir, colorCloud);

                addOutItemDrawable(DEF_CT_Reader_ASCRGB_sceneOut, scene);
                addOutItemDrawable(DEF_CT_Reader_ASCRGB_colorOut, colors);

                return true;

            }
            else
            {
                delete pointCloud;
                delete colorCloud;
            }
        } else {
            PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'est pas accessible.")).arg(filepath()));
        }
    } else {
        PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'existe pas.")).arg(filepath()));
    }

    return false;
}
