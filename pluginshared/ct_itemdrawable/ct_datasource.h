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

#include "ct_abstractsingularitemdrawable.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_shape2ddata/ct_shape2ddata.h"

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
                     const CT_AbstractResult *result, CT_AbstractReader* readerPrototype);

    CT_DataSource(const QString &modelName,
                     const CT_AbstractResult *result,CT_AbstractReader* readerPrototype);

    virtual ~CT_DataSource();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual bool addReader(CT_AbstractReader* reader);

    const CT_AbstractReader* getActiveReader() const;

    bool activateNextReader();

    int getNumberOfReader() const;

    virtual void init();


protected:
    CT_AbstractReader*                  _readerPrototype;
    QMap<int, CT_AbstractReader*>       _readers;
    int                                 _activeReader;
    int                                 _lastReaderIndice;

};

#endif // CT_DATASOURCE_H
