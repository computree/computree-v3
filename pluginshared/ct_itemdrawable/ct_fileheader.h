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

#ifndef CT_FILEHEADER_H
#define CT_FILEHEADER_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"

#include <QFileInfo>

/**
  * Représente un header de fichier
  */
class PLUGINSHAREDSHARED_EXPORT CT_FileHeader : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_FileHeader, CT_AbstractItemDrawableWithoutPointCloud, File header)

public:
    /**
      * \brief Contructeur vide (seulement pour la srialisation avec BOOST).
      */
    CT_FileHeader();
    /**
      * \brief Contructeur
      */
    CT_FileHeader(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result);

    CT_FileHeader(const QString &modelName,
                     const CT_AbstractResult *result);

    virtual ~CT_FileHeader();

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    void setFile(const QString &filename);
    inline const QFileInfo& getFileInfo() {return _fileInfo;}

    inline QString getFileName() const {return _fileInfo.fileName();}
    inline QString getDirectory() const {return _fileInfo.path();}

    // By default Ct_FileHeader don't have a Bounding Box : redefine in children class of geographical files
    virtual bool hasBoundingBox() const {return false;}

    void setMinAndMaxCoordinates(double minx, double miny, double minz, double maxx, double maxy, double maxz);


protected:
    QFileInfo     _fileInfo;    

private:

    CT_DEFAULT_IA_BEGIN(CT_FileHeader)
    CT_DEFAULT_IA_V3(CT_FileHeader, CT_AbstractCategory::staticInitDataDisplayableName(), &CT_FileHeader::getFileName, QObject::tr("FileName"), "fn")
    CT_DEFAULT_IA_V3(CT_FileHeader, CT_AbstractCategory::staticInitDataDisplayableName(), &CT_FileHeader::getDirectory, QObject::tr("Directory"), "dir")
    CT_DEFAULT_IA_END(CT_FileHeader)

};

#endif // CT_FILEHEADER_H
