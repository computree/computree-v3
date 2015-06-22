#ifndef CT_IACCESSPOINTCLOUD_H
#define CT_IACCESSPOINTCLOUD_H

#include "pluginShared_global.h"

#include "ct_defines.h"
#include "ct_point.h"

class PLUGINSHAREDSHARED_EXPORT CT_IAccessPointCloud
{
public:
    virtual ~CT_IAccessPointCloud() {}

    /**
     * @brief Returns the point cloud index
     */
    virtual const CT_AbstractPointCloudIndex* getPointCloudIndex() const = 0;

    #ifdef USE_PCL
    /**
     * @brief Convert the point cloud index to a pcl cloud.
     * @warning Please re-use the shared_ptr returned because
     *          a new collection is created at each call.
     */
    virtual boost::shared_ptr<pcl::PointCloud<CT_PointData> > getPCLCloud() const = 0;
    #endif
};

#endif // CT_IACCESSPOINTCLOUD_H
