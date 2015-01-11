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

#include "ct_polyline2ddata.h"

#include "ct_triangulation/ct_nodet.h"

#include <math.h>
#include <limits>
#include <algorithm>

CT_Polyline2DData::CT_Polyline2DData() : CT_Shape2DData()
{
}

CT_Polyline2DData::CT_Polyline2DData(const QVector<Eigen::Vector2d*> &vertices, bool copy) : CT_Shape2DData()
{

    int size = vertices.size();
    _vertices.resize(size);

    _minX = std::numeric_limits<double>::max();
    _minY = std::numeric_limits<double>::max();
    _maxX = -std::numeric_limits<double>::max();
    _maxY = -std::numeric_limits<double>::max();

    for (int i = 0 ; i < size ; i++)
    {
        Eigen::Vector2d* source = vertices.at(i);
        if (copy)
        {
            _vertices[i] = new Eigen::Vector2d(*source);
        } else {
            _vertices[i] = vertices.at(i);
        }

        if ((*_vertices[i])(0) < _minX) {_minX = (*_vertices[i])(0);}
        if ((*_vertices[i])(0) > _maxX) {_maxX = (*_vertices[i])(0);}
        if ((*_vertices[i])(1) < _minY) {_minY = (*_vertices[i])(1);}
        if ((*_vertices[i])(1) > _maxY) {_maxY = (*_vertices[i])(1);}

    }

    _center(0) = (_maxX + _minX) / 2.0;
    _center(1) = (_maxY + _minY) / 2.0;
}

CT_Polyline2DData::~CT_Polyline2DData()
{
    qDeleteAll(_vertices);
}


CT_Polyline2DData* CT_Polyline2DData::clone() const
{
    return new CT_Polyline2DData(_vertices);
}

void CT_Polyline2DData::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min(0) = _minX;
    min(1) = _minY;
    min(2) = 0;
    max(0) = _maxX;
    max(1) = _maxY;
    max(2) = 0;
}
