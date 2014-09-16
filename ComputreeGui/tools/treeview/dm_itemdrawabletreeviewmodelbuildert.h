#ifndef DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_H
#define DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_H

#include <QVector>

#include "tools/worker/dm_abstractworker.h"
#include "tools/treeview/dm_iitemdrawablestandarditembuildert.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_item/tools/iterator/ct_childiterator.h"

/**
 * @brief This class is a worker (can call the method apply() with a thread) to create the list of QStandardItem for the list of CT_AbstractItemDrawable
 */
template<class Item>
class DM_ItemDrawableTreeViewModelBuilderT : public DM_AbstractWorker
{
public:
    DM_ItemDrawableTreeViewModelBuilderT();

    /**
     * @brief Set the collection to manage. The collection will be resized to map the size of the list of CT_AbstractItemDrawable passed
     *        in method setItemDrawable(...) and will contains the result of the creation made by the DM_IItemDrawableStandardItemBuilder.
     *
     *        If in the collection a list is empty you must not create the row because the CT_AbstractItemDrawable has been filtered by the DM_IItemDrawableStandardItemBuilder.
     */
    void setCollection(const QVector< QList<Item*> > *collection);

    /**
     * @brief Set the builder who transform a CT_AbstractItemDrawable in a list of QStandardItem
     */
    void setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilderT<Item> *builder);

    /**
     * @brief Set how many level to build
     */
    void setNLevelToBuild(const int &nLevel);

    /**
     * @brief Set the list of CT_AbstractItemDrawable to transform
     *
     * @warning If a QStandarItem was set to be updated, it will be ignored
     */
    void setItemDrawable(const QList<CT_AbstractItemDrawable*> &list);

    /**
     * @brief If you want to refresh a QStandardItem set it with this method
     * @param item : the Item to refresh
     * @param itemDrawable : the CT_AbstractItemDrawable corresponding to the Item to refresh
     * @warning the list of CT_AbstractItemDrawable to transform will be ignored
     */
    void setQStandardItemToUpdate(const QList< QPair<Item*, CT_AbstractItemDrawable*> > &list);

private:
    QVector< QList<Item*> >                                     *m_collection;
    DM_IItemDrawableStandardItemBuilderT<Item>                  *m_itemModelBuilder;
    QList<CT_AbstractItemDrawable*>                             m_items;
    QList< QPair<Item*, CT_AbstractItemDrawable*> >             m_itemsToUpdate;
    int                                                         m_nLevel;

    void recursiveCreateItemForNextLevel(CT_AbstractItemDrawable *item, Item *parent, const int &level);

public:

    /**
     * @brief Do the compute
     */
    void apply();
};

#include "tools/treeview/dm_itemdrawabletreeviewmodelbuildert.hpp"

#endif // DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_H
