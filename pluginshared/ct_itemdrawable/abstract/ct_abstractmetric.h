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

#ifndef CT_ABSTRACTMETRIC_H
#define CT_ABSTRACTMETRIC_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"

/*!
 * \class CT_AbstractMetric
 * \ingroup PluginShared_Items
 * \brief <b>Commun abstract base of all templated CT_Metric<type> </b>
 *
 * It's usefull to manage generically a attribute data
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractMetric : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
public:

    /**
      * \brief Empty Contructor vide
      */
    CT_AbstractMetric();
    CT_AbstractMetric(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result);
    CT_AbstractMetric(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const QString &label);

    CT_AbstractMetric(const QString &modelName, const CT_AbstractResult *result);
    CT_AbstractMetric(const QString &modelName, const CT_AbstractResult *result, const QString &label);

    virtual ~CT_AbstractMetric();

    virtual QString getType() const;
    static QString staticGetType();

    virtual QString name() const = 0;

    virtual double getDoubleValue() const = 0;

    inline void setLabel(QString label) {_label = label;}
    inline const QString getLabel() const {return _label;}

    virtual QString toString() const = 0;

    virtual bool hasBoundingBox() const {return false;}

protected:
    QString _label;

};

#endif // CT_ABSTRACTMETRIC_H
