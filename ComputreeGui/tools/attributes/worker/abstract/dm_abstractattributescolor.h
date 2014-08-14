#ifndef DM_ABSTRACTATTRIBUTESCOLOR_H
#define DM_ABSTRACTATTRIBUTESCOLOR_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"
#include "interfaces.h"

class DM_AbstractAttributesColor : public DM_AbstractAttributes
{
public:
    DM_AbstractAttributesColor();

    void setSyncWithCloud(RepositoryInterface::SyncCloudWith syncWith);
    RepositoryInterface::SyncCloudWith syncWithCloud() const;

    virtual void checkAndSetColorCloudToDoc() = 0;

private:
    RepositoryInterface::SyncCloudWith m_syncWith;
};

#endif // DM_ABSTRACTATTRIBUTESCOLOR_H
