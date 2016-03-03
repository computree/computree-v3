#ifndef CT_ITEMATTRIBUTELIST_H
#define CT_ITEMATTRIBUTELIST_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

/**
 * @brief A singular item that contains item attributes
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemAttributeList : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_ItemAttributeList, CT_AbstractSingularItemDrawable)

public:
    CT_ItemAttributeList();

    CT_ItemAttributeList(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result);

    CT_ItemAttributeList(const QString &modelName,
                         const CT_AbstractResult *result);

    /**
     * @brief Returns a copy of this item
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_ITEMATTRIBUTELIST_H
