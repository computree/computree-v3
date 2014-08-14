#ifndef CT_ABSTRACTMODIFIABLECLOUDINDEX_H
#define CT_ABSTRACTMODIFIABLECLOUDINDEX_H

#include "ct_cloudindex/abstract/ct_abstractcloudindex.h"

/**
 * @brief Represent a cloud index that can be modifiable
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractModifiableCloudIndex : virtual public CT_AbstractCloudIndex
{
public:
    virtual ~CT_AbstractModifiableCloudIndex() {}

    /**
     * @brief Add a new index in the cloud
     * @param newIndex : index to add
     */
    virtual void addIndex(const size_t &newIndex) = 0;

    /**
     * @brief Remove the index from the cloud
     * @param index : index to remove
     */
    virtual void removeIndex(const size_t &index) = 0;

    /**
     * @brief Removes all indexes in the cloud index
     */
    virtual void clear() = 0;
};

#endif // CT_ABSTRACTMODIFIABLECLOUDINDEX_H
