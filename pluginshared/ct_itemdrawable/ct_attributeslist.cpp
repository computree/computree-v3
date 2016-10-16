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

#include "ct_attributeslist.h"
#include "qdebug.h"


CT_AttributesList::CT_AttributesList() : CT_AbstractItemDrawableWithoutPointCloud()
{
}

CT_AttributesList::CT_AttributesList(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
}

CT_AttributesList::CT_AttributesList(const QString &modelName,
                        const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{

}

CT_AbstractItemDrawable* CT_AttributesList::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_AttributesList *ref = new CT_AttributesList((const CT_OutAbstractSingularItemModel *)model, result);
    ref->setAlternativeDrawManager(getAlternativeDrawManager());

    return ref;
}

CT_AbstractItemDrawable *CT_AttributesList::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_AttributesList *ref = new CT_AttributesList(modelName, result);
    ref->setAlternativeDrawManager(getAlternativeDrawManager());

    return ref;
}
