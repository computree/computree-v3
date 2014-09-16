#include "ct_outabstractitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

CT_OutAbstractItemModel::CT_OutAbstractItemModel(const QString &uniqueName,
                                                 const QString &description,
                                                 const QString &displayableName) : CT_OutAbstractModel(uniqueName,
                                                                                                       description,
                                                                                                       displayableName)
{
}

void CT_OutAbstractItemModel::setItem(CT_AbstractItem *item)
{
    if(item != NULL)
        static_cast<CT_AbstractItemDrawable*>(item)->setModel(this);

    CT_OutAbstractModel::setItem(item);
}
