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

#include "ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_abstractitemdrawablewithpointcloud.h"

#include <QFile>

const CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager CT_AbstractItemDrawableWithoutPointCloud::IDWITHOUTPC_DRAW_MANAGER;

CT_AbstractItemDrawableWithoutPointCloud::CT_AbstractItemDrawableWithoutPointCloud() : CT_AbstractSingularItemDrawable()
{
    setBaseDrawManager(&IDWITHOUTPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithoutPointCloud::CT_AbstractItemDrawableWithoutPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{
    setBaseDrawManager(&IDWITHOUTPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithoutPointCloud::CT_AbstractItemDrawableWithoutPointCloud(const QString &modelName,
                                                                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{
    setBaseDrawManager(&IDWITHOUTPC_DRAW_MANAGER);
}

QString CT_AbstractItemDrawableWithoutPointCloud::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_AbstractItemDrawableWithoutPointCloud";
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_AbstractItemDrawableWithoutPointCloud)
#endif
