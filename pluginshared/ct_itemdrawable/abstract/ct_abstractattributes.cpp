#include "ct_abstractattributes.h"

CT_AbstractAttributes::CT_AbstractAttributes() : CT_AbstractSingularItemDrawable()
{
}

CT_AbstractAttributes::CT_AbstractAttributes(const CT_OutAbstractSingularItemModel *model,
                                             const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model , result)
{
}

CT_AbstractAttributes::CT_AbstractAttributes(const QString &modelName,
                                             const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName , result)
{
}
