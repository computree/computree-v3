#include "ct_abstractpointsattributes.h"

#include "ct_global/ct_context.h"

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

CT_StandardAbstractPointsAttributesDrawManager CT_AbstractPointsAttributes::APA_DRAW_MANAGER;

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes() : CT_AbstractAttributes()
{
    m_apci = NULL;
    setBaseDrawManager(&APA_DRAW_MANAGER);
}

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes(const CT_OutAbstractSingularItemModel *model,
                                                         const CT_AbstractResult *result,
                                                         CT_PCIR pcir) : CT_AbstractAttributes(model,
                                                                                                                                                           result)
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == NULL ? NULL : m_pcir->abstractCloudIndexT());

    setBaseDrawManager(&APA_DRAW_MANAGER);
}

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes(const QString &modelName,
                                                         const CT_AbstractResult *result,
                                                         CT_PCIR pcir) : CT_AbstractAttributes(modelName,
                                                                                                                                                           result)
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == NULL ? NULL : m_pcir->abstractCloudIndexT());

    setBaseDrawManager(&APA_DRAW_MANAGER);
}

const CT_AbstractPointCloudIndex* CT_AbstractPointsAttributes::getPointCloudIndex() const
{
    return m_apci;
}

CT_PCIR CT_AbstractPointsAttributes::getPointCloudIndexRegistered() const
{
    return m_pcir;
}

int CT_AbstractPointsAttributes::getFastestIncrement() const
{
    return 16;
}

