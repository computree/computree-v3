#include "dm_abstractattributesnormal.h"

DM_AbstractAttributesNormal::DM_AbstractAttributesNormal() : DM_AbstractAttributes()
{
    m_syncWith = CT_Repository::SyncWithPointCloud;
}

void DM_AbstractAttributesNormal::setSyncWithCloud(CT_Repository::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;
}

CT_Repository::SyncCloudWith DM_AbstractAttributesNormal::syncWithCloud() const
{
    return m_syncWith;
}
