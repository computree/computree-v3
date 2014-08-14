#ifndef CT_ABSTRACTEDGECLOUDINDEX_H
#define CT_ABSTRACTEDGECLOUDINDEX_H

#include "pluginShared_global.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractEdgeCloudIndex : virtual public CT_AbstractCloudIndexT<CT_Edge>
{
public:
    CT_AbstractEdgeCloudIndex();
};

#endif // CT_ABSTRACTEDGECLOUDINDEX_H
