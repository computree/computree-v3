#include "dm_itemdrawableviewconfigurationbuilder.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

DM_ItemDrawableViewConfigurationBuilder::DM_ItemDrawableViewConfigurationBuilder(bool searchInChildren)
{
    m_searchInChildren = searchInChildren;
}

DM_ItemDrawableViewConfiguration DM_ItemDrawableViewConfigurationBuilder::getConfigurationWithName(const QString &name) const
{
    QMutexLocker((QMutex*)&m_mutex);

    QMapIterator<CT_ItemDrawableConfiguration*, size_t> it(m_config);

    while(it.hasNext())
    {
        it.next();

        if(it.key()->name() == name)
            return DM_ItemDrawableViewConfiguration(it.key());
    }

    return DM_ItemDrawableViewConfiguration(NULL);
}

void DM_ItemDrawableViewConfigurationBuilder::addItemDrawable(CT_AbstractItemDrawable &item)
{
    m_mutex.lock();

    CT_AbstractItemDrawable *pItem = (CT_AbstractItemDrawable*)&item;

    QList<DM_ItemDrawableViewConfiguration> addL;

    CT_ItemDrawableConfiguration *t = pItem->getViewConfiguration();
    size_t value = m_config.value(t, 0)+1;

    m_config.insert(t, value);

    if(value == 1)
        addL.append(DM_ItemDrawableViewConfiguration(t));

    if(m_searchInChildren)
    {
        QList<CT_ItemDrawableConfiguration*> liste = pItem->getDependantViewConfigurations();

        int size = liste.size();

        for(int i=0; i<size; ++i)
        {
            t = liste.at(i);
            value = m_config.value(t, 0)+1;
            m_config.insert(t, value);

            if(value == 1)
                addL.append(DM_ItemDrawableViewConfiguration(t));
        }
    }

    m_mutex.unlock();

    if(!addL.isEmpty())
    {
        emit listChanged();

        QListIterator<DM_ItemDrawableViewConfiguration> it(addL);

        while(it.hasNext())
            emit added(it.next());
    }
}

void DM_ItemDrawableViewConfigurationBuilder::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    m_mutex.lock();

    CT_AbstractItemDrawable *pItem = (CT_AbstractItemDrawable*)&item;

    QList<DM_ItemDrawableViewConfiguration> rL;

    CT_ItemDrawableConfiguration *t = pItem->getViewConfiguration();
    size_t value = m_config.value(t, 0)-1;

    m_config.insert(t, value);

    if(value == 0)
        rL.append(DM_ItemDrawableViewConfiguration(t));

    if(m_searchInChildren)
    {
        QList<CT_ItemDrawableConfiguration*> liste = pItem->getDependantViewConfigurations();

        int size = liste.size();

        for(int i=0; i<size; ++i)
        {
            t = liste.at(i);

            value = m_config.value(t, 0)-1;
            m_config.insert(t, value);

            if(value == 0)
                rL.append(DM_ItemDrawableViewConfiguration(t));
        }
    }

    m_mutex.unlock();

    if(!rL.isEmpty())
    {
        emit listChanged();

        QListIterator<DM_ItemDrawableViewConfiguration> it(rL);

        while(it.hasNext())
            emit removed(it.next());
    }
}

void DM_ItemDrawableViewConfigurationBuilder::clear()
{
    m_config.clear();

    emit listChanged();
}

QList<DM_ItemDrawableViewConfiguration> DM_ItemDrawableViewConfigurationBuilder::configurations() const
{
    QMutexLocker locker((QMutex*)&m_mutex);

    QList<DM_ItemDrawableViewConfiguration> l;

    QMapIterator<CT_ItemDrawableConfiguration*, size_t> it(m_config);

    while(it.hasNext())
    {
        it.next();

        if(it.value() != 0)
            l.append(DM_ItemDrawableViewConfiguration(it.key()));
    }

    return l;
}

DM_ItemDrawableViewConfiguration::DM_ItemDrawableViewConfiguration(const CT_ItemDrawableConfiguration *config)
{
    m_config = (CT_ItemDrawableConfiguration*)config;
}

CT_ItemDrawableConfiguration *DM_ItemDrawableViewConfiguration::configuration() const
{
    return m_config;
}
