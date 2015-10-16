#ifndef DM_OSGPICKER_H
#define DM_OSGPICKER_H

#include "interfaces.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"
#include "view/DocumentView/GraphicsViews/3D/Picking/dm_globalpointspolytopeintersector.h"

#include <osgViewer/Viewer>

class GOsgGraphicsView;
class DM_OsgSceneManager;
class DM_DocumentView;

/**
 * @brief Use this class to make picking in a osg view
 */
class DM_OsgPicker : public QObject
{
    Q_OBJECT

public:

    typedef osg::ByteArray                          LocalVertexAttribArray;
    typedef GraphicsViewInterface::SelectionMode    SelectionMode;

    DM_OsgPicker(GOsgGraphicsView *view, osgViewer::View *osgView, DM_OsgSceneManager *sceneManager);

    /**
     * @brief Set the selection width. Default is 3;
     */
    void setSelectRegionWidth(int width);

    /**
     * @brief Set the selection height. Default is 3;
     */
    void setSelectRegionHeight(int height);

    /**
     * @brief Set the polytope to use to select element in view
     */
    void setSelectPolytope(const osg::Polytope &polytope);

    /**
     * @brief Get the selection width. Default is 3;
     */
    int selectRegionWidth() const;

    /**
     * @brief Get the selection height. Default is 3;
     */
    int selectRegionHeight() const;

    /**
     * @brief Set the selection mode
     */
    void setSelectionMode(SelectionMode mode);

    /**
     * @brief Get the selection mode
     */
    SelectionMode selectionMode() const;

    /**
     * @brief Convert the selection mode to a basic type (SELECT, ADD, REMOVE, SELECT_ONE, ADD_ONE or REMOVE_ONE)
     */
    SelectionMode selectionModeToBasic() const;

    /**
     * @brief Returns true if the view must select points
     */
    bool mustSelectPoints() const;

    /**
     * @brief Returns true if the view must select edges
     */
    bool mustSelectEdges() const;

    /**
     * @brief Returns true if the view must select faces
     */
    bool mustSelectFaces() const;

    /**
     * @brief Returns true if the view must select items
     */
    bool mustSelectItems() const;

    /**
     * @brief Performs a selection in the scene from pixel coordinates.
     * @warning the y position must be inversed (not use QT coordinate system) : if the point was at the top of the view it must have
     *          the "height" value of the view in "y", and 0 if it was at the bottom of the view. X is from left to right (0 to width).
     */
    void select(const QPoint& pointYInversed);

    CT_CIR getSelectedPoints();

    /**
     * @brief Returns the bounding box of all global points selected. Returns false if no point is selected.
     */
    bool getBoundingBoxOfAllPointSelectedInScene(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    void addItemIDToSelection(const GLuint &id);
    void addItemToSelection(CT_AbstractItemDrawable *item);

    void removeItemIDFromSelection(const GLuint &id);
    void removeItemFromSelection(CT_AbstractItemDrawable *item);

    void addPointIdToSelection(const size_t &globalIndex);
    void addPointsIdToSelection(const CT_AbstractCloudIndex *indexes);
    void addCloudPointsIdToSelection(const QList<CT_AbstractCloudIndex*> &indexes);

    void removePointIdFromSelection(const size_t &globalIndex);
    void removePointsIdFromSelection(const CT_AbstractCloudIndex *indexes);
    void removeCloudPointsIdFromSelection(const QList<CT_AbstractCloudIndex*> &indexes);

    void setAllPointsSelected(bool selected);

    void addFaceIdToSelection(const size_t &globalIndex);
    void removeFaceIdFromSelection(const size_t &globalIndex);
    void setAllFacesSelected(bool selected);

    void addEdgeIdToSelection(const size_t &globalIndex);
    void removeEdgeIdFromSelection(const size_t &globalIndex);
    void setAllEdgesSelected(bool selected);

    void removeAllIdFromSelection();

private:

    typedef GDocumentViewForGraphics::AttribCloudType::AType GlobalVertexAttribArrayType;

    GOsgGraphicsView                            *m_view;
    osg::ref_ptr<osgViewer::View>               m_osgView;
    osg::ref_ptr<GlobalVertexAttribArrayType>   m_globalVertexAttribArray;
    osg::Polytope                               m_polytope;
    bool                                        m_usePolytope;

    SelectionMode                   m_selectionMode;
    int                             m_regionWidth;
    int                             m_regionHeight;
    DM_OsgSceneManager              *m_sceneManager;

    CT_CIR                          m_selectedPointsBackup;
    mutable bool                    m_selectionPointsChanged;

    CT_FaceAccessor                 m_fAccess;
    CT_EdgeAccessor                 m_eAccess;

    DM_DocumentView* getDocumentView() const;

    osg::Polytope* getPolytopeToUse() const;

    void setGlobalCloudPointsOfItemDrawableSelected(CT_AbstractItemDrawable *item, bool selected);
    void setVertexAttribArrayOfItemDrawableSelected(CT_AbstractItemDrawable *item, bool selected);
    void setVertexAttribArrayOfItemDrawableSelected(osg::Group *result, bool selected);

    void selectItemsInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope);
    void selectPointsInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope);
    void selectEdgesInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope);
    void selectFacesInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope);
    void selectPEFInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope, DM_GlobalPointsPolytopeIntersector::ElementFilter filter);

    CT_AbstractModifiablePointCloudIndex *convertSelectedPointsToIndexCloud() const;

private slots:

    /**
     * @brief Called when an itemdrawable was added to the GOsgGraphicsView
     */
    void itemAddedToView(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when an itemdrawable was removed from the GOsgGraphicsView
     */
    void itemRemovedFromView(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when an item emit signal "selectChange"
     */
    void itemSelectionChanged(bool selected, CT_AbstractItemDrawable *item);
};

#endif // DM_OSGPICKER_H
