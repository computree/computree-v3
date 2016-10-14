#include "ct_standarditemgroup.h"
#include <assert.h>

CT_StandardItemGroup::CT_StandardItemGroup() : CT_AbstractStandardItemGroup()
{
}

CT_StandardItemGroup::CT_StandardItemGroup(const CT_OutAbstractGroupModel *model,
                                           const CT_AbstractResult *result) : CT_AbstractStandardItemGroup(model, result)
{
}

CT_StandardItemGroup::CT_StandardItemGroup(const QString &modelName,
                                           const CT_AbstractResult *result) : CT_AbstractStandardItemGroup(modelName, result)
{
}

QString CT_StandardItemGroup::name() const
{
    return tr("Standard group");
}

bool CT_StandardItemGroup::addGroup(CT_AbstractItemGroup *group)
{
    return protectedAddGroup(group);
}

bool CT_StandardItemGroup::addItemDrawable(CT_AbstractSingularItemDrawable *item)
{
    return protectedAddItemDrawable(item);
}

CT_AbstractItemDrawable* CT_StandardItemGroup::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    const CT_OutAbstractGroupModel *newModel = dynamic_cast< const CT_OutAbstractGroupModel* >(model);

    CT_StandardItemGroup *itemGroup = new CT_StandardItemGroup(newModel, result);
    itemGroup->setId(id());
    itemGroup->setAlternativeDrawManager(getAlternativeDrawManager());

    if(!copyStructure(itemGroup))
    {
        delete itemGroup;
        return NULL;
    }

    return itemGroup;
}
