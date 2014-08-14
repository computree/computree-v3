#include "ct_standardnormalcloudregistered.h"

CT_StandardNormalCloudRegistered::~CT_StandardNormalCloudRegistered()
{
    delete m_normalCloud;
}

CT_AbstractNormalCloud *CT_StandardNormalCloudRegistered::abstractNormalCloud() const
{
    return m_normalCloud;
}

CT_AbstractCloud *CT_StandardNormalCloudRegistered::abstractCloud() const
{
    return m_normalCloud;
}

CT_StandardNormalCloudRegistered::CT_StandardNormalCloudRegistered(CT_AbstractNormalCloud *normalCloud) : CT_AbstractCloudRegistered()
{
    m_normalCloud = normalCloud;
}
