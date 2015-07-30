#ifndef CT_STANDARDNORMALCLOUDREGISTERED_H
#define CT_STANDARDNORMALCLOUDREGISTERED_H

#include "ct_cloud/registered/ct_stdcloudregisteredt.h"
#include "ct_normalcloud/ct_normalcloudosg.h"

/**
 * A cloud of normals registered. It's size and indices is sync with a global cloud.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardNormalCloudRegistered : public CT_StdCloudRegisteredT<CT_NormalCloudOsg>
{
public:
    /**
     * @brief Returns the abstract cloud of normal
     */
    CT_AbstractNormalCloud* abstractNormalCloud() const;

protected:

    friend class CT_CloudSyncToGlobalCloudManager;

    CT_StandardNormalCloudRegistered(CT_NormalCloudOsg *normalCloud);
};

#endif // CT_STANDARDNORMALCLOUDREGISTERED_H
