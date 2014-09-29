#ifndef CT_ITEMATTRIBUTELIST_H
#define CT_ITEMATTRIBUTELIST_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

/**
 * @brief A singular item that contains item attributes
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemAttributeList : public CT_AbstractSingularItemDrawable
{
    // IMPORTANT to have the name of the ItemDrawable in the view
    Q_OBJECT

public:
    CT_ItemAttributeList();

    CT_ItemAttributeList(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result);

    CT_ItemAttributeList(const QString &modelName,
                         const CT_AbstractResult *result);

    /**
      * WARNING : don't forget to redefined this methods if you inherit this class
      */
    virtual QString getType() const;
    static QString staticGetType();

    /**
     * @brief Returns a copy of this item
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_ITEMATTRIBUTELIST_H
