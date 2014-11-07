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

#include "ct_line2ddata.h"

#include "ct_mathpoint.h"
#include <math.h>


CT_Line2DData::CT_Line2DData() : CT_Shape2DData()
{
}

CT_Line2DData::CT_Line2DData(const QVector2D &p1, const QVector2D &p2) : CT_Shape2DData(p1)
{
    _p1 = p1;
    _p2 = p2;
}

const QVector2D& CT_Line2DData::getP1() const
{
    return _p1;
}

const QVector2D& CT_Line2DData::getP2() const
{
    return _p2;
}


double CT_Line2DData::x1() const
{
    return _p1.x();
}

double CT_Line2DData::y1() const
{
    return _p1.y();
}

double CT_Line2DData::x2() const
{
    return _p2.x();
}

double CT_Line2DData::y2() const
{
    return _p2.y();
}

double CT_Line2DData::length() const
{
    return CT_MathPoint::distance2D(_p1, _p2);
}

void CT_Line2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(std::min(_p1.x(), _p2.x()));
    min.setY(std::min(_p1.y(), _p2.y()));
    max.setX(std::max(_p1.x(), _p2.x()));
    max.setY(std::max(_p1.y(), _p2.y()));
}

CT_Line2DData* CT_Line2DData::clone() const
{
    return new CT_Line2DData(getP1(), getP2());
}
