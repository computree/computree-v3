#include "ct_abstractedgeattributes.h"

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes() : CT_AbstractAttributes()
{
    m_aci = NULL;
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(const CT_OutAbstractSingularItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_ECIR cir) : CT_AbstractAttributes(model, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : cir->abstractCloudIndexT());
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_ECIR cir) : CT_AbstractAttributes(modelName, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : cir->abstractCloudIndexT());
}

QString CT_AbstractEdgeAttributes::getType() const
{
    return staticGetType();
}

QString CT_AbstractEdgeAttributes::staticGetType()
{
    return CT_AbstractAttributes::staticGetType() + "/CT_AbstractEdgeAttributes";
}

const CT_AbstractEdgeCloudIndex* CT_AbstractEdgeAttributes::getEdgeCloudIndex() const
{
    return m_aci;
}

CT_ECIR CT_AbstractEdgeAttributes::getEdgeCloudIndexRegistered() const
{
    return m_cir;
}
