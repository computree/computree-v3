#ifndef DM_IITEMDRAWABLESTANDARDITEMBUILDER_H
#define DM_IITEMDRAWABLESTANDARDITEMBUILDER_H

#include <QStandardItem>

#include "interfaces.h"

/**
 * @brief You must inherit this class if you want to use the DM_ItemDrawableTreeViewController.
 */
class DM_IItemDrawableStandardItemBuilder
{
public:
    virtual ~DM_IItemDrawableStandardItemBuilder() {}

    /**
     * @brief Return the list of QStandardItem to add in a row for the ItemDrawable 'item' at level 'level'. If
     *        this method return a empty list the row is not created but the build of the QStandardItemModel continue.
     *
     *        If you want to filter certains ItemDrawable you can return a empty list.
     */
    virtual QList<QStandardItem*> createItems(const ItemDrawable &item, const int &level) const = 0;
};

#endif // DM_IITEMDRAWABLESTANDARDITEMBUILDER_H
