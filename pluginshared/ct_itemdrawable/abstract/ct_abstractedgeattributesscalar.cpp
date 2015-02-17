#include "ct_abstractedgeattributesscalar.h"

CT_AbstractEdgeAttributesScalar::CT_AbstractEdgeAttributesScalar() : CT_AbstractEdgeAttributes()
{
}

CT_AbstractEdgeAttributesScalar::CT_AbstractEdgeAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                                                 CT_AbstractResult *result,
                                                                 CT_ECIR cir) : CT_AbstractEdgeAttributes(model,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{
}

CT_AbstractEdgeAttributesScalar::CT_AbstractEdgeAttributesScalar(const QString &modelName,
                                                                 CT_AbstractResult *result,
                                                                 CT_ECIR cir) : CT_AbstractEdgeAttributes(modelName,
                                                                                                                                                                     result,
                                                                                                                                                                     cir)
{
}

QString CT_AbstractEdgeAttributesScalar::getType() const
{
    return staticGetType();
}

QString CT_AbstractEdgeAttributesScalar::staticGetType()
{
    return CT_AbstractEdgeAttributes::staticGetType() + "/CT_AbstractEdgeAttributesScalar";
}
