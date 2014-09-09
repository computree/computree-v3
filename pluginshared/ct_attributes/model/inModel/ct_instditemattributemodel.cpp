#include "ct_instditemattributemodel.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

CT_InStdItemAttributeModel::CT_InStdItemAttributeModel(const QString &uniqueName,
                                                       const QString &categoryType,
                                                       CT_AbstractCategory::ValueType valueType,
                                                       const QString &displayableName,
                                                       const QString &description,
                                                       ChoiceMode choiceMode,
                                                       FinderMode finderMode) : CT_InAbstractItemAttributeModel(uniqueName,
                                                                                                                         QList<QString>() << categoryType,
                                                                                                                         valueType,
                                                                                                                         description,
                                                                                                                         displayableName)
{
    internalSetChoiceMode(choiceMode);
    internalSetFinderMode(finderMode);
}

CT_InStdItemAttributeModel::CT_InStdItemAttributeModel(const QString &uniqueName,
                                                       const QList<QString> &categoriesType,
                                                       CT_AbstractCategory::ValueType valueType,
                                                       const QString &displayableName,
                                                       const QString &description,
                                                       ChoiceMode choiceMode,
                                                       FinderMode finderMode) : CT_InAbstractItemAttributeModel(uniqueName,
                                                                                                                         categoriesType,
                                                                                                                         valueType,
                                                                                                                         description,
                                                                                                                         displayableName)
{
    internalSetChoiceMode(choiceMode);
    internalSetFinderMode(finderMode);
}

QString CT_InStdItemAttributeModel::modelTypeDisplayable() const
{
    return QString("CT_InStdItemAttributeModel");
}

CT_InAbstractModel* CT_InStdItemAttributeModel::copy(bool withPossibilities) const
{
    CT_InStdItemAttributeModel *cpy = new CT_InStdItemAttributeModel(uniqueName(),
                                                                     categoriesType(),
                                                                     valueType(),
                                                                     description(),
                                                                     displayableName());

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}

bool CT_InStdItemAttributeModel::canBeComparedWith(const CT_OutAbstractModel &model) const
{
    return (dynamic_cast<const CT_OutAbstractItemAttributeModel*>(&model) != NULL);
}

bool CT_InStdItemAttributeModel::canAddPossibilityForThisModel(const CT_OutAbstractModel &model) const
{
    const CT_OutAbstractItemAttributeModel *iaModel = dynamic_cast<const CT_OutAbstractItemAttributeModel*>(&model);

    if(iaModel == NULL)
        return false;

    CT_AbstractItemAttribute *ia = iaModel->itemAttribute();

    if(!categoriesType().contains(ia->category()->uniqueName()))
    {
        bool ok = false;
        QList<QString> l = categoriesType();
        QListIterator<QString> it(l);

        while(it.hasNext()
              && !ok)
        {
            if(ia->category()->isEquivalentTo(it.next()))
                ok = true;
        }

        if(!ok)
            return false;
    }

    if((ia->type() & valueType()) == 0)
        return false;

    return true;
}
