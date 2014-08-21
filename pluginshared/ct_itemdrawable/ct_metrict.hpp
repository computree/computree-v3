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

#ifndef CT_METRIC_HPP
#define CT_METRIC_HPP

#include "ct_itemdrawable/ct_metrict.h"
#include "ct_tools/ct_numerictostringconversiont.h"

#include <typeinfo>

template< typename T> CT_DEFAULT_IA_INIT(CT_MetricT<T>)

template< typename T>
CT_MetricT<T>::CT_MetricT() : CT_AbstractMetric()
{
}

template< typename T>
CT_MetricT<T>::CT_MetricT(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const T &value) : CT_AbstractMetric(model, result)
{
    _value = value;
}

template< typename T>
CT_MetricT<T>::CT_MetricT(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const T &value, QString label) : CT_AbstractMetric(model, result, label)
{
    _value = value;
}

template< typename T>
CT_MetricT<T>::CT_MetricT(const QString &modelName, const CT_AbstractResult *result, const T &value) : CT_AbstractMetric(modelName, result)
{
    _value = value;
}

template< typename T>
CT_MetricT<T>::CT_MetricT(const QString &modelName, const CT_AbstractResult *result, const T &value, QString label) : CT_AbstractMetric(modelName, result, label)
{
    _value = value;
}

template< typename T>
CT_MetricT<T>::~CT_MetricT()
{

}

template< typename T>
QString CT_MetricT<T>::getType() const
{
    return staticGetType();
}

template< typename T>
QString CT_MetricT<T>::staticGetType()
{
    return CT_AbstractMetric::staticGetType() + "/CT_MetricT<" + typeid(T).name() + ">";
}

template< typename T>
QString CT_MetricT<T>::name() const
{
    return QString("CT_MetricT<") + typeid(T).name() + QString(">");
}

template< typename T>
QString CT_MetricT<T>::toString() const
{
    return CT_NumericToStringConversionT<T>::toString(getValue());
}

template< typename T>
CT_AbstractItemDrawable* CT_MetricT<T>::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_MetricT<T>* cpy = new CT_MetricT<T>((const CT_OutAbstractSingularItemModel *)model, result, _value);
    cpy->setId(this->id());

    cpy->setAlternativeDrawManager(this->getAlternativeDrawManager());

    return cpy;
}

#endif // CT_METRIC_HPP
