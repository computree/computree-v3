#ifndef DM_ABSTRACTATTRIBUTESNORMAL_H
#define DM_ABSTRACTATTRIBUTESNORMAL_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"
#include "interfaces.h"

class DM_AbstractAttributesNormal : public DM_AbstractAttributes
{
public:
    DM_AbstractAttributesNormal();

    void setSyncWithCloud(RepositoryInterface::SyncCloudWith syncWith);
    RepositoryInterface::SyncCloudWith syncWithCloud() const;

    virtual void checkAndSetNormalCloudToDoc() = 0;

private:
    RepositoryInterface::SyncCloudWith m_syncWith;
};

#endif // DM_ABSTRACTATTRIBUTESNORMAL_H
