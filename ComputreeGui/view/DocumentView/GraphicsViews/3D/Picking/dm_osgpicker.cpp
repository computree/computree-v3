#include "dm_osgpicker.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_cloudindex/ct_cloudindexstdvectort.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"

#include "view/DocumentView/GraphicsViews/3D/dm_osgscenemanager.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_fakepainter.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"
#include "view/DocumentView/GraphicsViews/3D/Picking/dm_itemdrawablepolytopeintersector.h"
#include "view/DocumentView/GraphicsViews/3D/Converter/dm_multipleitemdrawabletoosgworker.h"

#include <QDebug>

#define ENABLE_SELECTION 1
#define DISABLE_SELECTION 0

DM_OsgPicker::DM_OsgPicker(GOsgGraphicsView *view, osgViewer::View *osgView, DM_OsgSceneManager *sceneManager)
{
    m_view = view;
    m_osgView = osgView;
    m_sceneManager = sceneManager;

    m_selectionPointsChanged = false;

    if(getDocumentView() != NULL)
        m_globalVertexAttribArray = ((GDocumentViewForGraphics*)getDocumentView())->getOrCreateGlobalAttribArrayForPoints();

    connect(m_view, SIGNAL(itemDrawableAdded(CT_AbstractItemDrawable&)), this, SLOT(itemAddedToView(CT_AbstractItemDrawable&)), Qt::DirectConnection);
    connect(m_view, SIGNAL(itemDrawableRemoved(CT_AbstractItemDrawable&)), this, SLOT(itemRemovedFromView(CT_AbstractItemDrawable&)), Qt::DirectConnection);

    setSelectionMode(GraphicsViewInterface::NONE);
    setSelectRegionWidth(3);
    setSelectRegionHeight(3);
}

void DM_OsgPicker::setSelectRegionWidth(int width)
{
    if(width <= 0)
        width = 1;

    m_regionWidth = width;
    m_usePolytope = false;
}

void DM_OsgPicker::setSelectRegionHeight(int height)
{
    if(height <= 0)
        height = 1;

    m_regionHeight = height;
    m_usePolytope = false;
}

void DM_OsgPicker::setSelectPolytope(const osg::Polytope &polytope)
{
    m_polytope = polytope;
    m_usePolytope = true;
}

int DM_OsgPicker::selectRegionHeight() const
{
    return m_regionHeight;
}

int DM_OsgPicker::selectRegionWidth() const
{
    return m_regionWidth;
}

void DM_OsgPicker::setSelectionMode(DM_OsgPicker::SelectionMode mode)
{
    m_selectionMode = mode;
}

DM_OsgPicker::SelectionMode DM_OsgPicker::selectionMode() const
{
    return m_selectionMode;
}

DM_OsgPicker::SelectionMode DM_OsgPicker::selectionModeToBasic() const
{
    int m = selectionMode();

    while(m > GraphicsViewInterface::REMOVE_ONE)
        m -= GraphicsViewInterface::REMOVE_ONE;

    return (GraphicsViewInterface::SelectionMode)m;
}

bool DM_OsgPicker::mustSelectPoints() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_POINTS) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_POINT));
}

bool DM_OsgPicker::mustSelectEdges() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_EDGES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_EDGE));
}

bool DM_OsgPicker::mustSelectFaces() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_FACES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_FACE));
}

bool DM_OsgPicker::mustSelectItems() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE));
}

void DM_OsgPicker::select(const QPoint &pointYInversed)
{
    SelectionMode mode = selectionModeToBasic();

    if((mode == GraphicsViewInterface::SELECT) || (mode == GraphicsViewInterface::SELECT_ONE))
        removeAllIdFromSelection();

    osg::Camera* camera = m_osgView->getCamera();

    if( !camera )
      return;

    osg::Viewport* viewport = camera->getViewport();

    double mx = viewport->x() + pointYInversed.x();
    double my = viewport->y() + pointYInversed.y();

    // half width, height.
    double w = ((double)selectRegionWidth())/2.0;
    double h = ((double)selectRegionHeight())/2.0;

    if(mustSelectItems()) {
        selectItemsInWindowOrByPolytope(mode, mx, my, w, h, getPolytopeToUse());
    } else if(mustSelectPoints()) {
        selectPointsInWindowOrByPolytope(mode, mx, my, w, h, getPolytopeToUse());
    } else if(mustSelectEdges()) {
        selectEdgesInWindowOrByPolytope(mode, mx, my, w, h, getPolytopeToUse());
    } else if(mustSelectFaces()) {
        selectFacesInWindowOrByPolytope(mode, mx, my, w, h, getPolytopeToUse());
    }

    setSelectionMode(GraphicsViewInterface::NONE);
}

