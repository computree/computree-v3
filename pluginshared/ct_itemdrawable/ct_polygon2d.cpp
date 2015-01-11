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

#include "ct_polygon2d.h"

#include <limits>

CT_DEFAULT_IA_INIT(CT_Polygon2D)

const CT_StandardPolygon2DDrawManager CT_Polygon2D::POLYGON2D_DRAW_MANAGER;

CT_Polygon2D::CT_Polygon2D() : CT_AbstractAreaShape2D()
{
    setBaseDrawManager(&POLYGON2D_DRAW_MANAGER);
}

CT_Polygon2D::CT_Polygon2D(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result,
                         CT_Polygon2DData *data) : CT_AbstractAreaShape2D(model, result, data)
{
    setBaseDrawManager(&POLYGON2D_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates(0) = data->_minX;
        _minCoordinates(1) = data->_minY;

        _maxCoordinates(0) = data->_maxX;
        _maxCoordinates(1) = data->_maxY;
    } else {
        _minCoordinates(0) = 0;
        _minCoordinates(1) = 0;

        _maxCoordinates(0) = 0;
        _maxCoordinates(1) = 0;
    }
    _minCoordinates(2) = 0;
    _maxCoordinates(2) = 0;
}

CT_Polygon2D::CT_Polygon2D(const QString &modelName,
                         const CT_AbstractResult *result,
                         CT_Polygon2DData *data) : CT_AbstractAreaShape2D(modelName, result, data)
{
    setBaseDrawManager(&POLYGON2D_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates(0) = data->_minX;
        _minCoordinates(1) = data->_minY;

        _maxCoordinates(0) = data->_maxX;
        _maxCoordinates(1) = data->_maxY;
    } else {
        _minCoordinates(0) = 0;
        _minCoordinates(1) = 0;

        _maxCoordinates(0) = 0;
        _maxCoordinates(1) = 0;
    }
    _minCoordinates(2) = 0;
    _maxCoordinates(2) = 0;
}

const QVector<Eigen::Vector2d*>& CT_Polygon2D::getVertices() const
{
    return ((const CT_Polygon2DData&)getData()).getVertices();
}

QString CT_Polygon2D::getType() const
{
    return staticGetType();
}

QString CT_Polygon2D::staticGetType()
{
    return CT_AbstractAreaShape2D::staticGetType() + "/CT_Polygon2D";
}

CT_AbstractItemDrawable* CT_Polygon2D::copy(const CT_OutAbstractItemModel *model,
                                           const CT_AbstractResult *result,
                                           CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Polygon2D *polygon = new CT_Polygon2D((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_Polygon2DData&)getData()).clone() : NULL);
    polygon->setId(id());

    polygon->setAlternativeDrawManager(getAlternativeDrawManager());

    return polygon;
}

