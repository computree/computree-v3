#include "ct_globalcloudmanagerproxy.h"
#include "ct_globalpointcloudmanager.h"
#include "ct_global/ct_context.h"

CT_GlobalCloudManagerProxy::CT_GlobalCloudManagerProxy()
{
}

CT_AbstractGlobalCloudManager *CT_GlobalCloudManagerProxy::getGlobalPointCloudManager()
{
    return PS_REPOSITORY->globalCloudManager<CT_PointData>();
}

CT_AbstractPointCloud *CT_GlobalCloudManagerProxy::getGlobalPointCloud()
{
    return PS_REPOSITORY->globalCloud<CT_PointData>();
}
