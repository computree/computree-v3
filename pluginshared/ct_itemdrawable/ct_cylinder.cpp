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

#include "ct_cylinder.h"
#include "ct_line.h"

CT_DEFAULT_IA_INIT(CT_Cylinder)

const CT_StandardCylinderDrawManager CT_Cylinder::CYLINDER_DRAW_MANAGER;

CT_Cylinder::CT_Cylinder() : CT_AbstractShape()
{
    setBaseDrawManager(&CYLINDER_DRAW_MANAGER);
}

CT_Cylinder::CT_Cylinder(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result,
                         CT_CylinderData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&CYLINDER_DRAW_MANAGER);
}

CT_Cylinder::CT_Cylinder(const QString &modelName,
                         const CT_AbstractResult *result,
                         CT_CylinderData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&CYLINDER_DRAW_MANAGER);
}

QString CT_Cylinder::name() const
{
    return tr("Cylinder");
}

double CT_Cylinder::getRadius() const
{
    return ((const CT_CylinderData&)getData()).getRadius();
}

double CT_Cylinder::getHeight() const
{
    return ((const CT_CylinderData&)getData()).getHeight();
}

double CT_Cylinder::getLineError() const
{
    return ((const CT_CylinderData&)getData()).getLineError();
}

double CT_Cylinder::getCircleError() const
{
    return ((const CT_CylinderData&)getData()).getCircleError();
}

CT_AbstractItemDrawable* CT_Cylinder::copy(const CT_OutAbstractItemModel *model,
                                           const CT_AbstractResult *result,
                                           CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Cylinder *cylinder = new CT_Cylinder((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_CylinderData&)getData()).clone() : NULL);
    cylinder->setId(id());

    cylinder->setAlternativeDrawManager(getAlternativeDrawManager());

    return cylinder;
}

CT_Cylinder* CT_Cylinder::staticCreate3DCylinderFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                               quint64 id,
                                                               const CT_AbstractResult *result,
                                                               const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                               const Eigen::Vector3d &pointCloudBarycenter)
{
    Q_UNUSED(id)

    CT_CylinderData *data = CT_CylinderData::staticCreate3DCylinderDataFromPointCloud(pointCloudIndex,
                                                                                      pointCloudBarycenter);

    if(data == NULL)
        return NULL;

    return new CT_Cylinder((const CT_OutAbstractSingularItemModel *)model, result, data);
}

CT_Cylinder* CT_Cylinder::staticCreate3DCylinderFromPointCloud(const QString &modelName,
                                                               quint64 id,
                                                               const CT_AbstractResult *result,
                                                               const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                               const Eigen::Vector3d &pointCloudBarycenter)
{
    Q_UNUSED(id)

    CT_CylinderData *data = CT_CylinderData::staticCreate3DCylinderDataFromPointCloud(pointCloudIndex,
                                                                                      pointCloudBarycenter);

    if(data == NULL)
        return NULL;

    return new CT_Cylinder(modelName, result, data);
}


#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_CylinderData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Cylinder)
#endif
