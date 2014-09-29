#include "ct_itemattributelist.h"

CT_ItemAttributeList::CT_ItemAttributeList() : CT_AbstractSingularItemDrawable()
{
}

CT_ItemAttributeList::CT_ItemAttributeList(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{
}

CT_ItemAttributeList::CT_ItemAttributeList(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{
}

QString CT_ItemAttributeList::getType() const
{
    return staticGetType();
}

QString CT_ItemAttributeList::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_ItemAttributeList";
}

CT_AbstractItemDrawable* CT_ItemAttributeList::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList)

    return new CT_ItemAttributeList((const CT_OutAbstractSingularItemModel *)model, result);
}
