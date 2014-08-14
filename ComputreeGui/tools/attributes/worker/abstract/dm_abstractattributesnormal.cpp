#include "dm_abstractattributesnormal.h"

DM_AbstractAttributesNormal::DM_AbstractAttributesNormal() : DM_AbstractAttributes()
{
    m_syncWith = RepositoryInterface::SyncWithPointCloud;
}

void DM_AbstractAttributesNormal::setSyncWithCloud(RepositoryInterface::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;
}

RepositoryInterface::SyncCloudWith DM_AbstractAttributesNormal::syncWithCloud() const
{
    return m_syncWith;
}
