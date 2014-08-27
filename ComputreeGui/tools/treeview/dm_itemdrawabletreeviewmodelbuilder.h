#ifndef DM_ITEMDRAWABLETREEVIEWMODELBUILDER_H
#define DM_ITEMDRAWABLETREEVIEWMODELBUILDER_H

#include <QVector>

#include "tools/worker/dm_abstractworker.h"
#include "tools/treeview/dm_iitemdrawablestandarditembuilder.h"

/**
 * @brief This class is a worker (can call the method apply() with a thread) to create the list of QStandardItem for the list of CT_AbstractItemDrawable
 */
class DM_ItemDrawableTreeViewModelBuilder : public DM_AbstractWorker
{
    Q_OBJECT

public:
    DM_ItemDrawableTreeViewModelBuilder();

    /**
     * @brief Set the collection to manage. The collection will be resized to map the size of the list of CT_AbstractItemDrawable passed
     *        in method setItemDrawable(...) and will contains the result of the creation made by the DM_IItemDrawableStandardItemBuilder.
     *
     *        If in the collection a list is empty you must not create the row because the CT_AbstractItemDrawable has been filtered by the DM_IItemDrawableStandardItemBuilder.
     */
    void setCollection(const QVector< QList<QStandardItem*> > *collection);

    /**
     * @brief Set the builder who transform a CT_AbstractItemDrawable in a list of QStandardItem
     */
    void setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilder *builder);

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
     * @param item : the QStandardItem to refresh
     * @param itemDrawable : the CT_AbstractItemDrawable corresponding to the QStandardItem to refresh
     * @warning the list of CT_AbstractItemDrawable to transform will be ignored
     */
    void setQStandardItemToUpdate(const QList< QPair<QStandardItem*, CT_AbstractItemDrawable*> > &list);

private:
    QVector< QList<QStandardItem*> >                *m_collection;
    DM_IItemDrawableStandardItemBuilder             *m_itemModelBuilder;
    QList<CT_AbstractItemDrawable*>                            m_items;
    QList< QPair<QStandardItem*, CT_AbstractItemDrawable*> >   m_itemsToUpdate;
    int                                             m_nLevel;

    void recursiveCreateItemForNextLevel(CT_AbstractItemDrawable *item, QStandardItem *parent, const int &level);

public slots:

    /**
     * @brief Do the compute
     */
    void apply();
};

#endif // DM_ITEMDRAWABLETREEVIEWMODELBUILDER_H
