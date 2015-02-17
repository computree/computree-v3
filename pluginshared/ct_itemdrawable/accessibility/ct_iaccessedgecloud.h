#ifndef CT_IACCESSEDGECLOUD_H
#define CT_IACCESSEDGECLOUD_H

#include "pluginShared_global.h"
#include "ct_defines.h"

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
