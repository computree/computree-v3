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

#ifndef CT_POLYGON2D_H
#define CT_POLYGON2D_H

#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardpolygon2ddrawmanager.h"

#include "ct_shape2ddata/ct_polygon2ddata.h"

/*!
 * \class CT_Polygon2D
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_Polygon2DData</b>
 *
 * It represents a polygon in 2D.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Polygon2D : public CT_AbstractAreaShape2D
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Polygon2D, CT_AbstractAreaShape2D, 2D polygon)

public:    

    CT_Polygon2D();
    /**
      * \brief Contructeur avec une instance des donnes (CT_Polygon2DData*), ne peut tre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Polygon2D(const CT_OutAbstractSingularItemModel *model,
                const CT_AbstractResult *result,
                CT_Polygon2DData *data);

    CT_Polygon2D(const QString &modelName,
                const CT_AbstractResult *result,
                CT_Polygon2DData *data);

    const QVector<Eigen::Vector2d*>& getVertices() const;

    inline int getNumberOfVertices() const {return getVertices().size();}
    inline void computeCentroid() const {((CT_Polygon2DData*) getDataNotConst())->computeCentroid();}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model,
                const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

private:
    const static CT_StandardPolygon2DDrawManager POLYGON2D_DRAW_MANAGER;

    CT_DEFAULT_IA_BEGIN(CT_Polygon2D)
    CT_DEFAULT_IA_V3(CT_Polygon2D, CT_AbstractCategory::staticInitDataSize(), &CT_Polygon2D::getNumberOfVertices, QObject::tr("Number Of Vertices"), "nov")
    CT_DEFAULT_IA_V3(CT_Polygon2D, CT_AbstractCategory::staticInitDataArea(), &CT_Polygon2D::getArea, QObject::tr("Polygon Area"), "pa")
    CT_DEFAULT_IA_END(CT_Polygon2D)
};

#endif // CT_POLYGON2D_H
