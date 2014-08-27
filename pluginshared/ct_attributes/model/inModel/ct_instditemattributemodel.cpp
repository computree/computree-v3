#include "ct_instditemattributemodel.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

CT_InStdItemAttributeModel::CT_InStdItemAttributeModel(const QString &uniqueName,
                                                       const QString &description,
                                                       const QString &displayableName) : CT_InAbstractItemAttributeModel(uniqueName,
                                                                                                                         description,
                                                                                                                         displayableName)
{
}

QString CT_InStdItemAttributeModel::modelTypeDisplayable() const
{
    return QString("CT_InStdItemAttributeModel");
}

CT_InAbstractModel* CT_InStdItemAttributeModel::copy(bool withPossibilities) const
{
    CT_InStdItemAttributeModel *cpy = new CT_InStdItemAttributeModel(uniqueName(),
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
    return canBeComparedWith(model);
}
