#include "ct_instdgroupmodel.h"

#include "def_ct_abstractgroupmodelout.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

CT_InStdGroupModel::CT_InStdGroupModel(const QString &uniqueName,
                                       const QString &groupType,
                                       const QString &displayableName,
                                       const QString &description,
                                       GroupChoiceMode choiceMode,
                                       GroupFinderMode finderMode) : CT_InAbstractGroupModel(uniqueName,
                                                                                             description,
                                                                                             displayableName)
{
    setGroupType(groupType);
    internalSetChoiceMode((CT_InAbstractGroupModel::ChoiceMode)choiceMode);
    internalSetFinderMode((CT_InAbstractGroupModel::FinderMode)finderMode);
}

QString CT_InStdGroupModel::modelTypeDisplayable() const
{
    return QString("CT_InStdGroupModel");
}

CT_InAbstractModel* CT_InStdGroupModel::copy(bool withPossibilities) const
{
    CT_InStdGroupModel *cpy = new CT_InStdGroupModel(uniqueName(),
                                                     groupType(),
                                                     displayableName(),
                                                     description(),
                                                     (CT_InAbstractGroupModel::GroupChoiceMode)choiceMode(),
                                                     (CT_InAbstractGroupModel::GroupFinderMode)finderMode());

    QListIterator< DEF_CT_AbstractGroupModelIn* > itG(groups());

    while(itG.hasNext())
        cpy->addGroupWithoutVerification((DEF_CT_AbstractGroupModelIn*)itG.next()->copy(withPossibilities));

    QListIterator<CT_InAbstractSingularItemModel*> itI(items());

    while(itI.hasNext())
        cpy->addItemWithoutVerification((CT_InAbstractSingularItemModel*)itI.next()->copy(withPossibilities));

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}

bool CT_InStdGroupModel::canBeComparedWith(const CT_OutAbstractModel &model) const
{
    // ok if the out model represent a group
    return (dynamic_cast<const CT_OutAbstractGroupModel*>(&model) != NULL);
}

bool CT_InStdGroupModel::canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const
{
    const CT_OutAbstractGroupModel *outModel = dynamic_cast<const CT_OutAbstractGroupModel*>(&model);

    if(outModel == NULL)
        return false;

    // we accept the out model to be a possibility if the item of the out model start with the type of the item of this model
    return outModel->group()->getType().startsWith(groupType());
}
