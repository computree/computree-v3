/**
 * @author Michael Krebs (AMVALOR)
 * @date 25.01.2017
 * @version 1
 */
#ifndef CT_SHOT_H
#define CT_SHOT_H

#include "Eigen/Dense"

#include "pluginShared_global.h"

/**
 * @brief Represent a shot
 */
struct PLUGINSHAREDSHARED_EXPORT CT_Shot {
public:
    /**
     * @brief Construct a default shot
     */
    CT_Shot(bool valid = true)
    {
        if(valid)
            m_direction = Eigen::Vector3d(1,0,0);
    }

    /**
     * @brief Construct a shot with an origin and a direction
     */
    CT_Shot(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction) : m_origin(origin), m_direction(direction) {}

    /**
     * @brief Set the origin of the shot
     */
    inline void setOrigin(const Eigen::Vector3d& origin) { m_origin = origin; }

    /**
     * @brief Returns the origin of the shot
     */
    inline const Eigen::Vector3d& getOrigin() const { return m_origin; }

    /**
     * @brief Set the direction of the shot
     */
    inline void setDirection(const Eigen::Vector3d& direction) { m_direction = direction; }

    /**
     * @brief Returns the direction of the shot
     */
    inline const Eigen::Vector3d& getDirection() const { return m_direction; }

private:
    Eigen::Vector3d     m_direction;
    Eigen::Vector3d     m_origin;
};

#endif // CT_SHOT_H
