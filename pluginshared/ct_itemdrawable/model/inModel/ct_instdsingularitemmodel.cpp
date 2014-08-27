#include "ct_instdsingularitemmodel.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

CT_InStdSingularItemModel::CT_InStdSingularItemModel(const QString &uniqueName,
                                                     const QString &itemType,
                                                     const QString &displayableName,
                                                     const QString &description,
                                                     CT_InStdSingularItemModel::ChoiceMode choiceMode,
                                                     CT_InStdSingularItemModel::FinderMode finderMode) : DEF_CT_AbstractItemDrawableModelIn(uniqueName,
                                                                                                                                           description,
                                                                                                                                           displayableName)
{
    setItemType(itemType);
    internalSetChoiceMode(choiceMode);
    internalSetFinderMode(finderMode);
}

QString CT_InStdSingularItemModel::modelTypeDisplayable() const
{
    return QString("CT_InStdSingularItemModel");
}

CT_InAbstractModel* CT_InStdSingularItemModel::copy(bool withPossibilities) const
{
    CT_InStdSingularItemModel *cpy = new CT_InStdSingularItemModel(*this);

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}

bool CT_InStdSingularItemModel::canBeComparedWith(const CT_OutAbstractModel &model) const
{
    // ok if the out model represent a singular item
    return (dynamic_cast<const CT_OutAbstractSingularItemModel*>(&model) != NULL);
}

bool CT_InStdSingularItemModel::canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const
{
    const CT_OutAbstractSingularItemModel *outModel = dynamic_cast<const CT_OutAbstractSingularItemModel*>(&model);

    if(outModel == NULL)
        return false;

    // we accept the out model to be a possibility if the item of the out model start with the type of the item of this model
    return outModel->itemDrawable()->getType().startsWith(itemType());
}

CT_InStdSingularItemModel::CT_InStdSingularItemModel(const CT_InStdSingularItemModel &model) : DEF_CT_AbstractItemDrawableModelIn(model.uniqueName(),
                                                                                                                                  model.description(),
                                                                                                                                  model.displayableName())
{
    setItemType(model.itemType());
    internalSetChoiceMode(model.choiceMode());
    internalSetFinderMode(model.finderMode());
}
