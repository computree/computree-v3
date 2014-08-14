#include "ct_abstractpointsattributes.h"

#include "ct_global/ct_context.h"

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

CT_StandardAbstractPointsAttributesDrawManager CT_AbstractPointsAttributes::APA_DRAW_MANAGER;

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes() : CT_AbstractAttributes()
{
    m_apci = NULL;
    setBaseDrawManager(&APA_DRAW_MANAGER);
}

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes(const CT_OutAbstractItemModel *model,
                                                         const CT_AbstractResult *result,
                                                         CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir) : CT_AbstractAttributes(model,
                                                                                                                                                           result)
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == NULL ? NULL : dynamic_cast<CT_AbstractPointCloudIndex*>(m_pcir->abstractCloudIndexT()));

    setBaseDrawManager(&APA_DRAW_MANAGER);
}

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes(const QString &modelName,
                                                         const CT_AbstractResult *result,
                                                         CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR pcir) : CT_AbstractAttributes(modelName,
                                                                                                                                                           result)
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == NULL ? NULL : dynamic_cast<CT_AbstractPointCloudIndex*>(m_pcir->abstractCloudIndexT()));

    setBaseDrawManager(&APA_DRAW_MANAGER);
}

QString CT_AbstractPointsAttributes::getType() const
{
    return staticGetType();
}

QString CT_AbstractPointsAttributes::staticGetType()
{
    return CT_AbstractAttributes::staticGetType() + "/CT_AbstractPointsAttributes";
}

const CT_AbstractPointCloud* CT_AbstractPointsAttributes::getPointCloud() const
{
    return PS_REPOSITORY->globalPointCloud();
}

const CT_AbstractPointCloudIndex* CT_AbstractPointsAttributes::getPointCloudIndex() const
{
    return m_apci;
}

const CT_AbstractCloudIndexT<CT_Point> *CT_AbstractPointsAttributes::getPointCloudIndexT() const
{
    if(m_pcir.data() == NULL)
        return NULL;

    return m_pcir->abstractCloudIndexT();
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Point>::CT_AbstractCIR CT_AbstractPointsAttributes::getPointCloudIndexRegistered() const
{
    return m_pcir;
}

int CT_AbstractPointsAttributes::getFastestIncrement() const
{
    return 16;
}
