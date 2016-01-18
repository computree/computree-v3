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

#include "ct_plotlistingrid.h"

CT_DEFAULT_IA_INIT(CT_PlotListInGrid)

CT_PlotListInGrid::CT_PlotListInGrid() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _areaXY = NULL;
}

CT_PlotListInGrid::CT_PlotListInGrid(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result,
                                     const CT_AreaShape2DData *areaShape2D,
                                     const Eigen::Vector2d &refCoords,
                                     double spacing,
                                     double size,
                                     CT_PlotListInGrid::Type type) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _areaXY = (CT_AreaShape2DData*) areaShape2D->copy();
    _spacing = spacing;
    _size = size;
    _type = type;
    computeMinMax(refCoords);
}

CT_PlotListInGrid::CT_PlotListInGrid(const QString &modelName,
                                     const CT_AbstractResult *result,
                                     const CT_AreaShape2DData *areaShape2D,
                                     const Eigen::Vector2d &refCoords,
                                     double spacing,
                                     double size,
                                     CT_PlotListInGrid::Type type) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _areaXY = (CT_AreaShape2DData*) areaShape2D->copy();
    _spacing = spacing;
    _size = size;
    _type = type;
    computeMinMax(refCoords);
}

void CT_PlotListInGrid::computeMinMax(const Eigen::Vector2d &refCoords)
{
    Eigen::Vector3d minShape, maxShape;
    _areaXY->getBoundingBox(minShape, maxShape);

    Eigen::Vector2d _min, _max;

    _min(0) = std::floor((minShape(0) - refCoords(0)) / _spacing) * _spacing + refCoords(0);
    _min(1) = std::floor((minShape(1) - refCoords(1)) / _spacing) * _spacing + refCoords(1);

    while (_min(0) < minShape(0)) {_min(0) += _spacing;}
    while (_min(1) < minShape(1)) {_min(1) += _spacing;}

    _max(0) = _min(0);
    _max(1) = _min(1);

    size_t ncol = 0;
    size_t nrow = 0;

    while (_max(0) < maxShape(0)) {ncol++; _max(0) += _spacing;}
    while (_max(1) < maxShape(1)) {nrow++; _max(1) += _spacing;}

}


CT_PlotListInGrid::~CT_PlotListInGrid()
{
}

QString CT_PlotListInGrid::getType() const
{
    return staticGetType();
}

QString CT_PlotListInGrid::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_PlotListInGrid";
}


CT_AbstractItemDrawable *CT_PlotListInGrid::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_PlotListInGrid *cpy = new CT_PlotListInGrid((const CT_OutAbstractSingularItemModel*)model, result, _areaXY, _min, _max, _spacing, _size, _type);

    return cpy;
}

CT_PlotListInGrid::CT_PlotListInGrid(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result,
                                     const CT_AreaShape2DData *areaShape2D,
                                     const Eigen::Vector2d &min,
                                     const Eigen::Vector2d &max,
                                     double spacing,
                                     double size,
                                     CT_PlotListInGrid::Type type) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _areaXY = (CT_AreaShape2DData*) areaShape2D->copy();
    _spacing = spacing;
    _size = size;
    _type = type;
    _min = min;
    _max = max;

}
