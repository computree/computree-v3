/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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

#include "ct_mathboundingshape.h"
#include "ct_math/ct_mathpoint.h"

bool CT_MathBoundingShape::aabbIntersects(const QVector3D &min1, const QVector3D &max1, const QVector3D &min2, const QVector3D &max2)
{
    if (max1.x() < min2.x()) {return false;}
    if (max2.x() < min1.x()) {return false;}
    if (max1.y() < min2.y()) {return false;}
    if (max2.y() < min1.y()) {return false;}
    if (max1.z() < min2.z()) {return false;}
    if (max2.z() < min1.z()) {return false;}
    return true;
}

bool CT_MathBoundingShape::aabbIntersects2D(const QVector3D &min1, const QVector3D &max1, const QVector3D &min2, const QVector3D &max2)
{     
    if (max1.x() < min2.x()) {return false;}
    if (max2.x() < min1.x()) {return false;}
    if (max1.y() < min2.y()) {return false;}
    if (max2.y() < min1.y()) {return false;}
    return true;
}

bool CT_MathBoundingShape::containsPoint(const QVector3D &min, const QVector3D &max, const CT_Point& point)
{
    if (point.getX() < min.x()) {return false;}
    if (point.getX() > max.x()) {return false;}
    if (point.getY() < min.y()) {return false;}
    if (point.getY() > max.y()) {return false;}
    if (point.getZ() < min.z()) {return false;}
    if (point.getZ() > max.z()) {return false;}
    return true;
}

bool CT_MathBoundingShape::containsPointIn2D(const QVector3D &min, const QVector3D &max, const CT_Point& point)
{
    if (point.getX() < min.x()) {return false;}
    if (point.getX() > max.x()) {return false;}
    if (point.getY() < min.y()) {return false;}
    if (point.getY() > max.y()) {return false;}
    return true;
}

