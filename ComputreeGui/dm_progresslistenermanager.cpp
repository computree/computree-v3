#include "dm_progresslistenermanager.h"

DM_ProgressListenerManager::DM_ProgressListenerManager()
{
}

void DM_ProgressListenerManager::addListener(const DM_IProgressListener *l)
{
    m_collection.append((DM_IProgressListener*)l);
}

void DM_ProgressListenerManager::removeListener(const DM_IProgressListener *l)
{
    m_collection.removeOne((DM_IProgressListener*)l);
}

void DM_ProgressListenerManager::setProgressRange(const int &min, const int &max)
{
    QListIterator<DM_IProgressListener*> it(m_collection);

    while(it.hasNext())
        it.next()->setProgressRange(min, max);
}

void DM_ProgressListenerManager::setProgress(const int &progress)
{
    QListIterator<DM_IProgressListener*> it(m_collection);

    while(it.hasNext())
        it.next()->setProgress(progress);
}
