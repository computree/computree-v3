#ifndef DM_ITEMDRAWABLETREEVIEWMODELRESEARCHERT_H
#define DM_ITEMDRAWABLETREEVIEWMODELRESEARCHERT_H

#include <QVector>

#include "tools/worker/dm_abstractworker.h"
#include "tools/treeview/dm_itreeviewmanagert.h"

/**
 * @brief This class is a worker (can call the method apply() with a thread) to transform a list of CT_AbstractItemDrawable to a list of QStandardItem (mapping).
 */
template<class Item>
class DM_ItemDrawableTreeViewModelResearcherT : public DM_AbstractWorker
{
public:
    DM_ItemDrawableTreeViewModelResearcherT();

    /**
     * @brief Set the collection to manage. The collection will be resized to map the size of the list of CT_AbstractItemDrawable passed
     *        in method setItemDrawable(...) and will contains the result of the mapping made by the DM_ITreeViewManager (method 'getItem').
     */
    void setCollection(const QVector< QList<Item*> > *collection);

    /**
     * @brief Set the manager who get a QStandardItem for a CT_AbstractItemDrawable
     */
    void setTreeViewManager(const DM_ITreeViewManagerT<Item> *man);

    /**
     * @brief Set the list of CT_AbstractItemDrawable to transform
     */
    void setItemDrawable(const QList<CT_AbstractItemDrawable*> &list);

private:
    QVector< QList<Item*> >             *m_collection;
    DM_ITreeViewManagerT<Item>          *m_treeViewManager;
    QList<CT_AbstractItemDrawable*>     m_items;

public:
    /**
     * @brief Do the compute
     */
    void apply();
};

#include "tools/treeview/dm_itemdrawabletreeviewmodelresearchert.hpp"

#endif // DM_ITEMDRAWABLETREEVIEWMODELRESEARCHERT_H
