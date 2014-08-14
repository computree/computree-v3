#ifndef DM_ITEMDRAWABLETREEVIEWMODELRESEARCHER_H
#define DM_ITEMDRAWABLETREEVIEWMODELRESEARCHER_H

#include <QVector>

#include "tools/worker/dm_abstractworker.h"
#include "tools/treeview/dm_itreeviewmanager.h"

/**
 * @brief This class is a worker (can call the method apply() with a thread) to transform a list of ItemDrawable to a list of QStandardItem (mapping).
 */
class DM_ItemDrawableTreeViewModelResearcher : public DM_AbstractWorker
{
    Q_OBJECT

public:
    DM_ItemDrawableTreeViewModelResearcher();

    /**
     * @brief Set the collection to manage. The collection will be resized to map the size of the list of ItemDrawable passed
     *        in method setItemDrawable(...) and will contains the result of the mapping made by the DM_ITreeViewManager (method 'getItem').
     */
    void setCollection(const QVector< QList<QStandardItem*> > *collection);

    /**
     * @brief Set the manager who get a QStandardItem for a ItemDrawable
     */
    void setTreeViewManager(const DM_ITreeViewManager *man);

    /**
     * @brief Set the list of ItemDrawable to transform
     */
    void setItemDrawable(const QList<ItemDrawable*> &list);

private:
    QVector< QList<QStandardItem*> >        *m_collection;
    DM_ITreeViewManager                     *m_treeViewManager;
    QList<ItemDrawable*>                    m_items;

public slots:
    /**
     * @brief Do the compute
     */
    void apply();
};

#endif // DM_ITEMDRAWABLETREEVIEWMODELRESEARCHER_H
