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

#ifndef CT_CIRCLE_H
#define CT_CIRCLE_H

#include "ct_itemdrawable/abstract/ct_abstractshape.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardcircledrawmanager.h"

#include "ct_shapedata/ct_circledata.h"

/*!
 * \class CT_Circle
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable managing a CT_CircleData</b>
 *
 * It represents a circle in 3D, defined by a center, a direction and a radius.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Circle : public CT_AbstractShape
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:

    CT_Circle();

    /**
      * \brief Contructeur avec une instance des donnes (CT_CircleData*), ne peut tre NULL ! (Supprime dans le destructeur de la classe).
      */
    CT_Circle(const CT_OutAbstractSingularItemModel *model,
              const CT_AbstractResult *result,
              CT_CircleData *data);

    CT_Circle(const QString &modelName,
              const CT_AbstractResult *result,
              CT_CircleData *data);


    /**
      * ATTENTION : ne pas oublier de redfinir ces deux méthodes si vous héritez de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    float getRadius() const;
    float getError() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
      * \brief Retourne un cercle 2D  partir du nuage de points pass en paramtre.
      *
      * \return NULL si le nombre de points est infrieur  3.
      */
    static CT_Circle* staticCreateZAxisAlignedCircleFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                   quint64 id,
                                                                   const CT_AbstractResult *result,
                                                                   const CT_AbstractPointCloud &pointCloud,
                                                                   const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                   double z = 0);

private:

    CT_DEFAULT_IA_BEGIN(CT_Circle)
    CT_DEFAULT_IA_V2(CT_Circle, CT_AbstractCategory::staticInitDataRadius(), &CT_Circle::getRadius, QObject::tr("Rayon du cercle"))
    CT_DEFAULT_IA_V2(CT_Circle, CT_AbstractCategory::staticInitDataR2(), &CT_Circle::getError, QObject::tr("Erreur d'ajustement du cercle"))
    CT_DEFAULT_IA_END(CT_Circle)

    const static CT_StandardCircleDrawManager   CIRCLE_DRAW_MANAGER;

//    void computeBoundingBox();

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractShape>(*this);
    }
};

BOOST_CLASS_EXPORT_KEY(CT_Circle)

#else
};
#endif

#endif // CT_CIRCLE_H
