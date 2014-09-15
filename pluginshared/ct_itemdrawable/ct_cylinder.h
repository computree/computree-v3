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

#ifndef CT_CYLINDER_H
#define CT_CYLINDER_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardcylinderdrawmanager.h"


#include "ct_shapedata/ct_cylinderdata.h"

/*!
 * \class CT_Cylinder
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_CylinderData</b>
 *
 * It represents a cylinder in 3D, defined by a center, a direction a radius and a height.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Cylinder : public CT_AbstractShape
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:
    CT_Cylinder();


    CT_Cylinder(const CT_OutAbstractSingularItemModel *model,
                const CT_AbstractResult *result,
                CT_CylinderData *data);

    CT_Cylinder(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_CylinderData *data);


    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    float getRadius() const;
    float getHeight() const;
    float getLineError() const;
    float getCircleError() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
      * \brief Retourne un cercle 2D  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  3.
      */
    static CT_Cylinder* staticCreate3DCylinderFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                             quint64 id,
                                                             const CT_AbstractResult *result,
                                                             const CT_AbstractPointCloud &pointCloud,
                                                             const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                             const QVector3D &pointCloudBarycenter);

private:

    CT_DEFAULT_IA_BEGIN(CT_Cylinder)
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataXDirection(), &CT_Cylinder::getDirectionX, tr("Direction X"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataYDirection(), &CT_Cylinder::getDirectionY, tr("Direction Y"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataZDirection(), &CT_Cylinder::getDirectionZ, tr("Direction Z"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataRadius(), &CT_Cylinder::getRadius, tr("Rayon"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataLength(), &CT_Cylinder::getHeight, tr("Longueur"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataR2(), &CT_Cylinder::getCircleError, tr("Erreur d'ajustement du cercle"))
    CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::staticInitDataR2(), &CT_Cylinder::getLineError, tr("Erreur d'ajustement de la ligne"))
    CT_DEFAULT_IA_END(CT_Cylinder)

    const static CT_StandardCylinderDrawManager CYLINDER_DRAW_MANAGER;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractShape>(*this);
    }
};

BOOST_CLASS_EXPORT_KEY(CT_Cylinder)

#else
};
#endif

#endif // CT_CYLINDER_H
