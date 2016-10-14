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

#ifndef CT_READERITEM_H
#define CT_READERITEM_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"

#include "ct_reader/abstract/ct_abstractreader.h"

/**
  * Représente un header de fichier
  */
class PLUGINSHAREDSHARED_EXPORT CT_ReaderItem : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_ReaderItem, CT_AbstractItemDrawableWithoutPointCloud)

public:
    /**
      * \brief Contructeur vide (seulement pour la srialisation avec BOOST).
      */
    CT_ReaderItem();
    /**
      * \brief Contructeur
      */
    CT_ReaderItem(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_AbstractReader* reader);

    CT_ReaderItem(const QString &modelName,
                     const CT_AbstractResult *result, CT_AbstractReader* reader);

    virtual ~CT_ReaderItem();

    virtual QString name() const;


    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    // By default CT_ReaderItem don't have a Bounding Box : redefine in children class of geographical files
    virtual bool hasBoundingBox() const {return false;}

    inline QString getReaderFilePath() const {if (_reader == NULL) {return "";} else {return _reader->filepath();}}
    inline CT_AbstractReader* getReader() {return _reader;}

    inline QString getReaderName() const {if (_reader == NULL) {return "";} else {return _reader->GetReaderName();}}

private:

    CT_AbstractReader*  _reader;

    CT_DEFAULT_IA_BEGIN(CT_ReaderItem)
    CT_DEFAULT_IA_V3(CT_ReaderItem, CT_AbstractCategory::staticInitDataDisplayableName(), &CT_ReaderItem::getReaderName, QObject::tr("ReaderType"), "rn")
    CT_DEFAULT_IA_V3(CT_ReaderItem, CT_AbstractCategory::staticInitDataFileName(), &CT_ReaderItem::getReaderFilePath, QObject::tr("FilePath"), "fi")
    CT_DEFAULT_IA_END(CT_ReaderItem)

};

#endif // CT_READERITEM_H
