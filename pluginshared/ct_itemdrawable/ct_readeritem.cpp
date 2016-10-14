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

#include "ct_readeritem.h"

CT_DEFAULT_IA_INIT(CT_ReaderItem)

CT_ReaderItem::CT_ReaderItem() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _reader = NULL;
}

CT_ReaderItem::CT_ReaderItem(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result, CT_AbstractReader* reader) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _reader = reader;
}

CT_ReaderItem::CT_ReaderItem(const QString &modelName,
                                   const CT_AbstractResult *result, CT_AbstractReader* reader) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _reader = reader;
}

CT_ReaderItem::~CT_ReaderItem()
{
}

QString CT_ReaderItem::name() const
{
    return tr("Reader item");
}

CT_AbstractItemDrawable *CT_ReaderItem::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_ReaderItem *cpy =  new CT_ReaderItem((CT_OutAbstractSingularItemModel*) model, result, _reader);
    return cpy;
}





