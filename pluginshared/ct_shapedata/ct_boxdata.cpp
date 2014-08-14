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

#include "ct_boxdata.h"

CT_BoxData::CT_BoxData() : CT_ShapeData()
{
}

CT_BoxData::CT_BoxData(const QVector3D &center, const QVector3D &direction, const QVector3D &widthDirection, float height, float width, float depth) : CT_ShapeData(center, direction)
{
    _widthDirection = widthDirection;
    _height = height;
    _width = width;
    _depth = depth;
}

const QVector3D& CT_BoxData::getWidthDirection() const
{
    return _widthDirection;
}

float CT_BoxData::getHeight() const
{
    return _height;
}

float CT_BoxData::getWidth() const
{
    return _width;
}

float CT_BoxData::getDepth() const
{
    return _depth;
}



CT_BoxData* CT_BoxData::clone() const
{
    return new CT_BoxData(getCenter(), getDirection(), getWidthDirection(), _height, _width, _depth);
}
