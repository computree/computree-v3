/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef CT_CYLINDERDATA_H
#define CT_CYLINDERDATA_H

#include "ct_circledata.h"

#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_linedata.h"

class PLUGINSHAREDSHARED_EXPORT CT_CylinderData : public CT_ShapeData
{
public:
    CT_CylinderData();
    CT_CylinderData(const QVector3D &center, const QVector3D &direction, float radius, float h);
    CT_CylinderData(const QVector3D &center, const QVector3D &direction, float radius, float h, float lineError, float circleError);

    float getRadius() const;
    float getHeight() const;
    float getLineError() const;
    float getCircleError() const;

    CT_CylinderData* clone() const;

    /**
      * \brief Retourne les donnes d'un cylindre 3D  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  3.
      */
    static CT_CylinderData* staticCreate3DCylinderDataFromPointCloud(const CT_AbstractPointCloud &pointCloud,
                                                                     const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                     const QVector3D &pointCloudBarycenter);

    static CT_CylinderData* staticCreate3DCylinderDataFromPointCloudAndDirection(const CT_AbstractPointCloud &pointCloud,
                                                                                 const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                                 const QVector3D &pointCloudBarycenter,
                                                                                 const CT_LineData &direction,
                                                                                 CT_CircleData *outCircleData = NULL);

private:

    float   _radius;
    float   _h;
    float   _lineError;
    float   _circleError;

    static const double M_PI_X2;

    /**
      * Classe prive permettant de traiter les points avant de faire un fitting d'un cercle
      */
    class CircleDataPreProcessingAction : public CT_CircleDataPreProcessingAction
    {
    public:

        CircleDataPreProcessingAction(const QVector3D &translation,
                                      double cosRotationZ,
                                      double sinRotationZ,
                                      double cosRotationY,
                                      double sinRotationY)
        {
            _translation = translation;
            _cosRotationZ = cosRotationZ;
            _sinRotationZ = sinRotationZ;
            _cosRotationY = cosRotationY;
            _sinRotationY = sinRotationY;
            _zMin = 999999999;
            _zMax = -999999999;
        }

        void preProcessing(const CT_Point &point, CT_Point &newPoint)
        {
            double x = point(0) + _translation.x();
            double y = point(1) + _translation.y();
            double z = point(2) + _translation.z();

            // rotation autour de l'axe z
            double xp = x*_cosRotationZ - y*_sinRotationZ;
            double yp = x*_sinRotationZ + y*_cosRotationZ;

            // rotation autour de l'axe y
            double xs = z*_sinRotationY + xp*_cosRotationY;
            double zs = z*_cosRotationY - xp*_sinRotationY;

            newPoint(0) = xs;
            newPoint(1) = yp;
            newPoint(2) = zs;

            if(newPoint(2) < _zMin)
            {
                _zMin = newPoint(2);
            }

            if(newPoint(2) > _zMax)
            {
                _zMax = newPoint(2);
            }
        }

        double getZMin() const { return _zMin; }
        double getZMax() const { return _zMax; }

        double getCosRotationZ() const { return _cosRotationZ; }
        double getCosRotationY() const { return _cosRotationY; }
        double getSinRotationZ() const { return _sinRotationZ; }
        double getSinRotationY() const { return _sinRotationY; }
        double getTranslationX() const { return _translation.x(); }
        double getTranslationY() const { return _translation.y(); }
        double getTranslationZ() const { return _translation.z(); }

    private:

        QVector3D   _translation;
        double      _cosRotationZ;
        double      _sinRotationZ;
        double      _cosRotationY;
        double      _sinRotationY;
        double      _zMin;
        double      _zMax;
    };

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        ar & _radius & _h & _lineError & _circleError;
    }
};

BOOST_CLASS_EXPORT_KEY(CT_CylinderData)
#else
};
#endif

#endif // CT_CYLINDERDATA_H