CT_CIR DM_OsgPicker::getSelectedPoints()
{
    if(m_selectionPointsChanged) {
        m_selectedPointsBackup = PS_REPOSITORY->registerPointCloudIndex(convertSelectedPointsToIndexCloud());
        m_selectionPointsChanged = false;
    }

    return m_selectedPointsBackup;
}

void DM_OsgPicker::addItemIDToSelection(const GLuint &id)
{
    addItemToSelection(getDocumentView()->getItemDrawable(id));
}

void DM_OsgPicker::addItemToSelection(CT_AbstractItemDrawable *item)
{
    if((item != NULL) && !item->isSelected())
        item->setSelected(true);
}

void DM_OsgPicker::removeItemIDFromSelection(const GLuint &id)
{
    removeItemFromSelection(getDocumentView()->getItemDrawable(id));
}

void DM_OsgPicker::removeItemFromSelection(CT_AbstractItemDrawable *item)
{
    if((item != NULL) && item->isSelected())
        item->setSelected(false);
}

void DM_OsgPicker::addPointIdToSelection(const size_t &globalIndex)
{
    (*m_globalVertexAttribArray.get())[globalIndex] = ENABLE_SELECTION;
    m_selectionPointsChanged = true;
}

void DM_OsgPicker::addPointsIdToSelection(const CT_AbstractCloudIndex *indexes)
{
    GlobalVertexAttribArrayType *array = m_globalVertexAttribArray.get();

    size_t s = indexes->size();

    for(size_t i=0; i<s; ++i) {
        (*array)[indexes->constIndexAt(i)] = ENABLE_SELECTION;
    }
    m_selectionPointsChanged = true;
}

void DM_OsgPicker::addCloudPointsIdToSelection(const QList<CT_AbstractCloudIndex *> &indexes)
{
    QListIterator<CT_AbstractCloudIndex*> it(indexes);

    while(it.hasNext())
        addPointsIdToSelection(it.next());
}

void DM_OsgPicker::removePointIdFromSelection(const size_t &globalIndex)
{
    (*m_globalVertexAttribArray.get())[globalIndex] = DISABLE_SELECTION;
    m_selectionPointsChanged = true;
}

void DM_OsgPicker::removePointsIdFromSelection(const CT_AbstractCloudIndex *indexes)
{
    GlobalVertexAttribArrayType *array = m_globalVertexAttribArray.get();

    size_t s = indexes->size();

    for(size_t i=0; i<s; ++i) {
        (*array)[indexes->constIndexAt(i)] = DISABLE_SELECTION;
    }
    m_selectionPointsChanged = true;
}

void DM_OsgPicker::removeCloudPointsIdFromSelection(const QList<CT_AbstractCloudIndex *> &indexes)
{
    QListIterator<CT_AbstractCloudIndex*> it(indexes);

    while(it.hasNext())
        removePointsIdFromSelection(it.next());
}

void DM_OsgPicker::setAllPointsSelected(bool selected)
{
    GlobalVertexAttribArrayType::value_type selection = selected ? ENABLE_SELECTION : DISABLE_SELECTION;

    GlobalVertexAttribArrayType *array = m_globalVertexAttribArray.get();

    size_t s = array->size();

    for(size_t i=0; i<s; ++i) {
        (*array)[i] = selection;
    }
    m_selectionPointsChanged = true;
}

void DM_OsgPicker::addFaceIdToSelection(const size_t &globalIndex)
{
    const CT_Face &face = m_fAccess.constFaceAt(globalIndex);

    addPointIdToSelection(face.iPointAt(0));
    addPointIdToSelection(face.iPointAt(1));
    addPointIdToSelection(face.iPointAt(2));
}

void DM_OsgPicker::removeFaceIdFromSelection(const size_t &globalIndex)
{
    const CT_Face &face = m_fAccess.constFaceAt(globalIndex);

    removePointIdFromSelection(face.iPointAt(0));
    removePointIdFromSelection(face.iPointAt(1));
    removePointIdFromSelection(face.iPointAt(2));
}

