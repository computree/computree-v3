/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ct_circle2ddata.h"
#include "ct_pointcloud/ct_pointcloudstdvector.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include "ct_math/ct_mathpoint.h"

CT_Circle2DData::CT_Circle2DData() : CT_AreaShape2DData()
{
    _radius = 0;
}

CT_Circle2DData::CT_Circle2DData(const QVector2D &center,float radius) : CT_AreaShape2DData(center)
{
    _radius = radius;
}

void CT_Circle2DData::setRadius(float radius)
{
    _radius = radius;
}

float CT_Circle2DData::getRadius() const
{
    return _radius;
}

void CT_Circle2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(_center.x() - _radius);
    min.setY(_center.y() - _radius);
    max.setX(_center.x() + _radius);
    max.setY(_center.y() + _radius);
}

bool CT_Circle2DData::contains(float x, float y) const
{
    float distance = sqrt(pow(_center.x() - x, 2) + pow(_center.y() - y, 2));
    return (distance <= _radius);
}

double CT_Circle2DData::getArea() const
{
    return M_PI*_radius*_radius;
}

CT_Circle2DData* CT_Circle2DData::clone() const
{
    return new CT_Circle2DData(getCenter(), getRadius());
}

CT_Circle2DData& CT_Circle2DData::operator= (const CT_Circle2DData& o)
{
    setRadius(o.getRadius());
    setCenter(o.getCenter());

    return *this;
}
