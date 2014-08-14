#include "dm_itemdrawabletreeviewmodelbuilder.h"

DM_ItemDrawableTreeViewModelBuilder::DM_ItemDrawableTreeViewModelBuilder() : DM_AbstractWorker()
{
    m_collection = NULL;
    m_itemModelBuilder = NULL;
    m_nLevel = 1;
    m_itemsToUpdate.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::setCollection(const QVector<QList<QStandardItem*> > *collection)
{
    m_collection = (QVector<QList<QStandardItem*> >*)collection;
}

void DM_ItemDrawableTreeViewModelBuilder::setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilder *builder)
{
    m_itemModelBuilder = (DM_IItemDrawableStandardItemBuilder*)builder;
}

void DM_ItemDrawableTreeViewModelBuilder::setNLevelToBuild(const int &nLevel)
{
    m_nLevel = nLevel;
}

void DM_ItemDrawableTreeViewModelBuilder::setItemDrawable(const QList<ItemDrawable *> &list)
{
    m_items = list;
    m_itemsToUpdate.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::setQStandardItemToUpdate(const QList<QPair<QStandardItem *, ItemDrawable *> > &list)
{
    m_itemsToUpdate = list;
    m_items.clear();
}

void DM_ItemDrawableTreeViewModelBuilder::recursiveCreateItemForNextLevel(ItemDrawable *item, QStandardItem *parent, const int &level)
{
    if(item->beginIterateChild())
    {
        ItemDrawable *child;

        while((child = item->nextChild()) != NULL)
        {
            QList<QStandardItem*> items = m_itemModelBuilder->createItems(*child, level);

            if(!items.isEmpty())
            {
                parent->appendRow(items);

                if((level+1) < m_nLevel)
                {
                    recursiveCreateItemForNextLevel(child, items.first(), level+1);
                }
            }
        }
    }
}

void DM_ItemDrawableTreeViewModelBuilder::apply()
{
    if(m_itemsToUpdate.isEmpty())
    {
        int size = m_items.size();
        int i = 0;

        m_collection->resize(size);

        QListIterator<ItemDrawable *> it(m_items);

        while(it.hasNext())
        {
            ItemDrawable *item = it.next();

            QList<QStandardItem*> items = m_itemModelBuilder->createItems(*item, 0);

            if(!items.isEmpty())
            {
                (*m_collection)[i] = items;

                if(m_nLevel > 1)
                    recursiveCreateItemForNextLevel(item, items.first(), 1);
            }

            ++i;
            setProgress((i*100)/size);
        }
    }
    else
    {
        int size = m_itemsToUpdate.size();
        int i = 0;

        m_collection->resize(size);

        QListIterator< QPair<QStandardItem*, ItemDrawable*> > it(m_itemsToUpdate);

        while(it.hasNext())
        {
            const QPair<QStandardItem*, ItemDrawable*> &pair = it.next();

            int level = 0;
            QStandardItem *parent = pair.first->parent();

            while(parent != NULL)
            {
                ++level;
                parent = parent->parent();
            }

            QList<QStandardItem*> items = m_itemModelBuilder->createItems(*pair.second, level);

            if(!items.isEmpty())
            {
                (*m_collection)[i] = items;

                if(m_nLevel > (level+1))
                    recursiveCreateItemForNextLevel(pair.second, items.first(), level+1);
            }

            ++i;
            setProgress((i*100)/size);
        }
    }

    setFinished();
}