void DM_OsgPicker::setAllFacesSelected(bool selected)
{
    size_t s = m_fAccess.size();

    if(selected) {
        for(size_t i=0; i<s; ++i) {
            const CT_Face &face = m_fAccess.constFaceAt(i);

            addPointIdToSelection(face.iPointAt(0));
            addPointIdToSelection(face.iPointAt(1));
            addPointIdToSelection(face.iPointAt(2));
        }
    } else {
        for(size_t i=0; i<s; ++i) {
            const CT_Face &face = m_fAccess.constFaceAt(i);

            removePointIdFromSelection(face.iPointAt(0));
            removePointIdFromSelection(face.iPointAt(1));
            removePointIdFromSelection(face.iPointAt(2));
        }
    }
}

void DM_OsgPicker::addEdgeIdToSelection(const size_t &globalIndex)
{
    const CT_Edge &edge = m_eAccess.constEdgeAt(globalIndex);

    addPointIdToSelection(edge.iPointAt(0));
    addPointIdToSelection(edge.iPointAt(1));
}

void DM_OsgPicker::removeEdgeIdFromSelection(const size_t &globalIndex)
{
    const CT_Edge &edge = m_eAccess.constEdgeAt(globalIndex);

    removePointIdFromSelection(edge.iPointAt(0));
    removePointIdFromSelection(edge.iPointAt(1));
}

void DM_OsgPicker::setAllEdgesSelected(bool selected)
{
    size_t s = m_fAccess.size();

    if(selected) {
        for(size_t i=0; i<s; ++i) {
            const CT_Edge &edge = m_eAccess.constEdgeAt(i);

            addPointIdToSelection(edge.iPointAt(0));
            addPointIdToSelection(edge.iPointAt(1));
        }
    } else {
        for(size_t i=0; i<s; ++i) {
            const CT_Edge &edge = m_eAccess.constEdgeAt(i);

            removePointIdFromSelection(edge.iPointAt(0));
            removePointIdFromSelection(edge.iPointAt(1));
        }
    }
}

void DM_OsgPicker::removeAllIdFromSelection()
{
    GlobalVertexAttribArrayType *array = m_globalVertexAttribArray.get();

    size_t s = array->size();

    for(size_t i=0; i<s; ++i) {
        (*array)[i] = DISABLE_SELECTION;
    }
    m_selectionPointsChanged = true;

    const DM_OsgSceneManager::ResultCollection &itemsRepresentedByGroup = m_sceneManager->itemDrawableAndResults();
    DM_OsgSceneManager::ResultCollectionIterator it(itemsRepresentedByGroup);

    while(it.hasNext()) {
        it.next();
        setVertexAttribArrayOfItemDrawableSelected(it.value().m_rootGroup.get(), false);
    }

    getDocumentView()->setSelectAllItemDrawable(false);

    m_sceneManager->dirtyDisplayListOfAllElementsOfItemDrawable();
}

DM_DocumentView* DM_OsgPicker::getDocumentView() const
{
    return m_view->getDocumentView();
}

osg::Polytope* DM_OsgPicker::getPolytopeToUse() const
{
    return (m_usePolytope ? ((osg::Polytope*)&m_polytope) : NULL);
}

void DM_OsgPicker::setGlobalCloudPointsOfItemDrawableSelected(CT_AbstractItemDrawable *item, bool selected)
{
    DM_FakePainter painter;
    painter.setComputingMode(DM_FakePainter::BackupPointCloudIndex | DM_FakePainter::BackupPointCloudIndexIfEdge | DM_FakePainter::BackupPointCloudIndexIfFace);
    item->draw(*m_view, painter);

    if(selected)
        addCloudPointsIdToSelection(painter.pointCloudIndexBackup());
    else
        removeCloudPointsIdFromSelection(painter.pointCloudIndexBackup());
}

void DM_OsgPicker::setVertexAttribArrayOfItemDrawableSelected(CT_AbstractItemDrawable *item, bool selected)
{
    osg::ref_ptr<osg::Group> group = m_sceneManager->itemDrawableToGroup(item);

    if(group.valid())
        setVertexAttribArrayOfItemDrawableSelected(group.get(), selected);
}

