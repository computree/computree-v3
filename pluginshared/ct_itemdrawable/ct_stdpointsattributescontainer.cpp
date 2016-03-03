#include "ct_stdpointsattributescontainer.h"

CT_StdPointsAttributesContainer::CT_StdPointsAttributesContainer() : CT_AbstractPointsAttributesContainer()
{
}

CT_StdPointsAttributesContainer::CT_StdPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                                                 const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(model,
                                                                                                                                         result)
{
}

CT_StdPointsAttributesContainer::CT_StdPointsAttributesContainer(const QString &modelName,
                                                                 const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(modelName,
                                                                                                                                         result)
{
}

void CT_StdPointsAttributesContainer::insertPointsAttributesAt(const int &key, const CT_AbstractPointsAttributes *att)
{
    internalInsertAttributes(key, att);
}

CT_AbstractItemDrawable* CT_StdPointsAttributesContainer::copy(const CT_OutAbstractItemModel *model,
                                                               const CT_AbstractResult *result,
                                                               CT_ResultCopyModeList copyModeList)
{
    return new CT_StdPointsAttributesContainer((CT_OutAbstractSingularItemModel*)model, result);
}
