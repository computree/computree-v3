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

#ifndef CT_ABSTRACTITEMDRAWABLEWITHPOINTCLOUD_H
#define CT_ABSTRACTITEMDRAWABLEWITHPOINTCLOUD_H

#include "ct_defines.h"

#include "ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/accessibility/ct_iaccesspointcloud.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.h"

/**
 * @brief Represent an singular item that contains a point cloud
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawableWithPointCloud : public CT_AbstractSingularItemDrawable, public CT_IAccessPointCloud
{
    Q_OBJECT

public:

    CT_AbstractItemDrawableWithPointCloud();

    /**
     * @brief Create a ItemDrawable with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractItemDrawableWithPointCloud(const CT_OutAbstractSingularItemModel *model,
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
    CT_AbstractItemDrawableWithPointCloud(const CT_OutAbstractSingularItemModel *model,
                                          const CT_AbstractResult *result,
                                          CT_PCIR pcir);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItemDrawableWithPointCloud(const QString &modelName,
                                          const CT_AbstractResult *result);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     *
     * @param pcir : a point cloud registered to the repository. You can pass a point cloud registered by another ItemDrawable if you
     *               want because sharing is managed automatically.
     */
    CT_AbstractItemDrawableWithPointCloud(const QString &modelName,
                                          const CT_AbstractResult *result,
                                          CT_PCIR pcir);

    virtual ~CT_AbstractItemDrawableWithPointCloud();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const = 0;
    static QString staticGetType();

    /**
     * @brief Update the bounding box. If this item has a valid point cloud index all
     *        points are covered to calculate the bounding box.
     */
    virtual void updateBoundingBox();

    /**
      * \brief Retourne les index du nuage de points.
      */
    const CT_AbstractPointCloudIndex* getPointCloudIndex() const;

    #ifdef USE_PCL
    /**
     * @brief Convert the point cloud index to a pcl cloud.
     * @warning Please re-use the shared_ptr returned because
     *          a new collection is created at each call.
     */
    boost::shared_ptr< CT_PCLCloud > getPCLCloud() const;
    #endif

    /**
     * @brief PointCloudIndexRegistered getter
     * @return retourne le nuage de points enregistré
     */
    CT_PCIR getPointCloudIndexRegistered() const;

    /**
     * @brief return the number of points
     */
    size_t getPointCloudIndexSize() const;

    /**
      * \brief Supprime le nuage de points et les index de la mmoire.
      */
    void clearFromMemory();

    /**
      * \brief Retourne le nombre de point  sauter lorsqu'il faut dessiner rapidement (par dfaut 16).
      */
    virtual int getFastestIncrement() const;

private:

    static CT_StandardAbstractItemDrawableWithPointCloudDrawManager IDWITHPC_DRAW_MANAGER;

    // default attributes
    CT_DEFAULT_IA_BEGIN(CT_AbstractItemDrawableWithPointCloud)
    CT_DEFAULT_IA_V2(CT_AbstractItemDrawableWithPointCloud, CT_AbstractCategory::staticInitDataNumber(), &CT_AbstractItemDrawableWithPointCloud::getPointCloudIndexSize, QObject::tr("Nombre de points"))
    CT_DEFAULT_IA_END(CT_AbstractItemDrawableWithPointCloud)
protected:

    void deletePointCloud();

    /**
     * @brief PointCloudIndexRegistered setter
     * @param pcr : le nuage d'index enregistré
     */
    void setPointCloudIndexRegisteredProtected(CT_PCIR pcir);

    /**
      * \brief A redfinir si vous voulez supprimer quelquechose lors de l'appel  la mthode clearFromMemory
      */
    virtual void clearFromMemoryProtected() {}

private:
    CT_PCIR                     _pcir;
    CT_AbstractPointCloudIndex  *m_apci;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawable>(*this);

        ar & _pointCloud & _pointCloudIndex & _autoDeletePointCloud & _autoDeletePointCloudIndex &_autoUpdateBoundingShape & _boundingShape;
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(CT_AbstractItemDrawableWithPointCloud)
BOOST_CLASS_EXPORT_KEY(CT_AbstractItemDrawableWithPointCloud)
#else
};
#endif

#endif // CT_ABSTRACTITEMDRAWABLEWITHPOINTCLOUD_H