void DM_OsgPicker::setVertexAttribArrayOfItemDrawableSelected(osg::Group *result, bool selected)
{
    osg::ref_ptr<LocalVertexAttribArray> array = (LocalVertexAttribArray*)DM_PainterToOsgElements::staticGetLocalVertexAttribArray(result, DM_MultipleItemDrawableToOsgWorker::staticLocalVertexAttribArrayLocationIndex());

    if(array.valid()) {
        size_t n = array->size();

        LocalVertexAttribArray::value_type value = selected ? 1 : 0;

        for(size_t i=0; i<n; ++i)
            array->at(i) = value;
    }
}

void DM_OsgPicker::selectItemsInWindowOrByPolytope(SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope)
{
    osg::ref_ptr<DM_ItemDrawablePolytopeIntersector> intersector = NULL;

    if(polytope == NULL)
        intersector = new DM_ItemDrawablePolytopeIntersector( osgUtil::Intersector::WINDOW, mx-w, my-h, mx+w, my+h );
    else
        intersector = new DM_ItemDrawablePolytopeIntersector(*polytope);

    if((basicMode == GraphicsViewInterface::SELECT_ONE)
            || (basicMode == GraphicsViewInterface::ADD_ONE)
            || (basicMode == GraphicsViewInterface::REMOVE_ONE)) {
        intersector->setIntersectionLimit(DM_ItemDrawablePolytopeIntersector::LIMIT_ONE);
    }

    osgUtil::IntersectionVisitor iv( intersector );
    DM_OsgSceneManager::staticSetPickerTraversalMask(iv);

    m_osgView->getCamera()->accept( iv );

    if(intersector->containsIntersections())
    {
        QSetIterator<osg::Group*> itR(intersector->results());

        bool continueLoop = true;

        while(itR.hasNext() && continueLoop) {
            osg::Group *r = itR.next();

            CT_AbstractItemDrawable *item = DM_OsgSceneManager::staticConvertNodeToItemDrawable(*r);

            if(item != NULL) {

                switch(basicMode) {
                    case GraphicsViewInterface::SELECT:     addItemToSelection(item);
                                                            break;

                    case GraphicsViewInterface::SELECT_ONE: addItemToSelection(item);
                                                            continueLoop = false;
                                                            break;

                    case GraphicsViewInterface::ADD:        addItemToSelection(item);
                                                            break;

                    case GraphicsViewInterface::ADD_ONE:    addItemToSelection(item);
                                                            continueLoop = false;
                                                            break;

                    case GraphicsViewInterface::REMOVE:     removeItemFromSelection(item);
                                                            break;

                    case GraphicsViewInterface::REMOVE_ONE: removeItemFromSelection(item);
                                                            continueLoop = false;
                                                            break;

                    default: continueLoop = false;
                             break;

                }
            }
        }
    }
}

void DM_OsgPicker::selectPointsInWindowOrByPolytope(DM_OsgPicker::SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope)
{
    selectPEFInWindowOrByPolytope(basicMode, mx, my, w, h, polytope, DM_GlobalPointsPolytopeIntersector::POINTS);
}

void DM_OsgPicker::selectEdgesInWindowOrByPolytope(DM_OsgPicker::SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope)
{
    selectPEFInWindowOrByPolytope(basicMode, mx, my, w, h, polytope, DM_GlobalPointsPolytopeIntersector::EDGES);
}

void DM_OsgPicker::selectFacesInWindowOrByPolytope(DM_OsgPicker::SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope)
{
    selectPEFInWindowOrByPolytope(basicMode, mx, my, w, h, polytope, DM_GlobalPointsPolytopeIntersector::FACES);
}

