#ifndef CT_IACCESSPOINTCLOUD_H
#define CT_IACCESSPOINTCLOUD_H

#include "pluginShared_global.h"

class CT_AbstractPointCloudIndex;

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
