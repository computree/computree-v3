/*
 * ct_parallelshootingpattern.h
 *
 *  Created on: Mar 3, 2017
 *      Author: Francis Giraldeau <francis.giraldeau@gmail.com>
 */

#ifndef CT_PARALLELSHOOTINGPATTERNFROMPOINTCLOUD_H
#define CT_PARALLELSHOOTINGPATTERNFROMPOINTCLOUD_H

#include "ct_shootingpattern.h"
#include "ct_accessor/ct_pointaccessor.h"

#include "Eigen/Core"
#include "Eigen/Geometry"

/**
 * @brief Shooting pattern where all shots have the same direction and different
 * origins. One shot per point in the cloud is generated. The direction of the
 * ray is obtained by the projection of the point on the plane described by the
 * origin and normal.It simulates an ideal scanner located very far from the
 * scene, where the angles between the rays are negligible.
 *
 */
class CT_ParallelShootingPatternFromPointCloud: public CT_ShootingPattern {
public:
    /**
     * @brief Shooting Pattern constructor
     * @param origin: a point in the plane
     * @param direction: the normal to the plane
     * @param pcir: point cloud
     */
    CT_ParallelShootingPatternFromPointCloud(
            const Eigen::Vector3d &point,
            const Eigen::Vector3d &normal,
            CT_PCIR pcir);

    /**
     * @brief Copy constructor
     */
    CT_ParallelShootingPatternFromPointCloud(const CT_ParallelShootingPatternFromPointCloud& other);

    const Eigen::Vector3d& getCenterCoordinate() const { return m_point; }
    size_t getNumberOfShots() const;
    CT_Shot getShotAt(const size_t& index) const;
    CT_ShootingPattern* clone() const;

private:
    Eigen::Vector3d     m_point;
    Eigen::Vector3d     m_normal;
    CT_PCIR             m_pcir;
    CT_PointAccessor    m_pAccessor;
    Eigen::Hyperplane<double, 3> m_plane;
};

#endif /* CT_PARALLELSHOOTINGPATTERNFROMPOINTCLOUD_H */
