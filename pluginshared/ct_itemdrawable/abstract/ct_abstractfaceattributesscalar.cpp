#include "ct_abstractfaceattributesscalar.h"

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar() : CT_AbstractFaceAttributes()
{
}

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                                                 CT_AbstractResult *result,
                                                                 CT_FCIR cir) : CT_AbstractFaceAttributes(model,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{

}

CT_AbstractFaceAttributesScalar::CT_AbstractFaceAttributesScalar(const QString &modelName,
                                                                 CT_AbstractResult *result,
                                                                 CT_FCIR cir) : CT_AbstractFaceAttributes(modelName,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{

}
