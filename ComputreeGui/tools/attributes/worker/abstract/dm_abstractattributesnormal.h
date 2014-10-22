#ifndef DM_ABSTRACTATTRIBUTESNORMAL_H
#define DM_ABSTRACTATTRIBUTESNORMAL_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"
#include "ct_global/ct_repository.h"

class DM_AbstractAttributesNormal : public DM_AbstractAttributes
{
public:
    DM_AbstractAttributesNormal();

    void setSyncWithCloud(CT_Repository::SyncCloudWith syncWith);
    CT_Repository::SyncCloudWith syncWithCloud() const;

private:
    CT_Repository::SyncCloudWith m_syncWith;
};

#endif // DM_ABSTRACTATTRIBUTESNORMAL_H
