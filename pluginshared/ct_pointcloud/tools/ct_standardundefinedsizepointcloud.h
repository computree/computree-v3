#ifndef CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H
#define CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H

#include "ct_defines.h"
#include "ct_pointcloud/tools/abstract/ct_abstractundefinedsizepointcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

class CT_CoordinateSystemManager;

/**
 * Use this class when you want to create a cloud of points if you
 * don't know it's size by advance.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardUndefinedSizePointCloud : public CT_AbstractUndefinedSizePointCloud
{
public:

    /**
     * @brief Returns the global index of the first point in this cloud
     */
    size_t beginIndex() const;

    /**
     * @brief Returns the global index of the last point in this cloud
     */
    size_t lastIndex() const;

    /**
     * @brief Returns the current size of this cloud
     */
    size_t size() const;

    /**
     * @brief Add a point to this cloud.
     */
    void addPoint(const CT_Point &p);

    // INTERNAL POINT

    /**
     * @brief Add a point to this cloud. The coordinate system used will be the default => index 0.
     * @warning Not intended for direct use by plugin developper
     */
    void addInternalPoint(const CT_PointData &point);

    /**
     * @brief Add a point to this cloud. You must pass the index of the coordinate system that will be set for this point
     * @warning Be sure to pass a valid coordinate system index
     * @warning Not intended for direct use by plugin developper
     */
    void addInternalPoint(const CT_PointData &point, const GLuint &csIndex);

private:

    size_t                                  m_bIndex;
    CT_PointCloudStdVector                  *m_pc;
    std::vector<GLuint>                     m_csIndexes;
    CT_CoordinateSystemManager              *m_csm;

protected:
    friend class CT_GlobalPointCloudManager;

    CT_StandardUndefinedSizePointCloud(const size_t &beginIndex, const CT_PointCloudStdVector *cloud);

    /**
     * @brief Returns global indexes of coordinate system of points added
     */
    const std::vector<GLuint>& coordinateSystemIndexOfPointsAdded() const;
};

#endif // CT_STANDARDUNDEFINEDSIZEPOINTCLOUD_H
