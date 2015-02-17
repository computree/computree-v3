#include "dm_elementinfomanager.h"

#include "ct_global/ct_context.h"
#include "ct_cloudindex/ct_cloudindexstdvectort.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"
#include "ct_point.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include <limits>

DM_ElementInfoManager::DM_ElementInfoManager(CT_Repository::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;

    m_enableSelection = 1;
    m_enableInvisibility = 2;

    m_disableSelection = std::numeric_limits<GLubyte>::max()-m_enableSelection;
    m_disableInvisibility = std::numeric_limits<GLubyte>::max()-m_enableInvisibility;

    m_infoR = PS_REPOSITORY->createNewCloudT<CT_StdCloudRegisteredT< CT_StandardCloudStdVectorT<GLubyte> >, CT_StandardCloudStdVectorT<GLubyte> >(m_syncWith);
    m_infoCloud = NULL;

    if(!m_infoR.isNull())
        m_infoCloud = m_infoR->cloudT();

    m_selectionChanged = false;
}

void DM_ElementInfoManager::addIDToSelection(const size_t &id)
{
    if(m_infoCloud != NULL) {
        GLubyte &info = m_infoCloud->tAt(id);
        info |= m_enableSelection;
        m_selectionChanged = true;
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

            if(size > 0)
                m_selectionChanged = true;

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
        m_selectionChanged = true;
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

            if(size > 0)
                m_selectionChanged = true;

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

        if(size > 0)
            m_selectionChanged = true;

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

CT_CIR DM_ElementInfoManager::selected()
{
    if(m_selectionChanged) {
        if(m_syncWith == CT_Repository::SyncWithPointCloud)
            m_selectedBackup = PS_REPOSITORY->registerPointCloudIndex(selectedT<CT_PointData>());
        else if(m_syncWith == CT_Repository::SyncWithEdgeCloud)
            m_selectedBackup = PS_REPOSITORY->registerEdgeCloudIndex(selectedT<CT_Edge>());
        else if(m_syncWith == CT_Repository::SyncWithFaceCloud)
            m_selectedBackup = PS_REPOSITORY->registerFaceCloudIndex(selectedT<CT_Face>());
    }

    return m_selectedBackup;
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
