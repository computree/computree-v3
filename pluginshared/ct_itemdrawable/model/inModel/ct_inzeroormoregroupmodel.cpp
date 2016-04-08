#include "ct_inzeroormoregroupmodel.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

CT_InZeroOrMoreGroupModel::CT_InZeroOrMoreGroupModel() : CT_InAbstractGroupModel("*",
                                                                                 tr("Aucun ou un groupe"),
                                                                                 tr("*"))
{
    setGroupType(CT_AbstractItemGroup::staticGetType());
    internalSetChoiceMode(CT_InAbstractModel::C_DontChoose);
    internalSetFinderMode(CT_InAbstractModel::F_IsObligatory);
}

QString CT_InZeroOrMoreGroupModel::modelTypeDisplayable() const
{
    return QString("CT_InZeroOrMoreGroupModel");
}

bool CT_InZeroOrMoreGroupModel::isAtLeastOnePossibilitySelectedIfItDoes() const
{
    // if this model not need possiblities
    if(!needOutputModel())
        return true;

    QList<CT_AbstractModel*> l = childrensOfPossibilities();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        CT_InAbstractModel *model = (CT_InAbstractModel*)it.next();

        model->clearError();

        // if no possibilities of this children (and recursively) is selected : we return false
        if(!model->recursiveIsAtLeastOnePossibilitySelectedIfItDoes()) {
            addToError(model->errors());
            return false;
        }
    }

    // all it's ok because this model is obligatory but it can not add a possibility if
    // childrens (recursively) is ok
    return true;
}

CT_InAbstractModel* CT_InZeroOrMoreGroupModel::copy(bool withPossibilities) const
{
    CT_InZeroOrMoreGroupModel *cpy = new CT_InZeroOrMoreGroupModel();

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

bool CT_InZeroOrMoreGroupModel::canBeComparedWith(const CT_OutAbstractModel &model) const
{
    // ok if the out model represent a group
    return (dynamic_cast<const CT_OutAbstractGroupModel*>(&model) != NULL);
}

bool CT_InZeroOrMoreGroupModel::canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const
{
    return canBeComparedWith(model);
}

bool CT_InZeroOrMoreGroupModel::mustCompareThisWithChildrenOfOutModel() const
{
    // we accept to compare this model with children of a model
    return true;
}

bool CT_InZeroOrMoreGroupModel::mustCompareOutModelWithChildrenOfThisModel() const
{
    // we accept to compare children of this model with the current output model
    return true;
}
