#ifndef DM_COLORSELECTIONMANAGERT_H
#define DM_COLORSELECTIONMANAGERT_H

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "ct_colorcloud/abstract/ct_abstractmodifiableindexcloudcolormap.h"

/**
 * Class that manage colors for selected TYPE (points, faces, edges, etc...) in a graphics view
 *
 * T must be CT_AbstractPointsAttributes or CT_AbstractFaceAttributes, etc...
 */
template<typename T>
class DM_ColorSelectionManagerT
{
public:
    /**
     * @brief Set in the constructor with which TYPE we must sync (CT_Repository::SyncWithPointCloud, CT_Repository::SyncWithFaceCloud, etc..)
     */
    DM_ColorSelectionManagerT(CT_Repository::SyncCloudWith syncWith);
    ~DM_ColorSelectionManagerT();

    /**
     * @brief Call this method when you know that a plugin is loaded to create the cloud index that will be synchronized
     */
    void init();

    /**
     * @brief Set the document. The document is used to get the color cloud of TYPE.
     */
    void setDocument(const GDocumentViewForGraphics *doc);

    /**
     * @brief Call this method when you add a TYPE to the selection
     */
    void addIDToSelection(const GLuint &id);

    /**
     * @brief Call this method when you want to add a list of index to the selection
     */
    void addCloudIndexToSelection(const QList<CT_AbstractCloudIndex*> &listID);

    /**
     * @brief Call when you must remove a lot of ID
     */
    void beginRemoveMultipleIDFromSelection(const size_t &n);

    /**
     * @brief Call this method when you remove a TYPE from the selection
     */
    void removeIDFromSelection(const GLuint &id);

    /**
     * @brief Call when you have finished to remove a lot of ID
     */
    void endRemoveMultipleIDFromSelection();

    /**
     * @brief Call this method when you want to remove a list of index from the selection
     */
    void removeCloudIndexFromSelection(const QList<CT_AbstractCloudIndex*> &listID);

    /**
     * @brief Call this method when you want to clear selection
     */
    void clearSelection();

    /**
     * @brief Returns the index cloud of selected TYPE
     */
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> selected() const;

private:
    CT_Repository::SyncCloudWith                                m_syncWith;
    GDocumentViewForGraphics                                    *m_document;
    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered>   m_selected;
    CT_AbstractModifiableIndexCloudColorMap                     *m_indexCloudColor;
    std::vector<size_t>                                         m_multipleVector;
    size_t                                                      m_multipleCurrentPos;

    QSharedPointer<CT_StandardColorCloudRegistered> createDocumentColorCloudForTypeIfNotExist();
};

#include "tools/graphicsview/dm_colorselectionmanagert.hpp"

#endif // DM_COLORSELECTIONMANAGERT_H
