#ifndef DM_COLORSELECTIONMANAGERT_HPP
#define DM_COLORSELECTIONMANAGERT_HPP

#include "tools/graphicsview/dm_colorselectionmanagert.h"

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

template<typename T>
DM_ColorSelectionManagerT<T>::DM_ColorSelectionManagerT(CT_Repository::SyncCloudWith syncWith)
{
    m_document = NULL;
    m_indexCloudColor = NULL;
    m_syncWith = syncWith;
    m_multipleCurrentPos = 0;
}

template<typename T>
DM_ColorSelectionManagerT<T>::~DM_ColorSelectionManagerT()
{
}

template<typename T>
void DM_ColorSelectionManagerT<T>::init()
{
    if(m_selected.data() == NULL)
    {
        m_selected = PS_REPOSITORY->createNewMapIndexCloudColor(m_syncWith);
        m_indexCloudColor = dynamic_cast<CT_AbstractModifiableIndexCloudColorMap*>(m_selected->abstractModifiableCloudIndex());
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::setDocument(const GDocumentViewForGraphics *doc)
{
    m_document = (GDocumentViewForGraphics*)doc;
}

template<typename T>
void DM_ColorSelectionManagerT<T>::addIDToSelection(const GLuint &id)
{
    if(m_indexCloudColor != NULL)
    {
        QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

        CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

        CT_Color &col = cc->colorAt(id);

        m_indexCloudColor->insertIndexAndColor(id, col);

        col.b = 0;
        col.g = 0;
        col.r = 255;
        col.a = 0;
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::addCloudIndexToSelection(const QList<CT_AbstractCloudIndex*> &listID)
{
    if(m_indexCloudColor != NULL)
    {
        QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

        CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

        QListIterator<CT_AbstractCloudIndex*> it(listID);

        size_t globalIndex;

        while(it.hasNext())
        {
            CT_AbstractCloudIndex *ci = it.next();

            size_t size = ci->size();

            for(size_t i=0; i<size; ++i)
            {
                ci->indexAt(i, globalIndex);

                CT_Color &col = cc->colorAt(globalIndex);

                m_indexCloudColor->insertIndexAndColor(globalIndex, col);

                col.b = 0;
                col.g = 0;
                col.r = 255;
                col.a = 0;
            }
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::beginRemoveMultipleIDFromSelection(const size_t &n)
{
    m_multipleVector.resize(n);
    m_multipleCurrentPos = 0;
}

template<typename T>
void DM_ColorSelectionManagerT<T>::removeIDFromSelection(const GLuint &id)
{
    if(m_indexCloudColor != NULL)
    {
        if(!m_multipleVector.empty())
        {
            m_multipleVector[m_multipleCurrentPos] = id;
            ++m_multipleCurrentPos;
        }
        else
        {
            QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

            if(ccr.data() != NULL)
            {
                CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

                CT_Color *colBackup = m_indexCloudColor->colorAtGlobalIndex(id, NULL);

                if(colBackup != NULL)
                {
                    CT_Color &col = cc->colorAt(id);
                    col.setColor(*colBackup);

                    m_indexCloudColor->removeIndex(id);
                }
            }
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::endRemoveMultipleIDFromSelection()
{
    QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

        m_indexCloudColor->copyColorsOfKeys(cc, m_multipleVector, true);
    }

    m_multipleVector.resize(0);
    m_multipleCurrentPos = 0;
}

template<typename T>
void DM_ColorSelectionManagerT<T>::removeCloudIndexFromSelection(const QList<CT_AbstractCloudIndex*> &listID)
{
    QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

        m_indexCloudColor->copyColorsOfCloudIndex(cc, listID, true);
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::clearSelection()
{
    QSharedPointer<CT_StandardColorCloudRegistered> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        CT_AbstractColorCloud *cc = ccr->abstractColorCloud();

        m_indexCloudColor->copyColors(cc);
    }

    m_indexCloudColor->clear();
}

template<typename T>
QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> DM_ColorSelectionManagerT<T>::selected() const
{
    return m_selected;
}

template<typename T>
QSharedPointer<CT_StandardColorCloudRegistered> DM_ColorSelectionManagerT<T>::createDocumentColorCloudForTypeIfNotExist()
{
    if(m_document->colorCloudRegistered<T>().data() == NULL)
        m_document->createColorCloudRegistered<T>();

    return m_document->colorCloudRegistered<T>();
}

#endif // DM_COLORSELECTIONMANAGERT_HPP
