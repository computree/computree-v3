/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#ifndef CT_ABSTRACTSHAPE2D_H
#define CT_ABSTRACTSHAPE2D_H

#include "ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_shape2ddata/ct_shape2ddata.h"

/**
  * Représente une forme géomtrique 2D
  */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractShape2D : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractShape2D, CT_AbstractItemDrawableWithoutPointCloud, 2D shape)

public:
    /**
      * \brief Contructeur vide (seulement pour la srialisation avec BOOST).
      */
    CT_AbstractShape2D();
    /**
      * \brief Contructeur avec une instance des donnes (CT_Shape2DData*), ne peut être NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_AbstractShape2D(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result,
                     CT_Shape2DData *data);

    CT_AbstractShape2D(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_Shape2DData *data);

    virtual ~CT_AbstractShape2D();

    void setCenterX(double x);
    void setCenterY(double y);
    void setCenterCoordinate(const Eigen::Vector3d &center);

    void setZValue(double z);
    double getZValue() const;
    bool isZValueDefined() const;

    double getCenterX() const;
    double getCenterY() const;

    const CT_Shape2DData* getPointerData() const;
    const CT_Shape2DData& getData() const;
    const Eigen::Vector2d& getCenter() const;

    void getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const;


private:

    CT_Shape2DData   *_data;
    double            _zValue;
    bool              _zValueDefined;

protected:

    CT_Shape2DData* getDataNotConst() const;

    CT_DEFAULT_IA_BEGIN(CT_AbstractShape2D)
    CT_DEFAULT_IA_V3(CT_AbstractShape2D, CT_AbstractCategory::staticInitDataZ(), &CT_AbstractShape2D::getZValue, QObject::tr("Z"), "z")
    CT_DEFAULT_IA_END(CT_AbstractShape2D)

};

#endif // CT_ABSTRACTSHAPE2D_H
