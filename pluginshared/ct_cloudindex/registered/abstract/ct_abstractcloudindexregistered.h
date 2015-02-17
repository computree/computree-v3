#ifndef CT_ABSTRACTCLOUDINDEXREGISTERED_H
#define CT_ABSTRACTCLOUDINDEXREGISTERED_H

#include "pluginShared_global.h"

class CT_AbstractCloudIndex;

/**
 * A cloud of index registered
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractCloudIndexRegistered
{
public:
    CT_AbstractCloudIndexRegistered();
    virtual ~CT_AbstractCloudIndexRegistered();

    /**
     * @brief Return the cloud index
     */
    virtual CT_AbstractCloudIndex* abstractCloudIndex() const = 0;
};

#endif // CT_ABSTRACTCLOUDINDEXREGISTERED_H
