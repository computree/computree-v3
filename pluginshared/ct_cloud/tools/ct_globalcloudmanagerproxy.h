#ifndef CT_GLOBALCLOUDMANAGERPROXY_H
#define CT_GLOBALCLOUDMANAGERPROXY_H

#include "ct_defines.h"
#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanager.h"

/**
 * The CT_Repository::globalCloudManager() function is declare protected.
 * Make it accessible in a plugin by uing sthis proxy class, which is friend.
 */

class CT_GlobalCloudManagerProxy
{
public:
    CT_GlobalCloudManagerProxy();
    CT_AbstractGlobalCloudManager *getGlobalPointCloudManager();
    CT_AbstractPointCloud *getGlobalPointCloud();
};

#endif // CT_GLOBALCLOUDMANAGERPROXY_H
