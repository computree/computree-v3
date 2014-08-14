#include "dm_itemdrawabletypebuilder.h"

DM_ItemDrawableTypeBuilder::DM_ItemDrawableTypeBuilder()
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_nLevel = 1;
}

DM_ItemDrawableTypeBuilder::~DM_ItemDrawableTypeBuilder()
{
    delete m_mutex;
    qDeleteAll(m_types.begin(), m_types.end());
}

bool DM_ItemDrawableTypeBuilder::tryLock()
{
    return m_mutex->tryLock();
}

void DM_ItemDrawableTypeBuilder::lock()
{
    m_mutex->lock();
}

void DM_ItemDrawableTypeBuilder::unlock()
{
    m_mutex->unlock();
}

void DM_ItemDrawableTypeBuilder::setNLevel(const int &nLevel)
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

bool DM_ItemDrawableTypeBuilder::existTypeWithName(const QString &name) const
{
    QMutexLocker locker(m_mutex);

    return m_types.contains(name);
}

DM_ItemDrawableType DM_ItemDrawableTypeBuilder::getTypeWithName(const QString &name) const
{
    QMutexLocker locker(m_mutex);

    return DM_ItemDrawableType(name, m_types.value(name, NULL));
}

void DM_ItemDrawableTypeBuilder::addItemDrawable(const ItemDrawable &item)
{
    recursiveAddItemDrawable(item, 0);
}

void DM_ItemDrawableTypeBuilder::removeItemDrawable(const ItemDrawable &item)
{
    recursiveRemoveItemDrawable(item, 0);
}

void DM_ItemDrawableTypeBuilder::recursiveAddItemDrawable(const ItemDrawable &item, const int &level)
{
    lock();

    QString t = getTypeForItemDrawable(item);
    DM_ItemDrawableTypeBackup *backup = m_types.value(t, NULL);

    if(backup == NULL)
    {
        backup = new DM_ItemDrawableTypeBackup();
        m_types.insert(t, backup);
    }

    backup->add(item, level);
    int size = backup->size();

    unlock();

    /*ItemDrawable *pI = (ItemDrawable*)&item;

    if(((level+1) < m_nLevel)
            && pI->beginIterateChild())
    {
        ItemDrawable *child;

        while((child = pI->nextChild()) != NULL)
            recursiveAddItemDrawable(*child, level+1);
    }*/

    if(size == 1)
    {
        DM_ItemDrawableType idt(t, backup);

        emit listChanged();
        emit added(idt);
    }
}

void DM_ItemDrawableTypeBuilder::recursiveRemoveItemDrawable(const ItemDrawable &item, const int &level)
{
    lock();

    QString t = getTypeForItemDrawable(item);

    DM_ItemDrawableTypeBackup *backup = m_types.value(t, NULL);

    backup->remove(item, level);
    bool isE = backup->isEmpty();

    unlock();

    /*ItemDrawable *pI = (ItemDrawable*)&item;

    if(((level+1) < m_nLevel)
            && pI->beginIterateChild())
    {
        ItemDrawable *child;

        while((child = pI->nextChild()) != NULL)
            recursiveRemoveItemDrawable(*child, level+1);
    }*/

    if(isE)
    {
        lock();
        delete m_types.take(t);
        unlock();

        emit listChanged();
        emit removed(DM_ItemDrawableType(t, NULL));
    }
}

QList<DM_ItemDrawableType> DM_ItemDrawableTypeBuilder::types() const
{
    QMutexLocker locker(m_mutex);

    QList<DM_ItemDrawableType> l;

    QHashIterator<QString, DM_ItemDrawableTypeBackup*> it(m_types);

    while(it.hasNext())
    {
        it.next();

        l.append(DM_ItemDrawableType(it.key(), it.value()));
    }

    return l;
}

bool DM_ItemDrawableTypeBuilder::isEmpty() const
{
    QMutexLocker locker(m_mutex);

    return m_types.isEmpty();
}

DM_ItemDrawableType::DM_ItemDrawableType(const QString &type)
{
    m_type = type;
    m_backup = NULL;
}

DM_ItemDrawableType::DM_ItemDrawableType(const QString &type, const DM_ItemDrawableTypeBackup *backup)
{
    m_type = type;
    m_backup = (DM_ItemDrawableTypeBackup*)backup;
}

const QString &DM_ItemDrawableType::type() const
{
    return m_type;
}

QList<ItemDrawable *> DM_ItemDrawableType::itemDrawableCollection() const
{
    return (m_backup != NULL ? m_backup->m_itemsWithLevel.uniqueKeys() : QList<ItemDrawable*>());
}

bool DM_ItemDrawableType::isEmpty() const
{
    return (m_backup != NULL ? m_backup->m_itemsWithLevel.isEmpty() : true);
}

void DM_ItemDrawableTypeBackup::add(const ItemDrawable &item, const int &level)
{
    m_itemsWithLevel.insert((ItemDrawable*)&item, level);
}

void DM_ItemDrawableTypeBackup::remove(const ItemDrawable &item, const int &level)
{
    m_itemsWithLevel.remove((ItemDrawable*)&item, level);
}

bool DM_ItemDrawableTypeBackup::isEmpty() const
{
    return m_itemsWithLevel.isEmpty();
}

int DM_ItemDrawableTypeBackup::size() const
{
    return m_itemsWithLevel.size();
}
