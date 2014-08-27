#include "ct_outabstractgroupmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

CT_OutAbstractGroupModel::CT_OutAbstractGroupModel(const QString &uniqueName,
                                                   const QString &description,
                                                   const QString &displayableName,
                                                   CT_AbstractItemGroup *group) : DEF_CT_AbstractGroupModelOut(uniqueName,
                                                                                                               description,
                                                                                                               displayableName)
{
    setItem(group);
}

CT_OutAbstractGroupModel::~CT_OutAbstractGroupModel()
{
}

CT_AbstractItemGroup* CT_OutAbstractGroupModel::group() const
{
    return (CT_AbstractItemGroup*)item();
}

CT_AbstractItemDrawable *CT_OutAbstractGroupModel::itemDrawable() const
{
    return group();
}
