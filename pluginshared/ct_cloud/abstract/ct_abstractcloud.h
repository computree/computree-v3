#ifndef CT_ABSTRACTCLOUD_H
#define CT_ABSTRACTCLOUD_H

#include "pluginShared_global.h"

/**
 * @brief A cloud
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractCloud
{
public:
    CT_AbstractCloud();
    virtual ~CT_AbstractCloud() {}

    virtual size_t size() const = 0;

    virtual CT_AbstractCloud* copy() const = 0;

protected:

    friend class CT_AbstractCloudSyncToGlobalCloudManager;

    /**
     * @brief delete all elements of the cloud between beginIndex and (beginIndex+size-1) included.
     */
    virtual void erase(const size_t &beginIndex, const size_t &size) = 0;

    /**
     * @brief resize the collection of colors
     */
    virtual void resize(const size_t &newSize) = 0;
};

#endif // CT_ABSTRACTCLOUD_H
