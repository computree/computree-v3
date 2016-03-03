#include "ct_abstractpointattributesscalar.h"

CT_AbstractPointAttributesScalar::CT_AbstractPointAttributesScalar() : CT_AbstractPointsAttributes()
{
}

CT_AbstractPointAttributesScalar::CT_AbstractPointAttributesScalar(const CT_OutAbstractSingularItemModel *model,
                                                                   const CT_AbstractResult *result,
                                                                   CT_PCIR pcir) : CT_AbstractPointsAttributes(model,
                                                                                                                                                                           result,
                                                                                                                                                                           pcir)
{

}

CT_AbstractPointAttributesScalar::CT_AbstractPointAttributesScalar(const QString &modelName,
                                                                   const CT_AbstractResult *result,
                                                                   CT_PCIR pcir) : CT_AbstractPointsAttributes(modelName,
                                                                                                                                                                           result,
                                                                                                                                                                           pcir)
{

}