void DM_OsgPicker::selectPEFInWindowOrByPolytope(DM_OsgPicker::SelectionMode basicMode, double mx, double my, double w, double h, osg::Polytope *polytope, DM_GlobalPointsPolytopeIntersector::ElementFilter filter)
{
    DM_GlobalPointsPolytopeIntersector *intersector = NULL;

    if(polytope == NULL)
        intersector = new DM_GlobalPointsPolytopeIntersector( osgUtil::Intersector::WINDOW, mx-w, my-h, mx+w, my+h );
    else
        intersector = new DM_GlobalPointsPolytopeIntersector(*polytope);

    intersector->setFilter(filter);

    if((basicMode == GraphicsViewInterface::SELECT_ONE )
            || (basicMode == GraphicsViewInterface::ADD_ONE)
            || (basicMode == GraphicsViewInterface::REMOVE_ONE))
        intersector->setIntersectionLimit(DM_GlobalPointsPolytopeIntersector::LIMIT_ONE);

    osgUtil::IntersectionVisitor iv( intersector );
    DM_OsgSceneManager::staticSetPickerTraversalMask(iv);

    const DM_OsgSceneManager::ResultCollection &itemsRepresentedByGroup = m_sceneManager->itemDrawableAndResults();
    DM_OsgSceneManager::ResultCollectionIterator it(itemsRepresentedByGroup);

    while(it.hasNext()) {
        it.next();
        osg::ref_ptr<osg::Group> local = DM_PainterToOsgElements::staticGetLocalGeometriesGroup(it.value().m_rootGroup.get());

        if(local.valid())
            DM_OsgSceneManager::staticSetEnablePicking(local.get(), false);
    }

    m_osgView->getCamera()->accept( iv );

    if(intersector->containsIntersections())
    {
        DM_GlobalPointsPolytopeIntersector::Intersections::const_iterator begin = intersector->getIntersections().begin();
        DM_GlobalPointsPolytopeIntersector::Intersections::const_iterator end = intersector->getIntersections().end();

        bool continueLoop = true;

        int nComputed = 0;
        int maxToCompute = 1;

        if(filter == DM_GlobalPointsPolytopeIntersector::EDGES)
            maxToCompute = 2;
        else if(filter == DM_GlobalPointsPolytopeIntersector::FACES)
            maxToCompute = 3;

        while(begin != end && continueLoop) {
            const size_t &index = (*begin);

            switch(basicMode) {
                case GraphicsViewInterface::SELECT:     addPointIdToSelection(index);
                                                        ++nComputed;
                                                        break;

                case GraphicsViewInterface::SELECT_ONE: addPointIdToSelection(index);
                                                        ++nComputed;
                                                        continueLoop = (nComputed < maxToCompute);
                                                        break;

                case GraphicsViewInterface::ADD:        addPointIdToSelection(index);
                                                        ++nComputed;
                                                        break;

                case GraphicsViewInterface::ADD_ONE:    addPointIdToSelection(index);
                                                        ++nComputed;
                                                        continueLoop = (nComputed < maxToCompute);
                                                        break;

                case GraphicsViewInterface::REMOVE:     removePointIdFromSelection(index);
                                                        ++nComputed;
                                                        break;

                case GraphicsViewInterface::REMOVE_ONE: removePointIdFromSelection(index);
                                                        ++nComputed;
                                                        continueLoop = (nComputed < 2);
                                                        break;

                default: continueLoop = false;
                         break;

            }

            ++begin;
        }
    }

    it.toFront();

    while(it.hasNext()) {
        it.next();
        osg::ref_ptr<osg::Group> local = DM_PainterToOsgElements::staticGetLocalGeometriesGroup(it.value().m_rootGroup.get());

        if(local.valid())
            DM_OsgSceneManager::staticSetEnablePicking(local.get(), true);
    }

    if(m_selectionPointsChanged)
        m_sceneManager->dirtyDisplayListOfAllElementsOfItemDrawable();
}

CT_AbstractModifiablePointCloudIndex* DM_OsgPicker::convertSelectedPointsToIndexCloud() const
{
    CT_PointCloudIndexVector *sel = new CT_PointCloudIndexVector();

    size_t size = m_globalVertexAttribArray->size();

    for(size_t i=0; i<size; ++i) {
        const GlobalVertexAttribArrayType::value_type &isSelected = (*m_globalVertexAttribArray)[i];

        if(isSelected == ENABLE_SELECTION)
            sel->addIndex(i);
    }

    if(sel->size() != 0)
        return sel;

    delete sel;

    return NULL;
}

void DM_OsgPicker::itemAddedToView(CT_AbstractItemDrawable &item)
{
    connect(&item, SIGNAL(selectChange(bool,CT_AbstractItemDrawable*)), this, SLOT(itemSelectionChanged(bool,CT_AbstractItemDrawable*)), Qt::DirectConnection);
}

void DM_OsgPicker::itemRemovedFromView(CT_AbstractItemDrawable &item)
{
    disconnect(&item, NULL, this, NULL);
}

void DM_OsgPicker::itemSelectionChanged(bool selected, CT_AbstractItemDrawable *item)
{
    setGlobalCloudPointsOfItemDrawableSelected(item, selected);
    setVertexAttribArrayOfItemDrawableSelected(item, selected);

    m_sceneManager->dirtyDisplayListOfAllElementsOfItemDrawable(item);
}
