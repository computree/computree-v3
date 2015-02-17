#ifndef CT_POINTACCESSOR_H
#define CT_POINTACCESSOR_H

#include "ct_defines.h"
#include "ct_point.h"

/**
 * @brief Use this class to access point in the global point cloud
 */
class PLUGINSHAREDSHARED_EXPORT CT_PointAccessor
{
public:
    CT_PointAccessor();

    /**
     * @brief Return the point at 'globalIndex'
     */
    CT_Point pointAt(const size_t &globalIndex) const;

    /**
     * @brief Return a const reference to the point at 'globalIndex'
     */
    const CT_Point& constPointAt(const size_t &globalIndex) const;

    /**
     * @brief Replace the point at 'globalIndex' by a new point with the coordinate system passed in parameter
     */
    void replacePointAt(const size_t &globalIndex, const CT_Point &p, CT_AbstractCoordinateSystem *sys);

    /**
     * @brief Return a reference to the point at 'globalIndex' (internal point used in cloud)
     */
    CT_PointData& internalPointAt(const size_t &globalIndex);

    /**
     * @brief Return a reference to the point at 'globalIndex' (internal point used in cloud)
     */
    const CT_PointData& constInternalPointAt(const size_t &globalIndex) const;

    /**
     * @brief Returns the size of the global point cloud
     */
    size_t size() const;

private:
    mutable CT_Point        m_p;
    CT_AbstractPointCloud   *m_pCloud;
};

#endif // CT_POINTACCESSOR_H
