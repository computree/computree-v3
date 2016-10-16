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

#ifndef CT_ATTRIBUTESLIST_H
#define CT_ATTRIBUTESLIST_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardreferencepointdrawmanager.h"

/*!
 * \class CT_AttributesList
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable representing an attributes list (empty by default)</b>
 *
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_AttributesList : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AttributesList, CT_AbstractItemDrawableWithoutPointCloud, Attribute list)

public:
    CT_AttributesList();

    /**
      * \brief Contructeur
      */
    CT_AttributesList(const CT_OutAbstractSingularItemModel *model,
                            const CT_AbstractResult *result);

    CT_AttributesList(const QString &modelName,
                            const CT_AbstractResult *result);

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

};

#endif // CT_ATTRIBUTESLIST_H
