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

#ifndef CT_DATASOURCEGEO_H
#define CT_DATASOURCEGEO_H

#include "ct_datasource.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_shape2ddata/ct_shape2ddata.h"

/**
  * Représente une liste de readers au même format
  */
class PLUGINSHAREDSHARED_EXPORT CT_DataSourceGeo : public CT_DataSource
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:
    /**
      * \brief Contructeur vide
      */
    CT_DataSourceGeo();
    /**
      * \brief Contructeur
      */
    CT_DataSourceGeo(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result);

    CT_DataSourceGeo(const QString &modelName,
                     const CT_AbstractResult *result);

    virtual ~CT_DataSourceGeo();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual bool hasBoundingBox() const {return true;}

    virtual bool addReader(CT_AbstractReader* reader);

    QList<QSharedPointer<CT_AbstractReader> > getReadersIntersecting(const CT_Shape2DData &data) const;
    QList<QSharedPointer<CT_AbstractReader> > getReadersIntersecting(const Eigen::Vector3d &min, const Eigen::Vector3d &max) const;

    bool intersects(const CT_Shape2DData &data);

    CT_AbstractItemDrawable *copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

};

#endif // CT_DATASOURCEGEO_H
