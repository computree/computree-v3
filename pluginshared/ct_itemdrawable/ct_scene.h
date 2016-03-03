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



#ifndef CT_SCENE_H
#define CT_SCENE_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"

/*!
 * \class CT_Scene
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable representing a point cloud scene</b>
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Scene : public CT_AbstractItemDrawableWithPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_Scene, CT_AbstractItemDrawableWithPointCloud)

public:
    CT_Scene();

    /**
     * @brief Create a ItemDrawable with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_Scene(const CT_OutAbstractSingularItemModel *model,
             const CT_AbstractResult *result);

    /**
     * @brief Create a ItemDrawable with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     *
     * @param pcir : a point cloud registered to the repository. You can pass a point cloud registered by another ItemDrawable if you
     *               want because sharing is managed automatically.
     */
    CT_Scene(const CT_OutAbstractSingularItemModel *model,
             const CT_AbstractResult *result,
             CT_PCIR pcir);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_Scene(const QString &modelName,
             CT_AbstractResult *result);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     *
     * @param pcir : a point cloud registered to the repository. You can pass a point cloud registered by another ItemDrawable if you
     *               want because sharing is managed automatically.
     */
    CT_Scene(const QString &modelName,
             CT_AbstractResult *result,
             CT_PCIR pcir);

    virtual ~CT_Scene();

    virtual int getFastestIncrement() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
     * @brief PointCloudIndexRegistered setter
     * @param pcir : le nuage d'index enregistré
     */
    void setPointCloudIndexRegistered(CT_PCIR pcir);

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithPointCloud>(*this);

        ar & _boundingBox;
    }
};

BOOST_CLASS_EXPORT_KEY(CT_Scene)
#else
};
#endif

#endif // CT_SCENE_H
