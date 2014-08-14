#include "ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

CT_OutAbstractSingularItemModel::CT_OutAbstractSingularItemModel(const QString &uniqueName,
                                                                 const QString &description,
                                                                 const QString &displayableName) : CT_OutAbstractItemModel(uniqueName,
                                                                                                                           description,
                                                                                                                           displayableName)
{
    _item = NULL;
}

CT_OutAbstractSingularItemModel::~CT_OutAbstractSingularItemModel()
{
    clearItem();
}

CT_AbstractSingularItemDrawable* CT_OutAbstractSingularItemModel::singularItem() const
{
    return _item;
}

CT_AbstractItemDrawable* CT_OutAbstractSingularItemModel::item() const
{
    return singularItem();
}

void CT_OutAbstractSingularItemModel::clearItem()
{
    delete _item;
    _item = NULL;
}

void CT_OutAbstractSingularItemModel::setSingularItem(CT_AbstractSingularItemDrawable *item)
{
    clearItem();
    _item = item;
}
