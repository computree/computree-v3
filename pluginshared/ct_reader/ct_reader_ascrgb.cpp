#include "ct_reader_ascrgb.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QDebug>

#include "ct_pointcloud/ct_pointcloudstdvector.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_global/ct_context.h"

#include <limits>

CT_Reader_ASCRGB::CT_Reader_ASCRGB() : CT_AbstractReader()
{
    m_filterRadius = 0;
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
                if (values.size() >= 6)
                {
                    f.close();
                    return CT_AbstractReader::setFilePath(filepath);
                }
            }

            f.close();
        }
    }

    return false;
}

void CT_Reader_ASCRGB::setRadiusFilter(const float &radius)
{
    m_filterRadius = radius;
}

CT_AbstractReader* CT_Reader_ASCRGB::copy() const
{
    return new CT_Reader_ASCRGB();
}

void CT_Reader_ASCRGB::protectedInit()
{
    addNewReadableFormat(FileFormat("asc", tr("Fichiers de points ASCII")));
}

void CT_Reader_ASCRGB::protectedCreateOutItemDrawableModelList()
{
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

            float xmin = std::numeric_limits<float>::max();
            float ymin = std::numeric_limits<float>::max();
            float zmin = std::numeric_limits<float>::max();

            float xmax = -std::numeric_limits<float>::max();
            float ymax = -std::numeric_limits<float>::max();
            float zmax = -std::numeric_limits<float>::max();

            QString line;

            while(!stream.atEnd()
                  && !isStopped())
            {
                line = stream.readLine();
                QStringList values = line.split(" ", QString::SkipEmptyParts);

                if (values.size() > 5)
                {
                    bool okX = false;
                    bool okY = false;
                    bool okZ = false;
                    bool okR = false;
                    bool okG = false;
                    bool okB = false;

                    float x = values.at(0).toFloat(&okX);
                    float y = values.at(1).toFloat(&okY);
                    float z = values.at(2).toFloat(&okZ);

                    float valueR = values.at(3).toFloat(&okR);
                    float valueG = values.at(4).toFloat(&okG);
                    float valueB = values.at(5).toFloat(&okB);

                    if (valueR < 0) {valueR = 0;}
                    if (valueG < 0) {valueG = 0;}
                    if (valueB < 0) {valueB = 0;}

                    if (valueR > 1) {valueR = 1;}
                    if (valueG > 1) {valueG = 1;}
                    if (valueB > 1) {valueB = 1;}

                    short r = 255*valueR;
                    short g = 255*valueG;
                    short b = 255*valueB;

                    if (okX && okY && okZ && okR && okG && okB)
                    {
                        float distance2D = sqrt(x*x + y*y);

                        if (!filter || (distance2D <= m_filterRadius))
                        {
                            CT_Point &p = pointCloud->addPoint();

                            p.setX((float)x);
                            p.setY((float)y);
                            p.setZ((float)z);

                            if (x<xmin) {xmin = (float)x;}
                            if (x>xmax) {xmax = (float)x;}
                            if (y<ymin) {ymin = (float)y;}
                            if (y>ymax) {ymax = (float)y;}
                            if (z<zmin) {zmin = (float)z;}
                            if (z>zmax) {zmax = (float)z;}

                            CT_Color &color = colorCloud->addColor();

                            color.r = r;
                            color.g = g;
                            color.b = b;
                        }
                    }
                }


                // Ralentit trop la lecture
                //setProgress(stream.pos()*100/fileSize);
            }

            f.close();

            if (colorCloud->size() > 0)
            {
                CT_ABSTRACT_NMPCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(pointCloud);

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
        }
    }

    return false;
}
