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

#include "ct_circle.h"

#include "math.h"

const CT_StandardCircleDrawManager CT_Circle::CIRCLE_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Circle)

CT_Circle::CT_Circle() : CT_AbstractShape()
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
}

CT_Circle::CT_Circle(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_CircleData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
//    if (data!=NULL) {computeBoundingBox();}
}

CT_Circle::CT_Circle(const QString &modelName,
                     const CT_AbstractResult *result, CT_CircleData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
    //    if (data!=NULL) {computeBoundingBox();}
}

double CT_Circle::getRadius() const
{
    return ((const CT_CircleData&)getData()).getRadius();
}

double CT_Circle::getError() const
{
    return ((const CT_CircleData&)getData()).getError();
}

CT_AbstractItemDrawable* CT_Circle::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Circle *circle = new CT_Circle((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_CircleData&)getData()).clone() : NULL);
    circle->setId(id());
    circle->setAlternativeDrawManager(getAlternativeDrawManager());

    return circle;
}

CT_AbstractItemDrawable* CT_Circle::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Circle *circle = new CT_Circle(modelName, result, (getPointerData() != NULL) ? ((const CT_CircleData&)getData()).clone() : NULL);
    circle->setId(id());
    circle->setAlternativeDrawManager(getAlternativeDrawManager());

    return circle;
}

CT_Circle* CT_Circle::staticCreateZAxisAlignedCircleFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                   quint64 id,
                                                                   const CT_AbstractResult *result,
                                                                   const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                   double z)
{
    Q_UNUSED(id)

    CT_CircleData *data = CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloud(pointCloudIndex, z);

    if(data == NULL)
    {
        return NULL;
    }

    return new CT_Circle((const CT_OutAbstractSingularItemModel *)model, result, data);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_CircleData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Circle)
#endif
