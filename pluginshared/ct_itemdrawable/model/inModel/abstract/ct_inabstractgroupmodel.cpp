#include "ct_inabstractgroupmodel.h"

#include "ct_itemdrawable/model/inModel/abstract/def_ct_abstractgroupmodelin.h"

CT_InAbstractGroupModel::CT_InAbstractGroupModel(const QString &uniqueName,
                                                 const QString &description,
                                                 const QString &displayableName) : DEF_CT_AbstractGroupModelIn(uniqueName,
                                                                                                               description,
                                                                                                               displayableName)
{
}

QString CT_InAbstractGroupModel::groupShortType() const
{
    return itemShortType();
}

QString CT_InAbstractGroupModel::groupType() const
{
    return itemType();
}

void CT_InAbstractGroupModel::setGroupType(const QString &groupType)
{
    setItemType(groupType);
}
