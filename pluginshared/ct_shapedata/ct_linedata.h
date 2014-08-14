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

#ifndef CT_LINEDATA_H
#define CT_LINEDATA_H

#include "ct_shapedata.h"

#include "ct_pointcloud/abstract/ct_abstractpointcloud.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

class PLUGINSHAREDSHARED_EXPORT CT_LineData : public CT_ShapeData
{
public:
    CT_LineData();
    CT_LineData(const QVector3D &p1, const QVector3D &p2);
    CT_LineData(const QVector3D &p1, const QVector3D &p2, float error, int n);

    const QVector3D& getP1() const;
    const QVector3D& getP2() const;
    float getError() const;

    double x1() const;
    double y1() const;
    double z1() const;

    double x2() const;
    double y2() const;
    double z2() const;

    double length() const;

    inline int n() const {return _n_points;}
    inline double getMSE() {return _error;}
    inline double getRMSE()
    {
        if (n() < 2) {return 0;}
        return sqrt(getMSE());
    }

    bool intersectionWithRect3D  (float plan_x, float plan_y, float plan_z, QVector3D &vect_plan, float *xi, float *yi, float *zi);

    CT_LineData* clone() const;

    /**
      * \brief Retourne les donnes d'une ligne 3D (rgression linaire)  partir du nuage de points passé en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  2.
      */
    static CT_LineData* staticCreateLineDataFromPointCloud(const CT_AbstractPointCloud &pointCloud,
                                                           const CT_AbstractPointCloudIndex &pointCloudIndex);

    template<typename XYZpoint>
    static CT_LineData* staticCreateLineDataFromPointCloud(QList<XYZpoint*> &l_gp);

private:

    QVector3D   _p1;
    QVector3D   _p2;
    int         _n_points;
    float       _error;


#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        qreal x = _p1.x();
        qreal y = _p1.y();
        qreal z = _p1.z();

        ar & x & y & z;

        x = _p2.x();
        y = _p2.y();
        z = _p2.z();

        ar & x & y & z & _error;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        qreal x, y, z;

        ar & x & y & z;

        _p1.setX(x);
        _p1.setY(y);
        _p1.setZ(z);

        ar & x & y & z & _error;

        _p2.setX(x);
        _p2.setY(y);
        _p2.setZ(z);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT_KEY(CT_LineData)
#else
};
#endif

// Includes the template implementations
#include "ct_shapedata/ct_linedata.hpp"

#endif // CT_LINEDATA_H
