#ifndef DM_ABSTRACTATTRIBUTESCOLOR_H
#define DM_ABSTRACTATTRIBUTESCOLOR_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"
#include "ct_global/ct_repository.h"

class DM_AbstractAttributesColor : public DM_AbstractAttributes
{
public:
    DM_AbstractAttributesColor();

    void setSyncWithCloud(CT_Repository::SyncCloudWith syncWith);
    CT_Repository::SyncCloudWith syncWithCloud() const;

    virtual void checkAndSetColorCloudToDoc() = 0;

private:
    CT_Repository::SyncCloudWith m_syncWith;
};

#endif // DM_ABSTRACTATTRIBUTESCOLOR_H
