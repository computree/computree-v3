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

#ifndef CT_ABSTRACTSHAPE_H
#define CT_ABSTRACTSHAPE_H

#include "ct_abstractitemdrawablewithoutpointcloud.h"
#include "ct_shapedata/ct_shapedata.h"


/**
  * Reprsente une forme gomtrique 2D ou 3D
  */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractShape : public CT_AbstractItemDrawableWithoutPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractShape, CT_AbstractItemDrawableWithoutPointCloud, 3D shape)

public:
    /**
      * \brief Contructeur vide (seulement pour la srialisation avec BOOST).
      */
    CT_AbstractShape();
    /**
      * \brief Contructeur avec une instance des donnes (CT_ShapeData*), ne peut être NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_AbstractShape(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result,
                     CT_ShapeData *data);

    CT_AbstractShape(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_ShapeData *data);

    virtual ~CT_AbstractShape();

    void setCenterX(double x);
    void setCenterY(double y);
    void setCenterZ(double z);
    void setCenterCoordinate(const Eigen::Vector3d& center);


    double getCenterX() const;
    double getCenterY() const;
    double getCenterZ() const;
    const Eigen::Vector3d& getCenterCoordinate() const;


    const CT_ShapeData* getPointerData() const;
    const CT_ShapeData& getData() const;
    const Eigen::Vector3d& getCenter() const;
    const Eigen::Vector3d& getDirection() const;
    double getDirectionX() const;
    double getDirectionY() const;
    double getDirectionZ() const;

private:

    CT_ShapeData   *_data;

protected:

    CT_ShapeData* getDataNotConst() const;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithoutPointCloud>(*this);
        ar & _data;
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(CT_AbstractShape)
BOOST_CLASS_EXPORT_KEY(CT_AbstractShape)

#else
};
#endif

#endif // CT_ABSTRACTSHAPE_H
