/**
 * @author Michael Krebs (AMVALOR)
 * @date 25.01.2017
 * @version 1
 */
#ifndef CT_SHOOTINGPATTERN_H
#define CT_SHOOTINGPATTERN_H

#include "ct_shot.h"

/**
 * @brief This class is abstract. It offers the possibility to define a specific
 *        pattern of shots for a scanner for example
 */
class PLUGINSHAREDSHARED_EXPORT CT_ShootingPattern
{
public:
    CT_ShootingPattern() {}

    virtual ~CT_ShootingPattern() {}

    /**
     * @brief Returns the center coordinate of this shooting pattern
     * @return Cartesian center coordinates
     */
    virtual const Eigen::Vector3d& getCenterCoordinate() const = 0;
    /**
     * @brief Returns the number of shots
     */
    virtual size_t getNumberOfShots() const = 0;

    /**
     * @brief Returns the shot at index specified
     */
    virtual CT_Shot getShotAt(const size_t& index) const = 0;

    /**
     * @brief Copy this pattern
     */
    virtual CT_ShootingPattern* clone() const = 0;

};


#endif // CT_SHOOTINGPATTERN_H
