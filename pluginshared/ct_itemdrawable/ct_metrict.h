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

#ifndef CT_METRIC_H
#define CT_METRIC_H

#include "ct_math/ct_math.h"
#include "ct_itemdrawable/abstract/ct_abstractmetric.h"

template< typename T>
class CT_MetricT : public CT_AbstractMetric
{
public:
    /**
      * \brief Empty Contructor vide
      */
    CT_MetricT();


    /*!
      * \brief Contructor with integer column and row coordinates
      *
      * \param model Item model for creation
      * \param result Result containing the item
      * \param value Value
      */
    CT_MetricT(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const T &value);

    /*!
     * \brief Contructor with integer column and row coordinates
     *
     * \param model Item model for creation
     * \param result Result containing the item
     * \param value Value
     * \param label Label
     */
    CT_MetricT(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const T &value, QString label);

    CT_MetricT(const QString &modelName, const CT_AbstractResult *result, const T &value);
    CT_MetricT(const QString &modelName, const CT_AbstractResult *result, const T &value, QString label);

    virtual ~CT_MetricT();
    
    virtual QString getType() const;
    static QString staticGetType();

    virtual QString name() const;

    inline void setValue(const T &value) {_value = value;}
    inline T getValue() const {return _value;}
    inline const T& constValue() const { return _value; }

    inline virtual double getDoubleValue() const { return (double) _value;}

    QString toString() const;

    /*!
     * \brief Copy method
     *
     * \param model Item model for the copy
     * \param result Result containing the copy
     * \param copyModeList Copy mode
     * \return Item copy
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

protected:
    T       _value;

private:

    CT_DEFAULT_IA_BEGIN(CT_MetricT<T>)
    CT_DEFAULT_IA_V2(CT_MetricT<T>, CT_AbstractCategory::staticInitDataValue(), &CT_MetricT<T>::getValue, QObject::tr("Value"))
    CT_DEFAULT_IA_END(CT_MetricT<T>)
};

// Spécialisations
template<>
inline double CT_MetricT<QString>::getDoubleValue() const { return 0.0; }

template<>
inline QString CT_MetricT<QString>::toString() const { return constValue(); }

// fin des spécialisations

// Includes the template implementations
#include "ct_itemdrawable/ct_metrict.hpp"

#endif // CT_METRIC_H
