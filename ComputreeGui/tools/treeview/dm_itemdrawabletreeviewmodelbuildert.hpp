#ifndef DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_HPP
#define DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_HPP

#include "tools/treeview/dm_itemdrawabletreeviewmodelbuildert.h"

template<class Item>
DM_ItemDrawableTreeViewModelBuilderT<Item>::DM_ItemDrawableTreeViewModelBuilderT() : DM_AbstractWorker()
{
    m_collection = NULL;
    m_itemModelBuilder = NULL;
    m_nLevel = 1;
    m_itemsToUpdate.clear();
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::setCollection(const QVector<QList<Item*> > *collection)
{
    m_collection = (QVector<QList<Item*> >*)collection;
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilderT<Item> *builder)
{
    m_itemModelBuilder = (DM_IItemDrawableStandardItemBuilderT<Item>*)builder;
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::setNLevelToBuild(const int &nLevel)
{
    m_nLevel = nLevel;
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::setItemDrawable(const QList<CT_AbstractItemDrawable *> &list)
{
    m_items = list;
    m_itemsToUpdate.clear();
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::setQStandardItemToUpdate(const QList<QPair<Item *, CT_AbstractItemDrawable *> > &list)
{
    m_itemsToUpdate = list;
    m_items.clear();
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::recursiveCreateItemForNextLevel(CT_AbstractItemDrawable *item, Item *parent, const int &level)
{
    CT_ChildIterator it(item);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *child = dynamic_cast<CT_AbstractItemDrawable*>((CT_AbstractItem*)it.next());

        QList<Item*> items = m_itemModelBuilder->createItems(*child, level);

        if(!items.isEmpty())
        {
            static_cast<Item*>(parent)->appendRow(items);

            if((level+1) < m_nLevel)
                recursiveCreateItemForNextLevel(child, items.first(), level+1);
        }
    }
}

template<class Item>
void DM_ItemDrawableTreeViewModelBuilderT<Item>::apply()
{
    if(m_itemsToUpdate.isEmpty())
    {
        int size = m_items.size();
        int i = 0;

        m_collection->resize(size);

        QListIterator<CT_AbstractItemDrawable *> it(m_items);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            QList<Item*> items = m_itemModelBuilder->createItems(*item, 0);

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

        QListIterator< QPair<Item*, CT_AbstractItemDrawable*> > it(m_itemsToUpdate);

        while(it.hasNext())
        {
            const QPair<Item*, CT_AbstractItemDrawable*> &pair = it.next();

            int level = 0;
            Item *parent = pair.first->parent();

            while(parent != NULL)
            {
                ++level;
                parent = parent->parent();
            }

            QList<Item*> items = m_itemModelBuilder->createItems(*pair.second, level);

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

#endif // DM_ITEMDRAWABLETREEVIEWMODELBUILDERT_HPP
