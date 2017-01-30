/**
 * @author Michael Krebs (AMVALOR)
 * @date 25.01.2017
 * @version 1
 */
#ifndef CT_SHOOTINGPATTERNFROMPOINTCLOUD_H
#define CT_SHOOTINGPATTERNFROMPOINTCLOUD_H

#include "ct_defines.h"
#include "ct_itemdrawable/tools/scanner/ct_shootingpattern.h"
#include "ct_accessor/ct_pointaccessor.h"

/**
 * @brief A pattern of shots can be shown like an origin of shot and a cloud of
 *        point of impact, [point of impact] - [origin] = [direction] !
 */
class PLUGINSHAREDSHARED_EXPORT CT_ShootingPatternFromPointCloud : public CT_ShootingPattern
{
public:
    /**
     * @brief Construct a pattern with an origin and a list of index of points
     * @param origin : origin of all shots
     * @param pcir : list of index of points that represent an impact or to deduce the direction
     */
    CT_ShootingPatternFromPointCloud(const Eigen::Vector3d& origin,
                                     CT_PCIR pcir);

    /**
     * @brief Construct it from existing object (copy it)
     */
    CT_ShootingPatternFromPointCloud(const CT_ShootingPatternFromPointCloud& other);

    /**
     * @brief Returns the number of shots
     */
    size_t getNumberOfShots() const;

    /**
     * @brief Returns the shot direction at index specified
     */
    void getShotDirectionAt(const size_t& index, Eigen::Vector3d& direction) const;

    /**
     * @brief Copy this pattern
     */
    CT_ShootingPattern* clone() const;

private:
    CT_PCIR             m_pcir;
    CT_PointAccessor    m_pAccessor;
};

#endif // CT_SHOOTINGPATTERNFROMPOINTCLOUD_H
