#ifndef CT_STANDARDNORMALCLOUDREGISTERED_H
#define CT_STANDARDNORMALCLOUDREGISTERED_H

#include "ct_cloud/registered/ct_stdcloudregisteredt.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"

/**
 * A cloud of normals registered. It's size and indices is sync with a global cloud.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardNormalCloudRegistered : public CT_StdCloudRegisteredT<CT_AbstractNormalCloud>
{
public:
    /**
     * @brief Returns the abstract cloud of normal
     */
    CT_AbstractNormalCloud* abstractNormalCloud() const;

protected:

    friend class CT_CloudSyncToGlobalCloudManager;

    CT_StandardNormalCloudRegistered(CT_AbstractNormalCloud *normalCloud);
};

#endif // CT_STANDARDNORMALCLOUDREGISTERED_H
