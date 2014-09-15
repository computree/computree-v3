/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre Piboule (ONF)

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

#ifndef CT_POLYGON2D_OLD_H
#define CT_POLYGON2D_OLD_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardpolygon2ddrawmanager.h"

#include "ct_shapedata/ct_polygon2ddata_old.h"

/*!
 * \class CT_Polygon2D_Old
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_Polygon2D_OldData</b>
 *
 * It represents a polygon in 2D.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Polygon2D_Old : public CT_AbstractShape
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:    

    CT_Polygon2D_Old();
    /**
      * \brief Contructeur avec une instance des donnes (CT_Polygon2D_OldData*), ne peut tre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Polygon2D_Old(const CT_OutAbstractSingularItemModel *model,
                const CT_AbstractResult *result,
                CT_Polygon2DData_Old *data);

    CT_Polygon2D_Old(const QString &modelName,
                const CT_AbstractResult *result,
                CT_Polygon2DData_Old *data);

    const QVector<QVector2D*>& getVertices() const;

    inline int getNumberOfVertices() const {return getVertices().size();}
    inline float getArea() const {return ((CT_Polygon2DData_Old*) getDataNotConst())->getAreaIfNotSelfIntersecting();}

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model,
                const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    const static CT_StandardPolygon2DDrawManager POLYGON2D_DRAW_MANAGER;

    CT_DEFAULT_IA_BEGIN(CT_Polygon2D_Old)
    CT_DEFAULT_IA_V2(CT_Polygon2D_Old, CT_AbstractCategory::staticInitDataSize(), &CT_Polygon2D_Old::getNumberOfVertices, tr("Number Of Vertices"))
    CT_DEFAULT_IA_END(CT_Polygon2D_Old)
};

#endif // CT_POLYGON2D_OLD_H
