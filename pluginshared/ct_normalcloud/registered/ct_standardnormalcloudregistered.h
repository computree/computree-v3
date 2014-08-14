#ifndef CT_STANDARDNORMALCLOUDREGISTERED_H
#define CT_STANDARDNORMALCLOUDREGISTERED_H

#include "ct_cloud/registered/abstract/ct_abstractcloudregistered.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"

/**
 * A cloud of normals registered. It's size and indices is sync with a global cloud.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardNormalCloudRegistered : public CT_AbstractCloudRegistered
{
public:
    ~CT_StandardNormalCloudRegistered();

    /**
     * @brief Returns the abstract cloud
     */
    CT_AbstractCloud* abstractCloud() const;

    /**
     * @brief Returns the abstract cloud of normal
     */
    CT_AbstractNormalCloud* abstractNormalCloud() const;

protected:

    template <typename T, class CRI> friend class CT_CloudSyncToGlobalCloudManagerT;

    CT_StandardNormalCloudRegistered(CT_AbstractNormalCloud *normalCloud);

private:
    CT_AbstractNormalCloud   *m_normalCloud;
};

#endif // CT_STANDARDNORMALCLOUDREGISTERED_H
