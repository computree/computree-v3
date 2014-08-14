#include "dm_itemdrawabletreeviewmodelresearcher.h"

DM_ItemDrawableTreeViewModelResearcher::DM_ItemDrawableTreeViewModelResearcher() : DM_AbstractWorker()
{
    m_collection = NULL;
    m_treeViewManager = NULL;
}

void DM_ItemDrawableTreeViewModelResearcher::setCollection(const QVector<QList<QStandardItem*> > *collection)
{
    m_collection = (QVector<QList<QStandardItem*> >*)collection;
}

void DM_ItemDrawableTreeViewModelResearcher::setTreeViewManager(const DM_ITreeViewManager *man)
{
    m_treeViewManager = (DM_ITreeViewManager*)man;
}

void DM_ItemDrawableTreeViewModelResearcher::setItemDrawable(const QList<ItemDrawable *> &list)
{
    m_items = list;
}

void DM_ItemDrawableTreeViewModelResearcher::apply()
{
    int size = m_items.size();
    int i = 0;

    m_collection->resize(size);

    QListIterator<ItemDrawable *> it(m_items);

    while(it.hasNext())
    {
        ItemDrawable *item = it.next();

        QStandardItem* sItem = m_treeViewManager->itemFromItemDrawable(item);

        if(sItem != NULL)
            (*m_collection)[i].append(sItem);

        ++i;
        setProgress((i*100)/size);
    }

    setFinished();
}
