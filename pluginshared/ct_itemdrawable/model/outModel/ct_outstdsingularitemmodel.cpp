#include "ct_outstdsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

CT_OutStdSingularItemModel::CT_OutStdSingularItemModel(const QString &uniqueName,
                                                       CT_AbstractSingularItemDrawable *item,
                                                       const QString &displayableName,
                                                       const QString &description) : DEF_CT_AbstractItemDrawableModelOut(uniqueName,
                                                                                                                         description,
                                                                                                                         displayableName.isEmpty() ? item->name() : displayableName)
{
    setSingularItem(item);
}

CT_OutAbstractModel *CT_OutStdSingularItemModel::copy() const
{
    CT_OutStdSingularItemModel *cpy = new CT_OutStdSingularItemModel(uniqueName(),
                                                                     NULL,
                                                                     displayableName(),
                                                                     description());

    if(item() != NULL)
        cpy->setSingularItem((CT_AbstractSingularItemDrawable*)item()->copy(cpy, result(), CT_ResultCopyModeList()));

    cpy->setOriginalModel(this);

    return cpy;
}
