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

CT_AbstractItemDrawableWithoutPointCloud::CT_AbstractItemDrawableWithoutPointCloud(const CT_OutAbstractItemModel *model,
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

int CT_AbstractItemDrawableWithoutPointCloud::recursiveSearchChildLevelThatMatchType(const QString &type, CT_AbstractItemDrawable &parent, int level)
{
    if(level > 10)
        return 0;

    int newLevel = 0;

    if(parent.beginIterateChild())
    {
        CT_AbstractItemDrawable *child = parent.nextChild();

        if((dynamic_cast<CT_AbstractItemDrawableWithPointCloud*>(child) != NULL)
                && child->getType().startsWith(type))
        {
            return level+1;
        }

        if((newLevel = recursiveSearchChildLevelThatMatchType(type, *child, level+1)) > 0)
        {
            return newLevel;
        }
    }

    return 0;
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_AbstractItemDrawableWithoutPointCloud)
#endif
