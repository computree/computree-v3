#include "dm_itemdrawableviewconfigurationbuilder.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

DM_ItemDrawableViewConfigurationBuilder::DM_ItemDrawableViewConfigurationBuilder(bool searchInChildren)
{
    m_searchInChildren = searchInChildren;
}

DM_ItemDrawableViewConfigurationBuilder::~DM_ItemDrawableViewConfigurationBuilder()
{
    qDeleteAll(m_config.begin(), m_config.end());
}

DM_ItemDrawableViewConfiguration DM_ItemDrawableViewConfigurationBuilder::getConfigurationWithName(const QString &name) const
{
    QMutexLocker((QMutex*)&m_mutex);

    QMapIterator<CT_ItemDrawableConfiguration*, QList<CT_AbstractItemDrawable*>* > it(m_config);

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

    int value = 0;

    if(t != NULL)
    {
        value = addItemDrawableConfigurationToCollection(t, item);

        if(value == 1)
            addL.append(DM_ItemDrawableViewConfiguration(t));
    }

    if(m_searchInChildren)
    {
        QList<CT_ItemDrawableConfiguration*> dependantViewConfigurations = pItem->getDependantViewConfigurations();

        int size = dependantViewConfigurations.size();

        for(int i=0; i<size; ++i)
        {
            t = dependantViewConfigurations.at(i);

            if(t != NULL)
            {
                value = addItemDrawableConfigurationToCollection(t, item);

                if(value == 1)
                    addL.append(DM_ItemDrawableViewConfiguration(t));
            }
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

    int value = 0;

    if(t != NULL)
    {
        value = removeItemDrawableConfigurationFromCollection(t, item);

        if(value == 0)
            rL.append(DM_ItemDrawableViewConfiguration(t));
    }

    if(m_searchInChildren)
    {
        QList<CT_ItemDrawableConfiguration*> dependantViewConfigurations = pItem->getDependantViewConfigurations();

        int size = dependantViewConfigurations.size();

        for(int i=0; i<size; ++i)
        {
            t = dependantViewConfigurations.at(i);

            if(t != NULL)
            {
                value = removeItemDrawableConfigurationFromCollection(t, item);

                if(value == 0)
                    rL.append(DM_ItemDrawableViewConfiguration(t));
            }
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

int DM_ItemDrawableViewConfigurationBuilder::addItemDrawableConfigurationToCollection(CT_ItemDrawableConfiguration *t, CT_AbstractItemDrawable &item)
{
    QList<CT_AbstractItemDrawable*> *list = m_config.value(t, NULL);

    if(list == NULL) {
        list = new QList<CT_AbstractItemDrawable*>();
        m_config.insert(t, list);
    }

    list->append(&item);

    return list->size();
}

int DM_ItemDrawableViewConfigurationBuilder::removeItemDrawableConfigurationFromCollection(CT_ItemDrawableConfiguration *t, CT_AbstractItemDrawable &item)
{
    QList<CT_AbstractItemDrawable*> *list = m_config.value(t, NULL);

    if(list == NULL)
        return 0;

    list->removeOne(&item);

    if(list->empty()) {
        m_config.remove(t);
        delete list;
        return 0;
    }

    return list->size();
}

QList<DM_ItemDrawableViewConfiguration> DM_ItemDrawableViewConfigurationBuilder::configurations() const
{
    QMutexLocker locker((QMutex*)&m_mutex);

    QList<DM_ItemDrawableViewConfiguration> l;

    QMapIterator<CT_ItemDrawableConfiguration*, QList<CT_AbstractItemDrawable*>* > it(m_config);

    while(it.hasNext())
    {
        it.next();

        if(it.value() != 0)
            l.append(DM_ItemDrawableViewConfiguration(it.key()));
    }

    return l;
}

QList<CT_AbstractItemDrawable *> DM_ItemDrawableViewConfigurationBuilder::itemDrawablesForConfiguration(CT_ItemDrawableConfiguration *config) const
{
    QList<CT_AbstractItemDrawable *> *l = m_config.value(config, NULL);

    if(l == NULL)
        return QList<CT_AbstractItemDrawable *>();

    return *l;
}

DM_ItemDrawableViewConfiguration::DM_ItemDrawableViewConfiguration(const CT_ItemDrawableConfiguration *config)
{
    m_config = (CT_ItemDrawableConfiguration*)config;
}

CT_ItemDrawableConfiguration *DM_ItemDrawableViewConfiguration::configuration() const
{
    return m_config;
}
