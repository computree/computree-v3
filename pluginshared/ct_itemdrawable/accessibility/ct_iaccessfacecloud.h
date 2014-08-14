#ifndef CT_IACCESSFACECLOUD_H
#define CT_IACCESSFACECLOUD_H

#include "pluginShared_global.h"

class CT_AbstractFaceCloudIndex;

class PLUGINSHAREDSHARED_EXPORT CT_IAccessFaceCloud
{
public:
    virtual ~CT_IAccessFaceCloud() {}

    /**
     * @brief Returns the face cloud index
     */
    virtual const CT_AbstractFaceCloudIndex* getFaceCloudIndex() const = 0;
};

#endif // CT_IACCESSFACECLOUD_H
