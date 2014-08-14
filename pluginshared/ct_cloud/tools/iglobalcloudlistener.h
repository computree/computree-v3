#ifndef IGLOBALCLOUDLISTENER_H
#define IGLOBALCLOUDLISTENER_H

#include <stdlib.h>

class IGlobalCloudListener
{
public:
    virtual ~IGlobalCloudListener() {}

    /**
     * @brief Called when the global cloud is imputed of elements between beginIndex and endIndex (endIndex = beginIndex+size).
     */
    virtual void cloudDeleted(const size_t &beginIndex, const size_t &size) = 0;

    /**
     * @brief Called when the size of the global cloud has increased
     */
    virtual void cloudAdded(const size_t &size) = 0;
};

#endif // IGLOBALCLOUDLISTENER_H
