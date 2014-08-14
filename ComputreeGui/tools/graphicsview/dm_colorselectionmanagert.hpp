#ifndef DM_COLORSELECTIONMANAGERT_HPP
#define DM_COLORSELECTIONMANAGERT_HPP

#include "tools/graphicsview/dm_colorselectionmanagert.h"

#include "dm_guimanager.h"

template<typename T>
DM_ColorSelectionManagerT<T>::DM_ColorSelectionManagerT(RepositoryInterface::SyncCloudWith syncWith)
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
    if((m_selected.data() == NULL)
            && (GUI_MANAGER->getPluginsContext() != NULL))
    {
        m_selected = GUI_MANAGER->getPluginsContext()->repository()->createNewMapIndexCloudColor(m_syncWith);
        m_indexCloudColor = dynamic_cast<IModifiableMapCloudIndexColor*>(m_selected->indexCloud());
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
        QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

        IColorCloud *cc = ccr->colorCloud();

        quint8* col = cc->valueAt(id);

        m_indexCloudColor->insertIndexAndColor(id, col);

        col[0] = 0;
        col[1] = 0;
        col[2] = 255;
        col[3] = 0;
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::addCloudIndexToSelection(const QList<ICloudIndex*> &listID)
{
    if(m_indexCloudColor != NULL)
    {
        QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

        IColorCloud *cc = ccr->colorCloud();

        QListIterator<ICloudIndex*> it(listID);

        size_t globalIndex;

        while(it.hasNext())
        {
            ICloudIndex *ci = it.next();

            size_t size = ci->indexSize();

            for(size_t i=0; i<size; ++i)
            {
                ci->indexAt(i, globalIndex);

                quint8* col = cc->valueAt(globalIndex);

                m_indexCloudColor->insertIndexAndColor(globalIndex, col);

                col[0] = 0;
                col[1] = 0;
                col[2] = 255;
                col[3] = 0;
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
            QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

            if(ccr.data() != NULL)
            {
                IColorCloud *cc = ccr->colorCloud();

                quint8* colBackup = m_indexCloudColor->colorAtGlobalIndex(id, NULL);

                if(colBackup != NULL)
                {
                    quint8* col = cc->valueAt(id);
                    copyColor(colBackup, col);

                    m_indexCloudColor->removeIndex(id);
                }
            }
        }
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::endRemoveMultipleIDFromSelection()
{
    QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        IColorCloud *cc = ccr->colorCloud();

        m_indexCloudColor->copyColorsOfKeys(cc, m_multipleVector, true);
    }

    m_multipleVector.resize(0);
    m_multipleCurrentPos = 0;
}

template<typename T>
void DM_ColorSelectionManagerT<T>::removeCloudIndexFromSelection(const QList<ICloudIndex*> &listID)
{
    QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        IColorCloud *cc = ccr->colorCloud();

        m_indexCloudColor->copyColorsOfCloudIndex(cc, listID, true);
    }
}

template<typename T>
void DM_ColorSelectionManagerT<T>::clearSelection()
{
    QSharedPointer<ColorCloudRegisteredInterface> ccr = createDocumentColorCloudForTypeIfNotExist();

    if(ccr.data() != NULL)
    {
        IColorCloud *cc = ccr->colorCloud();

        m_indexCloudColor->copyColors(cc);
    }

    m_indexCloudColor->clear();
}

template<typename T>
QSharedPointer<IndexCloudRegisteredInterface> DM_ColorSelectionManagerT<T>::selected() const
{
    return m_selected;
}

template<typename T>
QSharedPointer<ColorCloudRegisteredInterface> DM_ColorSelectionManagerT<T>::createDocumentColorCloudForTypeIfNotExist()
{
    if(m_document->colorCloudRegistered<T>().data() == NULL)
        m_document->createColorCloudRegistered<T>();

    return m_document->colorCloudRegistered<T>();
}

#endif // DM_COLORSELECTIONMANAGERT_HPP
