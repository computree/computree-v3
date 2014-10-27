#include "dm_elementinfomanager.h"

#include "ct_global/ct_context.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"

#include <limits>

DM_ElementInfoManager::DM_ElementInfoManager(CT_Repository::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;

    m_enableSelection = 1;
    m_enableInvisibility = 2;

    m_disableSelection = std::numeric_limits<GLubyte>::max()-m_enableSelection;
    m_disableInvisibility = std::numeric_limits<GLubyte>::max()-m_enableInvisibility;

    m_infoR = PS_REPOSITORY->createNewSyncCloudT<CT_StdCloudRegisteredT<GLubyte>, CT_StandardCloudStdVectorT<GLubyte> >(m_syncWith);
    m_infoCloud = dynamic_cast<CT_StandardCloudStdVectorT<GLubyte>*>(m_infoR->abstractCloud());
}

void DM_ElementInfoManager::addIDToSelection(const size_t &id)
{
    if(m_infoCloud != NULL) {
        GLubyte &info = m_infoCloud->tAt(id);
        info |= m_enableSelection;
    }
}

void DM_ElementInfoManager::addCloudIndexToSelection(const QList<CT_AbstractCloudIndex*> &listID)
{
    if(m_infoCloud != NULL) {
        QListIterator<CT_AbstractCloudIndex*> it(listID);

        size_t globalIndex;

        while(it.hasNext())
        {
            CT_AbstractCloudIndex *ci = it.next();

            size_t size = ci->size();

            for(size_t i=0; i<size; ++i)
            {
                ci->indexAt(i, globalIndex);

                GLubyte &info = m_infoCloud->tAt(globalIndex);
                info |= m_enableSelection;
            }
        }
    }
}

void DM_ElementInfoManager::removeIDFromSelection(const size_t &id)
{
    if(m_infoCloud != NULL) {
        GLubyte &info = m_infoCloud->tAt(id);
        info &= m_disableSelection;
    }
}

void DM_ElementInfoManager::removeCloudIndexFromSelection(const QList<CT_AbstractCloudIndex*> &listID)
{
    if(m_infoCloud != NULL) {
        QListIterator<CT_AbstractCloudIndex*> it(listID);

        size_t globalIndex;

        while(it.hasNext())
        {
            CT_AbstractCloudIndex *ci = it.next();

            size_t size = ci->size();

            for(size_t i=0; i<size; ++i)
            {
                ci->indexAt(i, globalIndex);

                GLubyte &info = m_infoCloud->tAt(globalIndex);
                info &= m_disableSelection;
            }
        }
    }
}

void DM_ElementInfoManager::addIDToInvisibility(const size_t &id)
{
    if(m_infoCloud != NULL) {
        GLubyte &info = m_infoCloud->tAt(id);
        info |= m_enableInvisibility;
    }
}

void DM_ElementInfoManager::removeIDFromInvisibility(const size_t &id)
{
    if(m_infoCloud != NULL) {
        GLubyte &info = m_infoCloud->tAt(id);
        info &= m_disableInvisibility;
    }
}

void DM_ElementInfoManager::clearSelection()
{
    if(m_infoCloud != NULL) {
        size_t size = m_infoCloud->size();

        for(size_t i=0; i<size; ++i) {
            GLubyte &info = m_infoCloud->tAt(i);
            info &= m_disableSelection;
        }
    }
}

void DM_ElementInfoManager::setAllSelectedInvisible()
{
    if(m_infoCloud != NULL) {
        size_t size = m_infoCloud->size();

        for(size_t i=0; i<size; ++i) {
            GLubyte &info = m_infoCloud->tAt(i);

            if(info & m_enableSelection)
                info |= m_enableInvisibility;
        }
    }
}

void DM_ElementInfoManager::setAllElementVisible()
{
    if(m_infoCloud != NULL) {
        size_t size = m_infoCloud->size();

        for(size_t i=0; i<size; ++i) {
            GLubyte &info = m_infoCloud->tAt(i);
            info &= m_disableInvisibility;
        }
    }
}

CT_StandardCloudStdVectorT<GLubyte>* DM_ElementInfoManager::informations() const
{
    return m_infoCloud;
}

GLubyte DM_ElementInfoManager::checkSelected() const
{
    return m_enableSelection;
}

GLubyte DM_ElementInfoManager::checkInvisible() const
{
    return m_enableInvisibility;
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> DM_ElementInfoManager::selected() const
{
    if(m_syncWith == CT_Repository::SyncWithPointCloud)
        return PS_REPOSITORY->registerPointCloudIndex(selectedT<CT_Point>());
    else if(m_syncWith == CT_Repository::SyncWithEdgeCloud)
        return PS_REPOSITORY->registerEdgeCloudIndex(selectedT<CT_Edge>());
    else if(m_syncWith == CT_Repository::SyncWithFaceCloud)
        return PS_REPOSITORY->registerFaceCloudIndex(selectedT<CT_Face>());

    return QSharedPointer<CT_AbstractModifiableCloudIndexRegistered>(NULL);
}

template<typename T>
CT_AbstractModifiableCloudIndexT<T>* DM_ElementInfoManager::selectedT() const
{
    CT_CloudIndexStdVectorT<T> *sel = new CT_CloudIndexStdVectorT<T>();

    size_t size = m_infoCloud->size();

    for(size_t i=0; i<size; ++i) {
        GLubyte &info = m_infoCloud->tAt(i);

        if(info & m_enableSelection)
            sel->addIndex(i);
    }

    if(sel->size() != 0)
        return sel;

    delete sel;

    return NULL;
}
