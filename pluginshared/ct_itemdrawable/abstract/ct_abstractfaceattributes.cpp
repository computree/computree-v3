#include "ct_abstractfaceattributes.h"

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes() : CT_AbstractAttributes()
{
    m_aci = NULL;
}

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes(const CT_OutAbstractItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir) : CT_AbstractAttributes(model, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : dynamic_cast<CT_AbstractFaceCloudIndex*>(cir->abstractCloudIndexT()));
}

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir) : CT_AbstractAttributes(modelName, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : dynamic_cast<CT_AbstractFaceCloudIndex*>(cir->abstractCloudIndexT()));
}

QString CT_AbstractFaceAttributes::getType() const
{
    return staticGetType();
}

QString CT_AbstractFaceAttributes::staticGetType()
{
    return CT_AbstractAttributes::staticGetType() + "/CT_AbstractFaceAttributes";
}

const CT_AbstractFaceCloudIndex* CT_AbstractFaceAttributes::getFaceCloudIndex() const
{
    return m_aci;
}

const CT_AbstractCloudIndexT<CT_Face>* CT_AbstractFaceAttributes::getFaceCloudIndexT() const
{
    if(m_cir.data() == NULL)
        return NULL;

    return m_cir->abstractCloudIndexT();
}

CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR CT_AbstractFaceAttributes::getFaceCloudIndexRegistered() const
{
    return m_cir;
}
