#include "ct_standardcolorcloudregistered.h"

CT_StandardColorCloudRegistered::~CT_StandardColorCloudRegistered()
{
    delete m_colorCloud;
}

CT_AbstractCloud* CT_StandardColorCloudRegistered::abstractCloud() const
{
    return m_colorCloud;
}

CT_AbstractColorCloud* CT_StandardColorCloudRegistered::abstractColorCloud() const
{
    return m_colorCloud;
}

CT_StandardColorCloudRegistered::CT_StandardColorCloudRegistered(CT_AbstractColorCloud *colorCloud) : CT_AbstractCloudRegistered()
{
    m_colorCloud = colorCloud;
}


