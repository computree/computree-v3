#ifndef CT_IACCESSEDGECLOUD_H
#define CT_IACCESSEDGECLOUD_H

#include "pluginShared_global.h"

class CT_AbstractEdgeCloudIndex;

class PLUGINSHAREDSHARED_EXPORT CT_IAccessEdgeCloud
{
public:
    virtual ~CT_IAccessEdgeCloud() {}

    /**
     * @brief Returns the edge cloud index
     */
    virtual const CT_AbstractEdgeCloudIndex* getEdgeCloudIndex() const = 0;
};

#endif // CT_IACCESSEDGECLOUD_H
