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

#include "ct_point2ddata.h"

CT_Point2DData::CT_Point2DData() : CT_Shape2DData()
{
}

CT_Point2DData::CT_Point2DData(const QVector2D &point) : CT_Shape2DData(point)
{
}

CT_Point2DData::CT_Point2DData(const float &x, const float &y) : CT_Shape2DData(x, y)
{
}

double CT_Point2DData::x() const
{
    return getCenter().x();
}

double CT_Point2DData::y() const
{
    return getCenter().y();
}

void CT_Point2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(x());
    min.setY(y());
    max.setX(x());
    max.setY(y());
}

CT_Point2DData* CT_Point2DData::clone() const
{
    return new CT_Point2DData(getCenter());
}
