#include "ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

CT_OutAbstractSingularItemModel::CT_OutAbstractSingularItemModel(const QString &uniqueName,
                                                                 const QString &description,
                                                                 const QString &displayableName) : CT_OutAbstractItemModel(uniqueName,
                                                                                                                           description,
                                                                                                                           displayableName)
{
}

CT_OutAbstractSingularItemModel::~CT_OutAbstractSingularItemModel()
{
}

CT_AbstractSingularItemDrawable* CT_OutAbstractSingularItemModel::singularItem() const
{
    return (CT_AbstractSingularItemDrawable*)item();
}

CT_AbstractItemDrawable* CT_OutAbstractSingularItemModel::itemDrawable() const
{
    return singularItem();
}
