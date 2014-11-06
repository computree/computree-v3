/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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

CT_Polyline2DData::CT_Polyline2DData(const QVector<QVector2D*> &vertices, bool copy) : CT_Shape2DData()
{

    int size = vertices.size();
    _vertices.resize(size);

    _minX = std::numeric_limits<float>::max();
    _minY = std::numeric_limits<float>::max();
    _maxX = -std::numeric_limits<float>::max();
    _maxY = -std::numeric_limits<float>::max();

    for (int i = 0 ; i < size ; i++)
    {
        QVector2D* source = vertices.at(i);
        if (copy)
        {
            _vertices[i] = new QVector2D(source->x(), source->y());
        } else {
            _vertices[i] = vertices.at(i);
        }

        if (_vertices[i]->x() < _minX) {_minX = _vertices[i]->x();}
        if (_vertices[i]->x() > _maxX) {_maxX = _vertices[i]->x();}
        if (_vertices[i]->y() < _minY) {_minY = _vertices[i]->y();}
        if (_vertices[i]->y() > _maxY) {_maxY = _vertices[i]->y();}

    }

    _center.setX((_maxX + _minX) / 2.0);
    _center.setY((_maxY + _minY) / 2.0);
}

CT_Polyline2DData::~CT_Polyline2DData()
{
    qDeleteAll(_vertices);
}


CT_Polyline2DData* CT_Polyline2DData::clone() const
{
    return new CT_Polyline2DData(_vertices);
}

void CT_Polyline2DData::getBoundingBox(QVector2D &min, QVector2D &max) const
{
    min.setX(_minX);
    min.setY(_minY);
    max.setX(_maxX);
    max.setY(_maxY);
}
