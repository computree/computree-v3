#include "ct_standardnormalcloudregistered.h"

CT_AbstractNormalCloud* CT_StandardNormalCloudRegistered::abstractNormalCloud() const
{
    return cloudT();
}

CT_StandardNormalCloudRegistered::CT_StandardNormalCloudRegistered(CT_AbstractNormalCloud *normalCloud) : CT_StdCloudRegisteredT<CT_AbstractNormalCloud>(normalCloud)
{
}
