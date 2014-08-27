#ifndef DM_ITEMDRAWABLEBUILDERT_H
#define DM_ITEMDRAWABLEBUILDERT_H

#include <QMutex>

#include "dm_abstractitemdrawablebuilder.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

template<class ItemTT>
class DM_ItemDrawableTypeBackup
{
public:
    QMultiHash<ItemTT*, int>  m_itemsWithLevel;

    void add(const ItemTT &item, const int &level);
    void remove(const ItemTT &item, const int &level);
    bool isEmpty() const;
    int size() const;
};

template<typename Type, class ItemTTT>
class DM_ItemDrawableType
{
public:
    DM_ItemDrawableType() { m_backup = NULL; }
    DM_ItemDrawableType(const Type &type);
    DM_ItemDrawableType(const Type &type, const DM_ItemDrawableTypeBackup<ItemTTT> *backup);

    const Type& type() const;
    QList<ItemTTT*> itemDrawableCollection() const;
    bool isEmpty() const;

private:
    Type                                    m_type;
    DM_ItemDrawableTypeBackup<ItemTTT>      *m_backup;
};

/**
 * @brief Class tools to create a list of differents types of CT_AbstractItemDrawable added to
 *        the builder. You must set the method (getter) of the ItemDrawable that must be used to get
 *        the type and set in class template parameter the type returned by the getter.
 *
 *        A signal [listChanged] is emitted when the list changed (a new type is detected
 *        or a type is removed).
 */
template<typename T, class ItemT>
class DM_ItemDrawableBuilderT : public DM_AbstractItemDrawableBuilder
{
public:
    typedef T (ItemT::*gMethod)() const;

    DM_ItemDrawableBuilderT();
    ~DM_ItemDrawableBuilderT();

    bool tryLock();
    void lock();
    void unlock();

    /**
     * @brief Set the method that must be used to get the type to compare
     */
    void setGetter(gMethod getter);

    /**
     * @brief Set how many levels the builder must go to get types (default : 1)
     */
    void setNLevel(const int &nLevel);

    /**
     * @brief Return true it the type passed in parameter exist
     */
    bool existType(const T &type) const;

    /**
     * @brief Return the type
     */
    DM_ItemDrawableType<T,ItemT> getType(const T &type) const;

    /**
     * @brief Return the differents types added to the builder
     */
    QList< DM_ItemDrawableType<T,ItemT> > types() const;

    /**
     * @brief Return true if types is empty
     */
    bool isEmpty() const;

    /**
     * @brief Return the text used in the builder to create the map
     */
    inline T getTypeForItemDrawable(const ItemT &item) const
    {
        return ((item).*m_getter)();
    }

    void addItemDrawable(const ItemT &item);
    void removeItemDrawable(const ItemT &item);

private:
    QHash<T, DM_ItemDrawableTypeBackup<ItemT>*>         m_types;
    QMutex                                              *m_mutex;
    int                                                 m_nLevel;
    gMethod                                             m_getter;

    void recursiveAddItemDrawable(const ItemT &item, const int &level);
    void recursiveRemoveItemDrawable(const ItemT &item, const int &level);
};

#include "dm_itemdrawablebuildert.hpp"

#endif // DM_ITEMDRAWABLEBUILDERT_H
