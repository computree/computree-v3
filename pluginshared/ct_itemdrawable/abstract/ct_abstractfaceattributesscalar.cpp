#include "ct_abstractfaceattributesscalar.h"

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar() : CT_AbstractFaceAttributes()
{
}

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar(const CT_OutAbstractItemModel *model,
                                                                 CT_AbstractResult *result,
                                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir) : CT_AbstractFaceAttributes(model,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{

}

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar(const QString &modelName,
                                                                 CT_AbstractResult *result,
                                                                 CT_AbstractCloudIndexRegistrationManagerT<CT_Face>::CT_AbstractCIR cir) : CT_AbstractFaceAttributes(modelName,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{

}

QString CT_AbstractFaceAttributesScalar::getType() const
{
    return staticGetType();
}

QString CT_AbstractFaceAttributesScalar::staticGetType()
{
    return CT_AbstractFaceAttributes::staticGetType() + "/CT_AbstractFaceAttributesScalar";
}

