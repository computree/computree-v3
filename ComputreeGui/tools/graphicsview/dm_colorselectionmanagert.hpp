#ifndef DM_COLORSELECTIONMANAGERT_HPP
#define DM_COLORSELECTIONMANAGERT_HPP

#include "tools/graphicsview/dm_colorselectionmanagert.h"

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

template<typename T>
DM_ColorSelectionManagerT<T>::DM_ColorSelectionManagerT(CT_Repository::SyncCloudWith syncWith)
{
    m_infoCloud = NULL;
    m_syncWith = syncWith;

    m_enableSelection = 1;
    m_disableSelection = std::numeric_limits<GLuint>::max()-m_enableSelection;

    m_enableInvisibility = 2;
    m_disableInvisibility = std::numeric_limits<GLuint>::max()-m_enableInvisibility;
}

template<typename T>
DM_ColorSelectionManagerT<T>::~DM_ColorSelectionManagerT()
{
}

template<typename T>
void DM_ColorSelectionManagerT<T>::init()
{
    if(m_infoR.isNull()){
        m_infoR = PS_REPOSITORY->createNewSyncCloudT<CT_StdCloudRegisteredT<GLuint>, CT_StandardCloudStdVectorT<GLuint> >(m_syncWith);
        m_infoCloud = dynamic_cast<CT_StandardCloudStdVectorT<GLuint>*>(m_infoR->abstractCloud());
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::setDocument(const GDocumentViewForGraphics *doc)
{
}

template<typename T>
void DM_ColorSelectionManagerT<T>::addIDToSelection(const GLuint &id)
{
    if(m_infoCloud != NULL) {
        GLuint &info = m_infoCloud->tAt(id);
        info |= m_enableSelection;
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::addCloudIndexToSelection(const QList<CT_AbstractCloudIndex*> &listID)
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

                GLuint &info = m_infoCloud->tAt(globalIndex);
                info |= m_enableSelection;
            }
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::beginRemoveMultipleIDFromSelection(const size_t &n)
{
}

template<typename T>
void DM_ColorSelectionManagerT<T>::removeIDFromSelection(const GLuint &id)
{
    if(m_infoCloud != NULL) {
        GLuint &info = m_infoCloud->tAt(id);
        info &= m_disableSelection;
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::endRemoveMultipleIDFromSelection()
{
}

template<typename T>
void DM_ColorSelectionManagerT<T>::removeCloudIndexFromSelection(const QList<CT_AbstractCloudIndex*> &listID)
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

                GLuint &info = m_infoCloud->tAt(globalIndex);
                info &= m_disableSelection;
            }
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::clearSelection()
{
    if(m_infoCloud != NULL) {
        size_t size = m_infoCloud->size();

        for(size_t i=0; i<size; ++i) {
            GLuint &info = m_infoCloud->tAt(i);
            info &= m_disableSelection;
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::setAllSelectedInvisible()
{
    if(m_infoCloud != NULL) {
        size_t size = m_infoCloud->size();

        for(size_t i=0; i<size; ++i) {
            GLuint &info = m_infoCloud->tAt(i);

            if(info & m_enableSelection)
                info |= m_enableInvisibility;
        }
    }
}

template<typename T>
CT_StandardCloudStdVectorT<GLuint>* DM_ColorSelectionManagerT<T>::informations() const
{
    return m_infoCloud;
}

template<typename T>
GLuint DM_ColorSelectionManagerT<T>::checkSelected() const
{
    return m_enableSelection;
}

template<typename T>
GLuint DM_ColorSelectionManagerT<T>::checkInvisible() const
{
    return m_enableInvisibility;
}

template<typename T>
QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> DM_ColorSelectionManagerT<T>::selected() const
{
    return QSharedPointer<CT_AbstractModifiableCloudIndexRegistered>(NULL);
}

#endif // DM_COLORSELECTIONMANAGERT_HPP
