#include "dm_abstractattributescolor.h"

DM_AbstractAttributesColor::DM_AbstractAttributesColor() : DM_AbstractAttributes()
{
    m_syncWith = RepositoryInterface::SyncWithPointCloud;
}

void DM_AbstractAttributesColor::setSyncWithCloud(RepositoryInterface::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;
}

RepositoryInterface::SyncCloudWith DM_AbstractAttributesColor::syncWithCloud() const
{
    return m_syncWith;
}
