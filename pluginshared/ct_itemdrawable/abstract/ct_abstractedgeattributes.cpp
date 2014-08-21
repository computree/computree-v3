#include "ct_abstractedgeattributes.h"

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes() : CT_AbstractAttributes()
{
    m_aci = NULL;
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(const CT_OutAbstractSingularItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR cir) : CT_AbstractAttributes(model, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : dynamic_cast<CT_AbstractEdgeCloudIndex*>(cir->abstractCloudIndexT()));
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR cir) : CT_AbstractAttributes(modelName, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : dynamic_cast<CT_AbstractEdgeCloudIndex*>(cir->abstractCloudIndexT()));
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

const CT_AbstractCloudIndexT<CT_Edge>* CT_AbstractEdgeAttributes::getEdgeCloudIndexT() const
{
    if(m_cir.data() == NULL)
        return NULL;

    return m_cir->abstractCloudIndexT();
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Edge>::CT_AbstractCIR CT_AbstractEdgeAttributes::getEdgeCloudIndexRegistered() const
{
    return m_cir;
}
