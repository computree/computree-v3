/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
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

#include "ct_itemdrawable/ct_loopcounter.h"

CT_DEFAULT_IA_INIT(CT_LoopCounter)

CT_LoopCounter::CT_LoopCounter() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _counter = QSharedPointer<CT_Counter>();
}

CT_LoopCounter::CT_LoopCounter(const CT_OutAbstractSingularItemModel *model,
                                    const CT_AbstractResult *result,
                                    QSharedPointer<CT_Counter> counter) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _counter = counter;
}

CT_LoopCounter::CT_LoopCounter(const QString &modelName,
                                    const CT_AbstractResult *result,
                                    QSharedPointer<CT_Counter> counter) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _counter = counter;
}

CT_LoopCounter::~CT_LoopCounter()
{
    _counter.clear();
}

CT_AbstractItemDrawable* CT_LoopCounter::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_LoopCounter* cpy = new CT_LoopCounter((const CT_OutAbstractSingularItemModel*)model, result, _counter);

    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}

