/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ct_jsonobject.h"


CT_JsonObject::CT_JsonObject() : CT_AbstractItemDrawableWithoutPointCloud()
{
}

CT_JsonObject::CT_JsonObject(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, QJsonObject *jsonObject) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _jsonObject = jsonObject;
}


CT_JsonObject::CT_JsonObject(const QString &modelName, const CT_AbstractResult *result, QJsonObject *jsonObject) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _jsonObject = jsonObject;
}

QString CT_JsonObject::getType() const
{
    return staticGetType();
}

QString CT_JsonObject::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_JsonObject";
}

CT_AbstractItemDrawable* CT_JsonObject::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_JsonObject *jsonObject = new CT_JsonObject((const CT_OutAbstractSingularItemModel *)model, result, *_jsonObject);

    jsonObject->setAlternativeDrawManager(getAlternativeDrawManager());

    return jsonObject;
}

CT_AbstractItemDrawable *CT_JsonObject::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_JsonObject *jsonObject = new CT_JsonObject(modelName, result, *_jsonObject);

    jsonObject->setAlternativeDrawManager(getAlternativeDrawManager());

    return jsonObject;
}


// Private constructors
CT_JsonObject::CT_JsonObject(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const QJsonObject &jsonObject) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _jsonObject = new QJsonObject(jsonObject);
}

CT_JsonObject::CT_JsonObject(const QString &modelName, const CT_AbstractResult *result, const QJsonObject &jsonObject) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _jsonObject = new QJsonObject(jsonObject);
}
