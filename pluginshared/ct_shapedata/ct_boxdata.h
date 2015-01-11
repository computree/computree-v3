/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef CT_BOXDATA_H
#define CT_BOXDATA_H

#include "ct_shapedata.h"

class PLUGINSHAREDSHARED_EXPORT CT_BoxData : public CT_ShapeData
{
public:
    CT_BoxData();
    CT_BoxData(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const Eigen::Vector3d &widthDirection, double height, double width, double depth);

    const Eigen::Vector3d& getWidthDirection() const;
    float getHeight() const;
    float getWidth() const;
    float getDepth() const;

    CT_BoxData* clone() const;

private:

    Eigen::Vector3d   _widthDirection;
    double            _width;  // selon l'axe de _widthDirection
    double            _height; // selon l'axe de _direction
    double            _depth;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        qreal x = _widthDirection.x();
        qreal y = _widthDirection.y();
        qreal z = _widthDirection.z();

        ar & x & y & z & _height & _width & _depth;

    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_ShapeData>(*this);

        qreal x, y, z;

        ar & x & y & z & _height & _width & _depth;

        _widthDirection.setX(x);
        _widthDirection.setY(y);
        _widthDirection.setZ(z);

    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT_KEY(CT_BoxData)
#else
};
#endif

#endif // CT_BOXDATA_H
