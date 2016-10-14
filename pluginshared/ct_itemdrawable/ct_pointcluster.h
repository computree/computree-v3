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

#ifndef CT_POINTCLUSTER_H
#define CT_POINTCLUSTER_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardpointclusterdrawmanager.h"
#include "ct_point.h"

#include <QMapIterator>

template<typename T> class CT_CloudIndexStdVectorT;

class PLUGINSHAREDSHARED_EXPORT CT_PointClusterBarycenter
{
public:

    CT_PointClusterBarycenter()
    {
        reset();
    }

    void reset()
    {
        _backupSum[0] = 0;
        _backupSum[1] = 0;
        _backupSum[2] = 0;

        _barycenter[0] = 0;
        _barycenter[1] = 0;
        _barycenter[2] = 0;

        _size = 0;

        _notComputed = false;
    }

    void addPoint(double x, double y, double z)
    {
        _backupSum[0] += x;
        _backupSum[1] += y;
        _backupSum[2] += z;

        ++_size;

        _notComputed = true;
    }

    void addPoint(const CT_Point &p)
    {
        _backupSum[0] += p(0);
        _backupSum[1] += p(1);
        _backupSum[2] += p(2);

        ++_size;

        _notComputed = true;
    }

    void compute() const
    {
        double *notConstBarycenter = const_cast<double*>(_barycenter);
        bool *notConstNotComputed = const_cast<bool*>(&_notComputed);

        if(_size != 0) {
            notConstBarycenter[0] = _backupSum[0]/_size;
            notConstBarycenter[1] = _backupSum[1]/_size;
            notConstBarycenter[2] = _backupSum[2]/_size;
        }

        *notConstNotComputed = false;        
    }

    inline double x() const { if(_notComputed){ compute(); } return _barycenter[0]; }
    inline double y() const { if(_notComputed){ compute(); } return _barycenter[1]; }
    inline double z() const { if(_notComputed){ compute(); } return _barycenter[2]; }

private:

    int     _size;
    double   _backupSum[3];
    double   _barycenter[3];

    bool    _notComputed;
};

/*!
 * \class CT_PointCluster
 * \ingroup PluginShared_Items
 * \brief <b>ItemDrawable representing a group of points</b>
 *
 * It contains an automatically updated barycenter.
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_PointCluster : public CT_AbstractItemDrawableWithPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointCluster, CT_AbstractItemDrawableWithPointCloud)

public:

    CT_PointCluster();

    CT_PointCluster(const CT_OutAbstractSingularItemModel *model,
                    const CT_AbstractResult *result);

    CT_PointCluster(const QString &modelName,
                    const CT_AbstractResult *result);

    virtual ~CT_PointCluster();

    virtual QString name() const;


    /**
      * \brief Ajoute un point au groupe  partir de son index dans le nuage de point (PointCloud)
      *
      *  - Si updateBoundingShapeAutomatically() renvoie true alors la forme englobante et la AxisAlignedBoundingBox est mise  jour.
      *  - Si le cluster n'a pas de nuage d'index un nuage est cr automatiquement
      *
      * \param verifyIfExist : si ce paramtre est  true alors l'index n'est ajout que si il n'existe pas dj dans le groupe de points.
      *
      * \return true si le point a t ajout
      */
    virtual bool addPoint(size_t index, bool verifyIfExist = false, bool firstPosition = false);

    /**
      * \brief Retourne le barycentre du groupe de points
      */
    const CT_PointClusterBarycenter& getBarycenter() const;

    /**
      * \brief Retourne un groupe de points rsultat de la fusion entre le groupe pCLuster1 et pCLuster2.
      *
      * \param verifyDuplicated : a true si vous voulez que lors de la fusion il y est une vrification de doublon afin de ne
      *                           pas ajouter deux fois le mme point (si il existe dans le premier et le deuxime groupe).
      *
      * ATTENTION les deux groupes doivent avoir le mme nuage de points en rfrence ! sinon la mthode renvoie NULL.
      */
    static CT_PointCluster* merge(CT_PointCluster &pCLuster1, CT_PointCluster &pCLuster2, const CT_OutAbstractSingularItemModel *model, quint64 id, CT_AbstractResult &result, bool verifyDuplicated = false);

    static CT_PointCluster* merge(CT_PointCluster &pCLuster1, CT_PointCluster &pCLuster2, const QString &modelName, quint64 id, CT_AbstractResult &result, bool verifyDuplicated = false);

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model,
                                          const CT_AbstractResult *result,
                                          CT_ResultCopyModeList copyModeList);

private:

    CT_PointClusterBarycenter                           _barycenter;
    const static CT_StandardPointClusterDrawManager     POINTCLUSTER_DRAW_MANAGER;
    CT_PointCloudIndexVector                            *m_pIndex;

protected:

    void initBarycenter();


#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithPointCloud>(*this);

        ar & _creationType;

        int size = _affiliationList.size();

        ar & size;

        QMapIterator<CT_PointCluster*, CT_PointCluster::AffiliationType> it(_affiliationList);

        while(it.hasNext())
        {
            it.next();

            ar & it.key() & it.value();
        }
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithPointCloud>(*this);

        ar & _creationType;

        int size;

        ar & size;

        for(int i=0; i<size; ++i)
        {
            CT_PointCluster::AffiliationType type;
            CT_PointCluster *pCluster;

            ar & type & pCluster;

            _affiliationList.insert(pCluster, type);
        }

        initBarycenter();
    }
};

BOOST_CLASS_EXPORT_KEY(CT_PointCluster)
#else
};
#endif

#endif // CT_POINTCLUSTER_H
