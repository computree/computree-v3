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
    m_filePath = filepath;

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
                if (values.size() >= 3)
                {                    
                    m_header = new CT_FileHeader(NULL, NULL);
                    m_header->setFile(m_filePath);

                    f.close();
                    return true;
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

    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_ASCRGB_sceneOut, new CT_Scene(), tr("Scène")));
    addOutItemDrawableModel(new CT_OutStdSingularItemModel(DEF_CT_Reader_ASCRGB_colorOut, new CT_PointsAttributesColor(), tr("Attribut de points (couleurs)")));
}

bool CT_Reader_ASCRGB::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            bool filter = m_filterRadius > 0;

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

            while(!stream.atEnd()
                  && !isStopped())
            {
                line = stream.readLine();
                QStringList values = line.split(" ", QString::SkipEmptyParts);

                if (values.size() > 2)
                {
                    bool okX = false;
                    bool okY = false;
                    bool okZ = false;
                    bool okR = false;
                    bool okG = false;
                    bool okB = false;

                    double x = values.at(0).toDouble(&okX);
                    double y = values.at(1).toDouble(&okY);
                    double z = values.at(2).toDouble(&okZ);

                    double valueR = 1;
                    double valueG = 1;
                    double valueB = 1;

                    if (values.size() > 3) {valueR = values.at(3).toDouble(&okR);}
                    if (values.size() > 4) {valueG = values.at(4).toDouble(&okG);}
                    if (values.size() > 5) {valueB = values.at(5).toDouble(&okB);}

                    if (!okG) {valueG = valueR;}
                    if (!okB) {valueB = valueR;}

                    if (valueR < 0) {valueR = 0;}
                    if (valueG < 0) {valueG = 0;}
                    if (valueB < 0) {valueB = 0;}

                    if (valueR > 1) {valueR = 1;}
                    if (valueG > 1) {valueG = 1;}
                    if (valueB > 1) {valueB = 1;}

                    short r = 255*valueR;
                    short g = 255*valueG;
                    short b = 255*valueB;

                    if (okX && okY && okZ)
                    {
                        double distance2D = sqrt(x*x + y*y);

                        if (!filter || (distance2D <= m_filterRadius))
                        {
                            if (x<xmin) {xmin = x;}
                            if (x>xmax) {xmax = x;}
                            if (y<ymin) {ymin = y;}
                            if (y>ymax) {ymax = y;}
                            if (z<zmin) {zmin = z;}
                            if (z>zmax) {zmax = z;}

                            pReaded(0) = x;
                            pReaded(1) = y;
                            pReaded(2) = z;

                            pointCloud->addPoint(pReaded);

                            CT_Color &color = colorCloud->addColor();

                            color.r() = r;
                            color.g() = g;
                            color.b() = b;
                        }
                    }
                }


                // Ralentit trop la lecture
                //setProgress(stream.pos()*100/fileSize);
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
