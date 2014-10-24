#ifndef CT_STDCLOUDREGISTEREDT_H
#define CT_STDCLOUDREGISTEREDT_H

#include "ct_cloud/registered/abstract/ct_abstractcloudregistered.h"

/**
 * A cloud registered of T. It's size and indices is sync with a global cloud.
 */
template<typename T >
class CT_StdCloudRegisteredT : public CT_AbstractCloudRegistered
{
public:
    ~CT_StdCloudRegisteredT();

    /**
     * @brief Returns the abstract cloud
     */
    CT_AbstractCloud* abstractCloud() const;

protected:

    friend class CT_CloudSyncToGlobalCloudManager;

    CT_StdCloudRegisteredT(CT_AbstractCloud *cloud);

private:
    CT_AbstractCloud                   *m_cloud;
};

#include "ct_cloud/registered/ct_stdcloudregisteredt.hpp"

#endif // CT_STDCLOUDREGISTEREDT_H
