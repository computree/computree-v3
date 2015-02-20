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

#ifndef CT_DATASOURCE_H
#define CT_DATASOURCE_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_shape2ddata/ct_shape2ddata.h"

#include <QSharedPointer>

/**
  * Représente une liste de readers au même format
  */
class PLUGINSHAREDSHARED_EXPORT CT_DataSource : public CT_AbstractSingularItemDrawable
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:
    /**
      * \brief Contructeur vide
      */
    CT_DataSource();
    /**
      * \brief Contructeur
      */
    CT_DataSource(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result);

    CT_DataSource(const QString &modelName,
                     const CT_AbstractResult *result);

    virtual ~CT_DataSource();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual bool addReader(CT_AbstractReader* reader);

    bool activateNextReader();

    const QSharedPointer<CT_AbstractReader> getActiveReader() const;

    const QSharedPointer<CT_AbstractReader> getNextReader();

    int getNumberOfReader() const;

    virtual void init(int n = 0) const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);


protected:
    QList<QSharedPointer<CT_AbstractReader> >        _readers;
    mutable int                                      _activeReader;
    int                                              _lastReaderIndice;

private:
    CT_DEFAULT_IA_BEGIN(CT_DataSource)
    CT_DEFAULT_IA_V2(CT_DataSource, CT_AbstractCategory::staticInitDataNumber(), &CT_DataSource::getNumberOfReader, QObject::tr("Nombre de readers"))
    CT_DEFAULT_IA_END(CT_DataSource)

};

#endif // CT_DATASOURCE_H
