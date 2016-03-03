#include "ct_abstractfaceattributes.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes() : CT_AbstractAttributes()
{
    m_aci = NULL;
}

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes(const CT_OutAbstractSingularItemModel *model,
                                                     const CT_AbstractResult *result,
                                                     CT_FCIR cir) : CT_AbstractAttributes(model, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : cir->abstractCloudIndexT());
}

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes(const QString &modelName,
                                                     const CT_AbstractResult *result,
                                                     CT_FCIR cir) : CT_AbstractAttributes(modelName, result)
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : cir->abstractCloudIndexT());
}

const CT_AbstractFaceCloudIndex* CT_AbstractFaceAttributes::getFaceCloudIndex() const
{
    return m_aci;
}

CT_FCIR CT_AbstractFaceAttributes::getFaceCloudIndexRegistered() const
{
    return m_cir;
}
