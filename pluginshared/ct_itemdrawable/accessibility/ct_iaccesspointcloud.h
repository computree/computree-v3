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
};

#endif // CT_IACCESSPOINTCLOUD_H
