#ifndef DM_ITEMDRAWABLEBUILDERT_HPP
#define DM_ITEMDRAWABLEBUILDERT_HPP

#include "dm_itemdrawablebuildert.h"

template<typename T, class ItemT>
DM_ItemDrawableBuilderT<T,ItemT>::DM_ItemDrawableBuilderT() : DM_AbstractItemDrawableBuilder()
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_nLevel = 1;
    m_getter = NULL;
}

template<typename T, class ItemT>
DM_ItemDrawableBuilderT<T,ItemT>::~DM_ItemDrawableBuilderT()
{
    delete m_mutex;
    qDeleteAll(m_types.begin(), m_types.end());
}

template<typename T, class ItemT>
bool DM_ItemDrawableBuilderT<T,ItemT>::tryLock()
{
    return m_mutex->tryLock();
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::lock()
{
    m_mutex->lock();
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::unlock()
{
    m_mutex->unlock();
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::setGetter(gMethod getter)
{
    m_getter = getter;
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::setNLevel(const int &nLevel)
{
    QMutexLocker locker(m_mutex);

    if(nLevel != m_nLevel)
    {
        if(nLevel < m_nLevel)
        {
            // remove all types that are after this new level
        }

        m_nLevel = nLevel;
    }
}

template<typename T, class ItemT>
bool DM_ItemDrawableBuilderT<T,ItemT>::existType(const T &type) const
{
    QMutexLocker locker(m_mutex);

    return m_types.contains(type);
}

template<typename T, class ItemT>
DM_ItemDrawableType<T,ItemT> DM_ItemDrawableBuilderT<T,ItemT>::getType(const T &type) const
{
    QMutexLocker locker(m_mutex);

    return DM_ItemDrawableType<T,ItemT>(type, m_types.value(type, NULL));
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::addItemDrawable(const ItemT &item)
{
    recursiveAddItemDrawable(item, 0);
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::removeItemDrawable(const ItemT &item)
{
    recursiveRemoveItemDrawable(item, 0);
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::recursiveAddItemDrawable(const ItemT &item, const int &level)
{
    lock();

    T t = getTypeForItemDrawable(item);
    DM_ItemDrawableTypeBackup<ItemT> *backup = m_types.value(t, NULL);

    if(backup == NULL)
    {
        backup = new DM_ItemDrawableTypeBackup<ItemT>();
        m_types.insert(t, backup);
    }

    backup->add(item, level);
    int size = backup->size();

    unlock();

    /*CT_ChildIterator it(&item);

    if(((level+1) < m_nLevel)
            && it.hasNext())
    {
        while(it.hasNext())
            recursiveAddItemDrawable(dynamic_cast<CT_AbstractItemDrawable*>(it.next()), level+1);
    }*/

    if(size == 1)
    {
        emit listChanged();
        emit added();
    }
}

template<typename T, class ItemT>
void DM_ItemDrawableBuilderT<T,ItemT>::recursiveRemoveItemDrawable(const ItemT &item, const int &level)
{
    lock();

    T t = getTypeForItemDrawable(item);

    DM_ItemDrawableTypeBackup<ItemT> *backup = m_types.value(t, NULL);

    backup->remove(item, level);
    bool isE = backup->isEmpty();

    unlock();

    /*CT_ChildIterator it(&item);

    if(((level+1) < m_nLevel)
            && it.hasNext())
    {
        while(it.hasNext())
            recursiveAddItemDrawable(dynamic_cast<CT_AbstractItemDrawable*>(it.next()), level+1);
    }*/

    if(isE)
    {
        lock();
        delete m_types.take(t);
        unlock();

        emit listChanged();
        emit removed();
    }
}

template<typename T, class ItemT>
QList< DM_ItemDrawableType<T,ItemT> > DM_ItemDrawableBuilderT<T,ItemT>::types() const
{
    QMutexLocker locker(m_mutex);

    QList<DM_ItemDrawableType<T,ItemT> > l;

    QHashIterator<T, DM_ItemDrawableTypeBackup<ItemT>*> it(m_types);

    while(it.hasNext())
    {
        it.next();

        l.append(DM_ItemDrawableType<T,ItemT>(it.key(), it.value()));
    }

    return l;
}

template<typename T, class ItemT>
bool DM_ItemDrawableBuilderT<T,ItemT>::isEmpty() const
{
    QMutexLocker locker(m_mutex);

    return m_types.isEmpty();
}

template<typename Type, class ItemTTT>
DM_ItemDrawableType<Type,ItemTTT>::DM_ItemDrawableType(const Type &type)
{
    m_type = type;
    m_backup = NULL;
}

template<typename Type, class ItemTTT>
DM_ItemDrawableType<Type,ItemTTT>::DM_ItemDrawableType(const Type &type, const DM_ItemDrawableTypeBackup<ItemTTT> *backup)
{
    m_type = type;
    m_backup = (DM_ItemDrawableTypeBackup<ItemTTT>*)backup;
}

template<typename Type, class ItemTTT>
const Type& DM_ItemDrawableType<Type,ItemTTT>::type() const
{
    return m_type;
}

template<typename Type, class ItemTTT>
QList<ItemTTT*> DM_ItemDrawableType<Type,ItemTTT>::itemDrawableCollection() const
{
    return (m_backup != NULL ? m_backup->m_itemsWithLevel.uniqueKeys() : QList<ItemTTT*>());
}

template<typename Type, class ItemTTT>
bool DM_ItemDrawableType<Type,ItemTTT>::isEmpty() const
{
    return (m_backup != NULL ? m_backup->m_itemsWithLevel.isEmpty() : true);
}

template<class ItemTT>
void DM_ItemDrawableTypeBackup<ItemTT>::add(const ItemTT &item, const int &level)
{
    m_itemsWithLevel.insert((ItemTT*)&item, level);
}

template<class ItemTT>
void DM_ItemDrawableTypeBackup<ItemTT>::remove(const ItemTT &item, const int &level)
{
    m_itemsWithLevel.remove((ItemTT*)&item, level);
}

template<class ItemTT>
bool DM_ItemDrawableTypeBackup<ItemTT>::isEmpty() const
{
    return m_itemsWithLevel.isEmpty();
}

template<class ItemTT>
int DM_ItemDrawableTypeBackup<ItemTT>::size() const
{
    return m_itemsWithLevel.size();
}


#endif // DM_ITEMDRAWABLEBUILDERT_HPP
