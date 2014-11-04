#include "ct_stdlaspointsattributescontainer.h"

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer() : CT_AbstractPointsAttributesContainer()
{
}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(model,
                                                                                                                                               result)
{

}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const QString &modelName,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(modelName,
                                                                                                                                               result)
{

}

QString CT_StdLASPointsAttributesContainer::getType() const
{
    return staticGetType();
}

QString CT_StdLASPointsAttributesContainer::staticGetType()
{
    return CT_AbstractPointsAttributesContainer::staticGetType() + "/CT_StdLASPointsAttributesContainer";
}

void CT_StdLASPointsAttributesContainer::insertPointsAttributesAt(CT_StdLASPointsAttributesContainer::LASPointAttributesType key, const CT_AbstractPointsAttributes *att)
{
    if(att != NULL)
        internalInsertAttributes(key, att);
}

CT_AbstractItemDrawable *CT_StdLASPointsAttributesContainer::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_StdLASPointsAttributesContainer((CT_OutAbstractSingularItemModel*)model, result);
}
