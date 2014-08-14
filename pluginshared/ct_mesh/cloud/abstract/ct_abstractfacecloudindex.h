#ifndef CT_ABSTRACTFACECLOUDINDEX_H
#define CT_ABSTRACTFACECLOUDINDEX_H

#include "pluginShared_global.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFaceCloudIndex : virtual public CT_AbstractCloudIndexT<CT_Face>
{
public:
    CT_AbstractFaceCloudIndex();
};

#endif // CT_ABSTRACTFACECLOUDINDEX_H
