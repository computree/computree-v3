#ifndef CT_STANDARDCOLORCLOUDREGISTERED_H
#define CT_STANDARDCOLORCLOUDREGISTERED_H

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_cloud/registered/abstract/ct_abstractcloudregistered.h"

/**
 * A color cloud registered. It's size and indices is sync with a global cloud.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardColorCloudRegistered : public CT_AbstractCloudRegistered
{
public:
    ~CT_StandardColorCloudRegistered();

    CT_AbstractCloud* abstractCloud() const;
    CT_AbstractColorCloud* abstractColorCloud() const;

protected:

    template <typename T, class CRI> friend class CT_CloudSyncToGlobalCloudManagerT;

    CT_StandardColorCloudRegistered(CT_AbstractColorCloud *colorCloud);

private:

    CT_AbstractColorCloud                   *m_colorCloud;
};

#endif // CT_STANDARDCOLORCLOUDREGISTERED_H
