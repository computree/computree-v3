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
    CT_ShootingPattern(const Eigen::Vector3d& origin) : m_origin(origin) {}
    virtual ~CT_ShootingPattern() {}

    /**
     * @brief Returns the origin of all shot
     */
    inline const Eigen::Vector3d& getOrigin() const { return m_origin; }

    /**
     * @brief Set the origin of all shot
     */
    inline void setOrigin (const Eigen::Vector3d& origin) { m_origin = origin; }

    /**
     * @brief Returns the number of shots
     */
    virtual size_t getNumberOfShots() const = 0;

    /**
     * @brief Returns the shot direction at index specified
     */
    Eigen::Vector3d getShotDirectionAt(const size_t& index) const {
        Eigen::Vector3d direction;

        getShotDirectionAt(index, direction);

        return direction;
    }

    /**
     * @brief Returns the shot direction at index specified
     */
    virtual void getShotDirectionAt(const size_t& index, Eigen::Vector3d& direction) const = 0;

    /**
     * @brief Returns the shot at index specified
     */
    CT_Shot getShotAt(const size_t& index) const {
        CT_Shot shot;

        getShotAt(index, shot);

        return shot;
    }

    /**
     * @brief Returns the shot at index specified
     */
    void getShotAt(const size_t& index, CT_Shot& shot) const {
        Eigen::Vector3d direction;
        getShotDirectionAt(index, direction);

        shot.setOrigin(getOrigin());
        shot.setDirection(direction);
    }

    /**
     * @brief Copy this pattern
     */
    virtual CT_ShootingPattern* clone() const = 0;

private:
    Eigen::Vector3d m_origin;     /*!< origin of all shot */
};


#endif // CT_SHOOTINGPATTERN_H
