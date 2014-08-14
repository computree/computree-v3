#include "ct_abstractcloudsynctoglobalcloudmanager.h"

#include "ct_global/ct_context.h"

CT_AbstractCloudSyncToGlobalCloudManager::CT_AbstractCloudSyncToGlobalCloudManager()
{
    m_syncProgress = 0;
}

CT_AbstractCloudSyncToGlobalCloudManager::~CT_AbstractCloudSyncToGlobalCloudManager()
{
    while(!m_crArray.isEmpty())
        m_crArray.takeLast()->setSyncCloudManager(NULL);
}

void CT_AbstractCloudSyncToGlobalCloudManager::setSyncProgress(int progress)
{
    if(progress != m_syncProgress)
    {
        m_syncProgress = progress;
        emit syncProgressChanged(progress);
    }
}

void CT_AbstractCloudSyncToGlobalCloudManager::cloudDeleted(const size_t &beginIndex, const size_t &size)
{
    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->lockAllDocuments();

    QMutexLocker locker(&m_mutex);
    emit beginSync();

    setSyncProgress(0);

    int iSync = 0;
    float syncDividor = 1;

    if(!m_crArray.isEmpty())
        syncDividor = 100.0/m_crArray.size();

    QListIterator<CT_AbstractCloudRegistered*> it(m_crArray);

    while(it.hasNext())
    {
        CT_AbstractCloudRegistered *cr = it.next();

        cr->abstractCloud()->erase(beginIndex, size);

        ++iSync;
        setSyncProgress(iSync*syncDividor);
    }

    setSyncProgress(100);

    emit endSync();

    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->unlockAllDocuments();
}

void CT_AbstractCloudSyncToGlobalCloudManager::cloudAdded(const size_t &size)
{
    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->lockAllDocuments();

    QMutexLocker locker(&m_mutex);
    emit beginSync();

    setSyncProgress(0);

    int iSync = 0;
    float syncDividor = 1;

    if(!m_crArray.isEmpty())
        syncDividor = 100.0/m_crArray.size();

    QListIterator<CT_AbstractCloudRegistered*> it(m_crArray);

    while(it.hasNext())
    {
        CT_AbstractCloudRegistered *cr = it.next();

        cr->abstractCloud()->resize(cr->abstractCloud()->size() + size);

        ++iSync;
        setSyncProgress(iSync*syncDividor);
    }

    setSyncProgress(100);

    emit endSync();

    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->unlockAllDocuments();
}

void CT_AbstractCloudSyncToGlobalCloudManager::internalRegisterCloud(CT_AbstractCloudRegistered *cr)
{
    cr->setSyncCloudManager(this);
    m_crArray.append(cr);
}

bool CT_AbstractCloudSyncToGlobalCloudManager::internalUnregisterCloud(const CT_AbstractCloudRegistered *cr)
{
    QMutexLocker locker(&m_mutex);

    return m_crArray.removeOne((CT_AbstractCloudRegistered*)cr);
}
