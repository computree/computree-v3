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

#include "ct_line.h"
#include "math.h"

const CT_StandardLineDrawManager CT_Line::LINE_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Line)

CT_Line::CT_Line() : CT_AbstractShape()
{
    setBaseDrawManager(&LINE_DRAW_MANAGER);
}

CT_Line::CT_Line(const CT_OutAbstractSingularItemModel *model,
                 const CT_AbstractResult *result,
                 CT_LineData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&LINE_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates(0) = std::min(data->x1(), data->x2());
        _minCoordinates(1) = std::min(data->y1(), data->y2());
        _minCoordinates(2) = std::min(data->z1(), data->z2());

        _maxCoordinates(0) = std::max(data->x1(), data->x2());
        _maxCoordinates(1) = std::max(data->y1(), data->y2());
        _maxCoordinates(2) = std::max(data->z1(), data->z2());
    }
}

CT_Line::CT_Line(const QString &modelName,
                 const CT_AbstractResult *result,
                 CT_LineData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&LINE_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates(0) = std::min(data->x1(), data->x2());
        _minCoordinates(1) = std::min(data->y1(), data->y2());
        _minCoordinates(2) = std::min(data->z1(), data->z2());

        _maxCoordinates(0) = std::max(data->x1(), data->x2());
        _maxCoordinates(1) = std::max(data->y1(), data->y2());
        _maxCoordinates(2) = std::max(data->z1(), data->z2());
    }
}

QString CT_Line::name() const
{
    return tr("3D line");
}

CT_AbstractItemDrawable* CT_Line::copy(const CT_OutAbstractItemModel *model,
                                       const CT_AbstractResult *result,
                                       CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Line *line = new CT_Line((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_LineData&)getData()).clone() : NULL);
    line->setId(id());

    line->setAlternativeDrawManager(getAlternativeDrawManager());

    return line;
}

CT_Line* CT_Line::staticCreateLineFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                 quint64 id,
                                                 const CT_AbstractResult *result,
                                                 const CT_AbstractPointCloudIndex &pointCloudIndex)
{
    Q_UNUSED(id)

    CT_LineData *data = CT_LineData::staticCreateLineDataFromPointCloud(pointCloudIndex);

    if(data == NULL)
        return NULL;

    return new CT_Line((const CT_OutAbstractSingularItemModel *)model, result, data);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_LineData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Line)
#endif
