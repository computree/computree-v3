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

#include "ct_fileheader.h"

CT_DEFAULT_IA_INIT(CT_FileHeader)

CT_FileHeader::CT_FileHeader() : CT_AbstractItemDrawableWithoutPointCloud()
{
}

CT_FileHeader::CT_FileHeader(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{

}

CT_FileHeader::CT_FileHeader(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{

}

CT_FileHeader::~CT_FileHeader()
{
}

CT_AbstractItemDrawable *CT_FileHeader::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_FileHeader *cpy =  new CT_FileHeader((CT_OutAbstractSingularItemModel*) model, result);
    cpy->setFile(_fileInfo.filePath());
    return cpy;
}

void CT_FileHeader::setFile(const QString &filename)
{
    _fileInfo = QFileInfo(filename);
}

void CT_FileHeader::setMinAndMaxCoordinates(double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
    _minCoordinates(0) = minx;
    _minCoordinates(1) = miny;
    _minCoordinates(2) = minz;

    _maxCoordinates(0) = maxx;
    _maxCoordinates(1) = maxy;
    _maxCoordinates(2) = maxz;
}



