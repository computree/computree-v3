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

#ifndef CT_ABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUD_H
#define CT_ABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUD_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

/**
  * \brief Classe reprsentant un ItemDrawable simple, sans rfrence sur un nuage de points.
  */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawableWithoutPointCloud : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT

public:
    CT_AbstractItemDrawableWithoutPointCloud();
    CT_AbstractItemDrawableWithoutPointCloud(const CT_OutAbstractItemModel *model,
                                             const CT_AbstractResult *result);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItemDrawableWithoutPointCloud(const QString &modelName,
                                             const CT_AbstractResult *result);

    virtual ~CT_AbstractItemDrawableWithoutPointCloud() {}

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const = 0;
    static QString staticGetType();

private:

    const static CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager IDWITHOUTPC_DRAW_MANAGER;

protected:

    /**
      * Recherche le premier enfant du type CT_AbstractItemDrawableWithPointCloud, si il y en a un : le niveau de celui-ci dans l'arborescence est retourn.
      */
    int recursiveSearchChildLevelThatMatchType(const QString &type, CT_AbstractItemDrawable &parent, int level);

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawable>(*this);
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(CT_AbstractItemDrawableWithoutPointCloud)
BOOST_CLASS_EXPORT_KEY(CT_AbstractItemDrawableWithoutPointCloud)
#else
};
#endif

#endif // CT_ABSTRACTITEMDRAWABLEWITHOUTPOINTCLOUD_H
