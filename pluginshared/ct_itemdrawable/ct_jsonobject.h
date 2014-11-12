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

#ifndef CT_JSONOBJECT_H
#define CT_JSONOBJECT_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h"
#include <QJsonObject>

/*!
 * \class CT_JsonObject
 * \brief <b>ItemDrawable representing a Kodama Buffer</b>
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_JsonObject : public CT_AbstractItemDrawableWithoutPointCloud
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:
    CT_JsonObject();

    /**
      * \brief Contructeur
      */
    CT_JsonObject(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, QJsonObject* jsonObject);
    CT_JsonObject(const QString &modelName, const CT_AbstractResult *result, QJsonObject* jsonObject);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous héritez de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    inline const QJsonObject *getJsonObject() const {return _jsonObject;}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
    virtual CT_AbstractItemDrawable* copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    QJsonObject *_jsonObject;

    CT_JsonObject(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const QJsonObject &jsonObject);
    CT_JsonObject(const QString &modelName, const CT_AbstractResult *result, const QJsonObject &jsonObject);


};

#endif // CT_JSONOBJECT_H
