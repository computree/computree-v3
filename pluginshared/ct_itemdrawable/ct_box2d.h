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

#ifndef CT_BOX2D_H
#define CT_BOX2D_H

#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardbox2ddrawmanager.h"

#include "ct_shape2ddata/ct_box2ddata.h"

/*!
 * \class CT_Box2D
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_Box2DData</b>
 *
 * It represents a box in 2D, defined by a width and an height.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Box2D : public CT_AbstractAreaShape2D
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:

    CT_Box2D();

    /**
      * \brief Contructeur avec une instance des donnes (CT_Box2DData*), ne peut etre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Box2D(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              CT_Box2DData *data);

    CT_Box2D(const QString &modelName,
              const CT_AbstractResult *result,
              CT_Box2DData *data);


    /**
      * ATTENTION : ne pas oublier de redfinir ces deux méthodes si vous héritez de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    float getWidth() const;
    float getHeight() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:

    CT_DEFAULT_IA_BEGIN(CT_Box2D)
    CT_DEFAULT_IA_V2(CT_Box2D, CT_AbstractCategory::staticInitDataLength(), &CT_Box2D::getWidth, QObject::tr("Largeur_X"))
    CT_DEFAULT_IA_V2(CT_Box2D, CT_AbstractCategory::staticInitDataLength(), &CT_Box2D::getHeight, QObject::tr("Hauteur_Y"))
    CT_DEFAULT_IA_END(CT_Box2D)

    const static CT_StandardBox2DDrawManager   BOX2D_DRAW_MANAGER;

};

#endif // CT_BOX2D_H
