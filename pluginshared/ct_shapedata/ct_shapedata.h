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

#ifndef CT_SHAPEDATA_H
#define CT_SHAPEDATA_H

#include "pluginShared_global.h"
#include "serialization.h"
#include "qvector3d.h"

/**
  * Reprsente les donnes d'une forme gomtrique
  */
class PLUGINSHAREDSHARED_EXPORT CT_ShapeData
{
public:

    CT_ShapeData();
    CT_ShapeData(const QVector3D &center, const QVector3D &direction);
    virtual ~CT_ShapeData();

    const QVector3D& getCenter() const;
    const QVector3D& getDirection() const;

    void setCenter(const QVector3D &center);
    void setDirection(const QVector3D &direction);

private:

    QVector3D   _center;
    QVector3D   _direction;

    friend class CT_AbstractShape;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        qreal x = _center.x();
        qreal y = _center.y();
        qreal z = _center.z();

        ar & x & y & z;

        x = _direction.x();
        y = _direction.y();
        z = _direction.z();

        ar & x & y & z;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        qreal x, y, z;

        ar & x & y & z;

        _center.setX(x);
        _center.setY(y);
        _center.setZ(z);

        ar & x & y & z;

        _direction.setX(x);
        _direction.setY(y);
        _direction.setZ(z);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT_KEY(CT_ShapeData)
#else
};
#endif

#endif // CT_SHAPEDATA_H
