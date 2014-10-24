/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "g3dgraphicsview.h"
#include "gdocumentview.h"
#include "manipulatedCameraFrame.h"
#include "dm_guimanager.h"
#include "tools/graphicsview/dm_colorselectionmanagert.h"
#include "tools/attributes/dm_attributesbuildingcollectiont.h"
#include "tools/attributes/worker/dm_attributescolort.h"
#include "tools/attributes/worker/dm_attributesnormalt.h"
#include "tools/attributes/worker/dm_attributesscalart.h"

#include "ct_global/ct_context.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"
#include "ct_attributes/abstract/ct_abstractattributesscalar.h"

#include "dm_iteminfoforgraphics.h"

#include <limits>

G3DGraphicsView::G3DGraphicsView(QWidget *parent) : QGLViewer(QGLFormat(QGL::SampleBuffers), parent), GGraphicsView()
{
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);

    m_docGV = NULL;
    m_painter = NULL;

    _mutex = new QMutex(QMutex::Recursive);
    _g.setGraphicsView(this);
    m_fakeG.setGraphicsView(this);

    _selectionMode = NONE;
    _drawModeToUse = NORMAL;
    _drawModeUsed = FAST;
    _2dActive = false;
    _forceDrawMode = false;

    m_useColorCloud = true;

    _cameraController.setRealCamera(QGLViewer::camera());
    _cameraController.setView(this);

    m_signalEmitter.setGraphicsView(this);

    m_pointsSelectionManager = new DM_ColorSelectionManagerT<CT_AbstractPointsAttributes>(CT_Repository::SyncWithPointCloud);
    m_facesSelectionManager = new DM_ColorSelectionManagerT<CT_AbstractFaceAttributes>(CT_Repository::SyncWithFaceCloud);
    m_edgesSelectionManager = new DM_ColorSelectionManagerT<CT_AbstractEdgeAttributes>(CT_Repository::SyncWithEdgeCloud);

    // 1000 lments slectionnable (4x1000 = 4000)
    setSelectBufferSize(4000);
    setSceneRadius(100); // 100 mètres

    connect(this, SIGNAL(mustSetDrawModeStartTimerAndRedraw()), this, SLOT(setDrawModeStartTimerAndRedraw()), Qt::QueuedConnection);
    connect(this, SIGNAL(mustRedraw()), this, SLOT(update()), Qt::QueuedConnection);
    connect(&_timerChangeDrawMode, SIGNAL(timeout()), this, SLOT(changeDrawMethodToNormal()), Qt::QueuedConnection);
    connect(QGLViewer::camera()->frame(), SIGNAL(spun()), QGLViewer::camera()->frame(), SLOT(stopSpinning()));

    changeStateFileName();

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), this, SLOT(initIndexCloudRegistered()));

    initIndexCloudRegistered();

    initFromOptions();
}

G3DGraphicsView::~G3DGraphicsView()
{
    saveStateToFile();
    delete _mutex;

    delete m_pointsSelectionManager;
    delete m_facesSelectionManager;
    delete m_edgesSelectionManager;
}

QWidget* G3DGraphicsView::getViewWidget() const
{
    return const_cast<G3DGraphicsView*>(this);
}

void G3DGraphicsView::setDocumentView(const DM_DocumentView *doc)
{
    m_docGV = dynamic_cast<GDocumentViewForGraphics*>((DM_DocumentView*)doc);

    m_pointsSelectionManager->setDocument(m_docGV);
    m_facesSelectionManager->setDocument(m_docGV);
    m_edgesSelectionManager->setDocument(m_docGV);

    connect(m_docGV, SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), this, SLOT(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), Qt::DirectConnection);

    GGraphicsView::setDocumentView(doc);
}

void G3DGraphicsView::setOptions(const DM_GraphicsViewOptions &newOptions)
{
    GGraphicsView::setOptions(newOptions);

    initFromOptions();
}

GraphicsViewOptionsInterface& G3DGraphicsView::getOptions()
{
    GraphicsViewOptionsInterface& opt = GGraphicsView::getOptions();
    ((DM_GraphicsViewOptions&)opt).drawAxis(axisIsDrawn());

    return opt;
}

const DM_GraphicsViewOptions &G3DGraphicsView::getOptions() const
{
    const DM_GraphicsViewOptions& opt = GGraphicsView::getOptions();
    ((DM_GraphicsViewOptions*)(&opt))->drawAxis(axisIsDrawn());

    return opt;
}

GraphicsViewSignalEmitterInterface* G3DGraphicsView::signalEmitter() const
{
    return (GraphicsViewSignalEmitterInterface*)&m_signalEmitter;
}

QSharedPointer<CT_StandardColorCloudRegistered> G3DGraphicsView::colorCloudOf(GraphicsViewInterface::ColorCloudType type) const
{
    if(type == GraphicsViewInterface::CPointCloud)
        return m_docGV->colorCloudRegistered<CT_AbstractPointsAttributes>();
    else if(type == GraphicsViewInterface::CFaceCloud)
        return m_docGV->colorCloudRegistered<CT_AbstractFaceAttributes>();
    else if(type == GraphicsViewInterface::CEdgeCloud)
        return m_docGV->colorCloudRegistered<CT_AbstractEdgeAttributes>();

    return QSharedPointer<CT_StandardColorCloudRegistered>(NULL);
}

QSharedPointer<CT_StandardNormalCloudRegistered> G3DGraphicsView::normalCloudOf(GraphicsViewInterface::NormalCloudType type) const
{
    if(type == GraphicsViewInterface::NPointCloud)
        return m_docGV->normalCloudRegistered<CT_AbstractPointsAttributes>();
    else if(type == GraphicsViewInterface::NFaceCloud)
        return m_docGV->normalCloudRegistered<CT_AbstractFaceAttributes>();
    else if(type == GraphicsViewInterface::NEdgeCloud)
        return m_docGV->normalCloudRegistered<CT_AbstractEdgeAttributes>();

    return QSharedPointer<CT_StandardNormalCloudRegistered>(NULL);
}

GraphicsViewInterface::GraphicsViewType G3DGraphicsView::type() const
{
    if(_2dActive)
        return GV2D;

    return GV3D;
}

QList<CT_AbstractItemDrawable *> G3DGraphicsView::getSelectedItems() const
{
    return getDocumentView().getSelectedItemDrawable();
}

void G3DGraphicsView::beginRemoveMultiplePointsFromSelection(const size_t &n)
{
    m_pointsSelectionManager->beginRemoveMultipleIDFromSelection(n);
}

void G3DGraphicsView::removePointFromSelection(const size_t &globalIndex)
{
    m_pointsSelectionManager->removeIDFromSelection(globalIndex);
}

void G3DGraphicsView::endRemoveMultiplePointsFromSelection()
{
    m_pointsSelectionManager->endRemoveMultipleIDFromSelection();
}

void G3DGraphicsView::setAllPointsSelected(bool select)
{
    if(!select)
    {
        m_pointsSelectionManager->clearSelection();
        return;
    }

    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::BackupPointCloudIndex);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    m_pointsSelectionManager->addCloudIndexToSelection(m_fakeG.pointCloudIndexBackup());

    unlockPaint();
}

void G3DGraphicsView::beginRemoveMultipleFacesFromSelection(const size_t &n)
{
    m_facesSelectionManager->beginRemoveMultipleIDFromSelection(n);
}

void G3DGraphicsView::removeFaceFromSelection(const size_t &globalIndex)
{
    m_facesSelectionManager->removeIDFromSelection(globalIndex);
}

void G3DGraphicsView::endRemoveMultipleFacesFromSelection()
{
    m_facesSelectionManager->endRemoveMultipleIDFromSelection();
}

void G3DGraphicsView::setAllFacesSelected(bool select)
{
    if(!select)
    {
        m_facesSelectionManager->clearSelection();
        return;
    }

    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::BackupFaceCloudIndex);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    m_facesSelectionManager->addCloudIndexToSelection(m_fakeG.faceCloudIndexBackup());

    unlockPaint();
}

void G3DGraphicsView::beginRemoveMultipleEdgesFromSelection(const size_t &n)
{
    m_edgesSelectionManager->beginRemoveMultipleIDFromSelection(n);
}

void G3DGraphicsView::removeEdgeFromSelection(const size_t &globalIndex)
{
    m_edgesSelectionManager->removeIDFromSelection(globalIndex);
}

void G3DGraphicsView::endRemoveMultipleEdgesFromSelection()
{
    m_edgesSelectionManager->endRemoveMultipleIDFromSelection();
}

void G3DGraphicsView::setAllEdgesSelected(bool select)
{
    if(!select)
    {
        m_edgesSelectionManager->clearSelection();
        return;
    }

    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::BackupEdgeCloudIndex);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    m_edgesSelectionManager->addCloudIndexToSelection(m_fakeG.edgeCloudIndexBackup());

    unlockPaint();
}

size_t G3DGraphicsView::countPoints()
{
    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::CountPoints);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    unlockPaint();

    return m_fakeG.nPoints();
}

size_t G3DGraphicsView::countEdges()
{
    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::CountEdges);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    unlockPaint();

    return m_fakeG.nEdges();
}

size_t G3DGraphicsView::countFaces()
{
    lockPaint();

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::CountFaces);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    unlockPaint();

    return m_fakeG.nFaces();
}

size_t G3DGraphicsView::countItems()
{
    return getDocumentView().nItemDrawable();
}

void G3DGraphicsView::setDrawMode(G3DGraphicsView::DrawMode dMode)
{
    _forceDrawMode = true;
    _drawModeToUse = dMode;

    startRedrawTimer();
}

GraphicsViewInterface::DrawMode G3DGraphicsView::drawMode() const
{
    return _drawModeUsed;
}

GraphicsViewInterface::DrawMode G3DGraphicsView::drawModeToUse() const
{
    return _drawModeToUse;
}

void G3DGraphicsView::setDrawModeChangeTime(const int &msec)
{
    _timerChangeDrawMode.setInterval(msec);

    if(msec == 0)
        _timerChangeDrawMode.stop();
}

int G3DGraphicsView::drawModeChangeTime() const
{
    return _timerChangeDrawMode.interval();
}

void G3DGraphicsView::setSelectBufferSize(size_t size)
{
    QGLViewer::setSelectBufferSize(size);
}

size_t G3DGraphicsView::selectBufferSize() const
{
    return QGLViewer::selectBufferSize();
}

void G3DGraphicsView::setSelectRegionWidth(int width)
{
    if(width <= 0)
        width = 1;

    QGLViewer::setSelectRegionWidth(width);
}

void G3DGraphicsView::setSelectRegionHeight(int height)
{
    if(height <= 0)
        height = 1;

    QGLViewer::setSelectRegionHeight(height);
}

void G3DGraphicsView::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    _selectionMode = mode;
}

GraphicsViewInterface::SelectionMode G3DGraphicsView::selectionMode() const
{
    return _selectionMode;
}

void G3DGraphicsView::select(const QPoint &point)
{
    QGLViewer::camera()->getModelViewMatrix(modelViewMatrix_);
    QGLViewer::camera()->getProjectionMatrix(projectionMatrix_);
    QGLViewer::camera()->getViewport(viewport_);

    m_centerPointOfSelection = point;
    QGLViewer::select(point);
}

bool G3DGraphicsView::getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const
{
    return getCamera()->getCameraFrustumPlanesCoefficients(coef);
}

float G3DGraphicsView::distanceToFrustumPlane(int index, const double &x, const double &y, const double &z) const
{
    return qglviewer::Vec(x, y, z) * qglviewer::Vec(m_planeCoefficients[index]) - m_planeCoefficients[index][3];
}

bool G3DGraphicsView::aaBoxIsVisible(const QVector3D &p1, const QVector3D &p2, bool *entirely) const
{
    bool allInForAllPlanes = true;
    for (int i=0; i<6; ++i)
      {
        bool allOut = true;
        for (unsigned int c=0; c<8; ++c)
      {
        if (distanceToFrustumPlane(i, (c&4)?p1.x():p2.x(), (c&2)?p1.y():p2.y(), (c&1)?p1.z():p2.z()) > 0.0)
          allInForAllPlanes = false;
        else
          allOut = false;
      }

        // The eight points are on the outside side of this plane
        if (allOut)
      return false;
      }

    if (entirely)
      // Entirely visible : the eight points are on the inside side of the 6 planes
      *entirely = allInForAllPlanes;

    // Too conservative, but tangent cases are too expensive to detect
    return true;
}

bool G3DGraphicsView::sphereIsVisible(const QVector3D &center, float radius) const
{
    for (int i=0; i<6; ++i) {
        if (distanceToFrustumPlane(i, center.x(), center.y(), center.z()) > radius)
          return false;
    }

    return true;
}

QVector3D G3DGraphicsView::pointUnderPixel(const QPoint &pixel, bool &found) const
{
    qglviewer::Vec vec = QGLViewer::camera()->pointUnderPixel(pixel, found);
    QVector3D vr(vec.x, vec.y, vec.z);

    return vr;
}

void G3DGraphicsView::convertClickToLine(const QPoint &pixel, QVector3D &orig, QVector3D &dir) const
{
    qglviewer::Vec origi;
    qglviewer::Vec direc;

    QGLViewer::camera()->convertClickToLine(pixel, origi, direc);

    orig.setX(origi.x);
    orig.setY(origi.y);
    orig.setZ(origi.z);

    dir.setX(direc.x);
    dir.setY(direc.y);
    dir.setZ(direc.z);
}

int G3DGraphicsView::width() const
{
    return QGLViewer::width();
}

int G3DGraphicsView::height() const
{
    return QGLViewer::height();
}

void G3DGraphicsView::lockPaint()
{
    _mutex->lock();
}

void G3DGraphicsView::unlockPaint()
{
    _mutex->unlock();
}

void G3DGraphicsView::redraw()
{
    if(QThread::currentThread() == qApp->thread())
        setDrawModeStartTimerAndRedraw();
    else
        emit mustSetDrawModeStartTimerAndRedraw();
}

void G3DGraphicsView::setDrawFastest(DM_GraphicsViewOptions::DrawFastestMode mode)
{
    _options->drawFastest(mode);
}

DM_GraphicsViewOptions::DrawFastestMode G3DGraphicsView::drawFastest() const
{
    return _options->drawFastest();
}

bool G3DGraphicsView::mustDrawFastestNow() const
{
    return (drawModeToUse() == FAST);
}

void G3DGraphicsView::setUseCloudColor(bool use)
{
    m_useColorCloud = use;
}

void G3DGraphicsView::setUseCloudNormal(bool use)
{
    _g.setUseNormalCloudForPoints(use);
    _g.setUseNormalCloudForFaces(use);
}

DM_GraphicsViewCamera* G3DGraphicsView::getCamera() const
{
    return (DM_GraphicsViewCamera*)&_cameraController;
}

void G3DGraphicsView::active2dView(bool enable)
{
    if(enable != _2dActive)
    {
        _2dActive = enable;

        if(enable)
        {
            _2dActive = enable;

            QGLViewer::camera()->setPosition(qglviewer::Vec(0, 0, 1));
            QGLViewer::camera()->lookAt(sceneCenter());

            QGLViewer::camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);

            qglviewer::WorldConstraint* constraint = new qglviewer::WorldConstraint();
            constraint->setRotationConstraintType(qglviewer::AxisPlaneConstraint::FORBIDDEN);

            QGLViewer::camera()->frame()->setConstraint(constraint);
        }
        else
        {
            QGLViewer::camera()->frame()->setConstraint(NULL);
            QGLViewer::camera()->setType(qglviewer::Camera::PERSPECTIVE);
        }
    }

    changeStateFileName();
}

bool G3DGraphicsView::is2DViewActived() const
{
    return _2dActive;
}

void G3DGraphicsView::showContextMenu(const QPoint &pos)
{
    QMenu *menu = constructContextMenu();

    if(!menu->isEmpty())
        menu->exec(pos);

    delete menu;
}

DM_ColorSelectionManagerT<CT_AbstractPointsAttributes> *G3DGraphicsView::pointsInformationManager() const
{
    return m_pointsSelectionManager;
}

DM_ColorSelectionManagerT<CT_AbstractFaceAttributes> *G3DGraphicsView::facesInformationManager() const
{
    return m_facesSelectionManager;
}

DM_ColorSelectionManagerT<CT_AbstractEdgeAttributes> *G3DGraphicsView::edgesInformationManager() const
{
    return m_edgesSelectionManager;
}

void G3DGraphicsView::addActionOptions(ActionOptionsInterface *options)
{
    getDocumentView().addActionOptions(options);
}

void G3DGraphicsView::removeActionOptions(ActionOptionsInterface *options)
{
    getDocumentView().removeActionOptions(options);
}

///////////// PROTECTED /////////////

void G3DGraphicsView::addIdToSelection(const GLuint &id)
{
    if(mustSelectPoints())
        addPointsIDToSelection(id);
    else if(mustSelectEdges())
        addEdgesIDToSelection(id);
    else if(mustSelectFaces())
        addFacesIDToSelection(id);
    else
        addItemsIDToSelection(id);
}

void G3DGraphicsView::addPointsIDToSelection(const GLuint &id)
{
    m_pointsSelectionManager->addIDToSelection(id);
}

void G3DGraphicsView::addFacesIDToSelection(const GLuint &id)
{
    m_facesSelectionManager->addIDToSelection(id);
}

void G3DGraphicsView::addEdgesIDToSelection(const GLuint &id)
{
    m_edgesSelectionManager->addIDToSelection(id);
}

void G3DGraphicsView::addItemsIDToSelection(const GLuint &id)
{
    CT_AbstractItemDrawable *item = getDocumentView().getItemDrawable(id);

    if(item != NULL)
    {
        /*lockPaint();

        m_fakeG.beginNewDraw();
        m_fakeG.setDrawMode(G3DFakePainter::BackupPointCloudIndex);

        item->draw(*this, m_fakeG);

        m_fakeG.endNewDraw();

        m_pointsSelectionManager->addCloudIndexToSelection(m_fakeG.pointCloudIndexBackup());

        unlockPaint();*/

        item->setSelected(true);
    }
}

void G3DGraphicsView::setLastItemIdSelected(const GLuint &id)
{
    CT_AbstractItemDrawable *item = getDocumentView().getItemDrawable(id);

    // fixe le pivot potentiel de la camera sur le centre du dernier item selectionne
    if(item != NULL)
        _cameraController.setLastItemSelectedCameraCenter(item->getCenterX(), item->getCenterY(), item->getCenterZ());
}

void G3DGraphicsView::setLastPointIdSelected(const GLuint &id)
{
    const CT_Point &point = PS_REPOSITORY->globalPointCloud()->constTAt(id);

    _cameraController.setLastItemSelectedCameraCenter(point.getX(), point.getY(), point.getZ());
}

void G3DGraphicsView::setLastFaceIdSelected(const GLuint &id)
{
    const CT_Face &face = PS_REPOSITORY->globalCloud<CT_Face>()->constTAt(id);

    float x = 0, y = 0, z = 0;

    for(int i=0; i<3; ++i)
    {
        CT_Point *point = face.pointAt(i);

        if(point == NULL)
            return;

        x += point->getX();
        y += point->getY();
        z += point->getZ();
    }

    _cameraController.setLastItemSelectedCameraCenter(x/3.0, y/3.0, z/3.0);
}

void G3DGraphicsView::setLastEdgeIdSelected(const GLuint &id)
{
    const CT_Edge &edge = PS_REPOSITORY->globalCloud<CT_Edge>()->constTAt(id);

    float x = 0, y = 0, z = 0;

    for(int i=0; i<2; ++i)
    {
        CT_Point *point = edge.pointAt(i);

        if(point == NULL)
            return;

        x += point->getX();
        y += point->getY();
        z += point->getZ();
    }

    _cameraController.setLastItemSelectedCameraCenter(x/2.0, y/2.0, z/2.0);
}

void G3DGraphicsView::removeIdFromSelection(const GLuint &id)
{
    if(mustSelectPoints())
        m_pointsSelectionManager->removeIDFromSelection(id);
    else if(mustSelectEdges())
        m_edgesSelectionManager->removeIDFromSelection(id);
    else if(mustSelectFaces())
        m_facesSelectionManager->removeIDFromSelection(id);
    else
    {
        CT_AbstractItemDrawable *item = getDocumentView().getItemDrawable(id);

        if(item != NULL)
            item->setSelected(false);
    }
}

void G3DGraphicsView::removePointsIDFromSelection(const GLuint &id)
{
    m_pointsSelectionManager->removeIDFromSelection(id);
}

void G3DGraphicsView::removeFacesIDFromSelection(const GLuint &id)
{
    m_facesSelectionManager->removeIDFromSelection(id);
}

void G3DGraphicsView::removeEdgesIDFromSelection(const GLuint &id)
{
    m_edgesSelectionManager->removeIDFromSelection(id);
}

void G3DGraphicsView::removeItemsIDFromSelection(const GLuint &id)
{
    CT_AbstractItemDrawable *item = getDocumentView().getItemDrawable(id);

    if(item != NULL)
        item->setSelected(false);
}

void G3DGraphicsView::removeAllIdFromSelection()
{
    m_pointsSelectionManager->clearSelection();
    m_edgesSelectionManager->clearSelection();
    m_facesSelectionManager->clearSelection();
    getDocumentView().setSelectAllItemDrawable(false);
}

void G3DGraphicsView::init()
{
    _g.initializeGl();

    if(!_2dActive)
        restoreStateFromFile();

    initOptions();

    if(colorVBOManager() != NULL)
        colorVBOManager()->initializeGL();
}

void G3DGraphicsView::initOptions()
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    DM_GraphicsViewOptions &options = dynamic_cast<DM_GraphicsViewOptions&>(((GraphicsViewOptionsInterface&)getOptions()));

    QColor col = backgroundColor();
    options.setBackgroudColor(col);
    options.drawAxis(axisIsDrawn());
    options.setCameraType(((QGLViewer::camera()->type() == qglviewer::Camera::PERSPECTIVE) ? CameraInterface::PERSPECTIVE : CameraInterface::ORTHOGRAPHIC));

    _g.setDefaultPointSize(options.getPointSize());
}

void G3DGraphicsView::initFromOptions()
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    DM_GraphicsViewOptions &options = dynamic_cast<DM_GraphicsViewOptions&>(GGraphicsView::getOptions());

    setBackgroundColor(options.getBackgroundColor());
    setAxisIsDrawn(options.drawAxis());

    if(options.drawFastest() == DM_GraphicsViewOptions::Always)
    {
        setDrawModeChangeTime(0);
        internalSetDrawMode(FAST);
    }
    else if(options.drawFastest() == DM_GraphicsViewOptions::Never)
    {
        setDrawModeChangeTime(0);
        internalSetDrawMode(NORMAL);
    }
    else
    {
        setDrawModeChangeTime(options.getFastDrawTime());
        internalSetDrawMode(FAST);
    }

    if(!_2dActive)
        QGLViewer::camera()->setType(((options.getCameraType() == CameraInterface::PERSPECTIVE) ? qglviewer::Camera::PERSPECTIVE : qglviewer::Camera::ORTHOGRAPHIC));
    else
        options.setCameraType(CameraInterface::ORTHOGRAPHIC);

    _g.setDefaultPointSize(options.getPointSize());
    m_fastestIncrementOptimizer.setMinFPS(options.getMinFPS());
}

void G3DGraphicsView::preDraw()
{
    m_fastestIncrementOptimizer.preDraw();

    // Classical 3D drawing, usually performed by paintGL().
    delete m_painter;
    m_painter = new QPainter();

    m_painter->begin(this);
    m_painter->setRenderHint(QPainter::Antialiasing);

    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Reset OpenGL parameters
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    if(((const DM_GraphicsViewOptions&)getOptions()).useLight())
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    //glEnable(GL_CULL_FACE);

    if(getOptions().useTransparency())
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    qglClearColor(backgroundColor());

    if(colorVBOManager() != NULL)
        colorVBOManager()->preDraw();

    QGLViewer::preDraw();
}

void G3DGraphicsView::draw()
{
    getCameraFrustumPlanesCoefficients(m_planeCoefficients);

    _g.setDrawFastest(mustDrawFastestNow());

    drawInternal();
}

void G3DGraphicsView::postDraw()
{
    QGLViewer::postDraw();

    if(colorVBOManager() != NULL)
        colorVBOManager()->postDraw();

    // Restore OpenGL state
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    drawOverlay(*m_painter);

    m_painter->setPen(Qt::white);
    m_painter->drawText(10, 10, QString().setNum(m_fastestIncrementOptimizer.currentFPS()));
    m_painter->drawText(10, 20, QString().setNum(m_fastestIncrementOptimizer.fastestIncrement()));
    /*m_painter->drawText(10, 30, QString().setNum(_g.nOctreeCellsDrawed()));*/

    m_painter->end();

    delete m_painter;
    m_painter = NULL;

    _drawModeUsed = _drawModeToUse;

    m_fastestIncrementOptimizer.postDraw();
}

void G3DGraphicsView::fastDraw()
{
    const DM_GraphicsViewOptions &options = ((const G3DGraphicsView*)this)->getOptions();

    _g.setDrawFastest(options.drawFastest() != DM_GraphicsViewOptions::Never);

    drawInternal();
}

void G3DGraphicsView::drawInternal()
{
    m_signalEmitter.emitDrawingStarted();

    lockPaint();

    _g.beginNewDraw();
    _g.setColor(Qt::white);
    _g.setPointSize(getOptions().getPointSize());
    _g.setPointFastestIncrement(m_fastestIncrementOptimizer.fastestIncrement());

    OctreeController *octreeC = (OctreeController*)m_docGV->octreeOfPoints();

    const DM_GraphicsViewOptions &options = ((const G3DGraphicsView*)this)->getOptions();

    if(octreeC->hasElements() && !octreeC->mustBeReconstructed() && options.showOctree())
        _g.drawOctreeOfPoints(octreeC, PainterInterface::DrawOctree);

    // IF you uncomment this line you must add points selected to the selection manager otherwise
    // elements selected wont be in selected color
    /*if(octreeC->hasElements() && !octreeC->mustBeReconstructed()) {
        _g.drawOctreeOfPoints(octreeC, PainterInterface::DrawElements);
    } else  {*/

        QColor selectedColor = getOptions().getSelectedColor();

        const QHash<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *> &ii = getDocumentView().getItemsInformations();

        QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

        CT_AbstractResult *lastResult = NULL;
        QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *hash = NULL;

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            if(lastResult != item->result())
                hash = ii.value(item->result(), NULL);

            DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value(item, NULL));

            if(item->isSelected())
            {
                if(colorVBOManager() != NULL)
                    colorVBOManager()->setUseColorCloud(false);

                _g.setUseColorCloudForPoints(false);
                _g.setUseColorCloudForFaces(false);
                _g.setUseColorCloudForEdges(false);
                _g.setColor(selectedColor);
                _g.enableSetColor(false);

                item->draw(*this, _g);

                _g.enableSetColor(true);
            }
            else
            {
                if(colorVBOManager() != NULL)
                    colorVBOManager()->setUseColorCloud(m_useColorCloud);

                _g.setUseColorCloudForPoints(m_useColorCloud);
                _g.setUseColorCloudForFaces(m_useColorCloud);
                _g.setUseColorCloudForEdges(m_useColorCloud);

                if(info != NULL)
                    _g.setColor(info->color());

                item->draw(*this, _g);
            }
        }
    //}

    CT_AbstractActionForGraphicsView *action = dynamic_cast<CT_AbstractActionForGraphicsView*>(actionsHandler()->currentAction());

    if(action != NULL)
        action->draw(*this, _g);

    _g.endNewDraw();

    unlockPaint();

    checkAndShowOpenGLErrors();

    m_signalEmitter.emitDrawingFinished();
}

void G3DGraphicsView::drawCoordinates(QPainter &painter)
{
    qglviewer::Vec pos = QGLViewer::camera()->position();
    qglviewer::Vec center = QGLViewer::camera()->sceneCenter();
    qglviewer::Vec direction = QGLViewer::camera()->viewDirection();

    QFontMetrics metrics = fontMetrics();

    QList<QString> stringList;

    const DM_GraphicsViewOptions &options = ((const G3DGraphicsView*)this)->getOptions();

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraPosition)
    {
        stringList.append(QString("x : %1").arg(pos.x));
        stringList.append(QString("y : %1").arg(pos.y));
        stringList.append(QString("z : %1").arg(pos.z));
    }

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraSceneCenter)
    {
        stringList.append(QString("cx : %1").arg(center.x));
        stringList.append(QString("cy : %1").arg(center.y));
        stringList.append(QString("cz : %1").arg(center.z));
    }

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraViewDirection)
    {
        stringList.append(QString("rx : %1").arg(direction.x));
        stringList.append(QString("ry : %1").arg(direction.y));
        stringList.append(QString("rz : %1").arg(direction.z));
    }

    int n = stringList.size();

    if(n != 0)
    {
        int h = fontMetrics().height() + 2;
        int maxWidth = 0;
        int x = 10;
        int y = 0;

        if((options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInUpperRightCorner)
                || (options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInLowerRightCorner))
        {
            maxWidth = metrics.width(QString("aa : %1").arg(std::numeric_limits<double>::max()));

            x = width() - maxWidth - 10;
        }

        if((options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInLowerRightCorner)
                || (options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInLowerLeftCorner))
        {
            y = height() - (h*n) - 10;
        }
        else
        {
            y = int(1.5*((QApplication::font().pixelSize()>0)?QApplication::font().pixelSize():QApplication::font().pointSize())) + h;
        }

        painter.save();
        painter.setPen(Qt::white);
        QListIterator<QString> it(stringList);

        while(it.hasNext())
        {
            painter.drawText(x, y, it.next());
            y += h;
        }
        painter.restore();
    }
}

void G3DGraphicsView::drawOverlay(QPainter &painter)
{
    drawCoordinates(painter);

    CT_AbstractActionForGraphicsView *action = dynamic_cast<CT_AbstractActionForGraphicsView*>(actionsHandler()->currentAction());

    lockPaint();

    if(action != NULL)
        action->drawOverlay(*this, painter);

    unlockPaint();
}

void G3DGraphicsView::drawWithNames()
{
    lockPaint();

    if(colorVBOManager() != NULL) {
        colorVBOManager()->preDraw();
        colorVBOManager()->setUseColorCloud(false);
    }

    bool selectItems = true;

    if(mustSelectPoints())
    {
        m_idToAddInSelection.clear();

        OctreeController *octreeC = (OctreeController*)m_docGV->octreeOfPoints();

        if((octreeC->hasElements() && !octreeC->mustBeReconstructed()))
        {
            QRect rect(m_centerPointOfSelection.x()-(selectRegionWidth()/2),
                       m_centerPointOfSelection.y()-(selectRegionHeight()/2),
                       selectRegionWidth(),
                       selectRegionHeight());

            m_fakeG.beginNewDraw();
            m_fakeG.setDrawMode(G3DFakePainter::DrawPointsWithName);
            m_fakeG.setDrawFastest(mustDrawFastestNow());
            m_fakeG.setPointSize(getOptions().getPointSize());
            m_fakeG.setPointFastestIncrement(m_fastestIncrementOptimizer.fastestIncrement());

            QVector3D min = octreeC->octreeMinCorner();
            double cellSize = octreeC->cellsSize();
            int s = octreeC->numberOfCells();

            GLdouble xx[8];
            GLdouble yy[8];
            GLdouble zz[8];

            for(int x=0; x<s; ++x)
            {
                for(int y=0; y<s; ++y)
                {
                    for(int z=0; z<s; ++z)
                    {
                        const CT_AbstractCloudIndexT<CT_Point> *indexes = dynamic_cast<const CT_AbstractCloudIndexT<CT_Point>*>(octreeC->at(x, y, z));

                        if((indexes != NULL)/* && octreeC->isCellVisibleInFrustrum(x, y, z, planeCoefficients, entirely)*/)
                        {
                            qglviewer::Vec p1(min.x()+(x*cellSize), min.y()+(y*cellSize), min.z()+(z*cellSize));
                            qglviewer::Vec p2(min.x()+((x+1)*cellSize), min.y()+((y+1)*cellSize), min.z()+((z+1)*cellSize));

                            gluProject(p1.x,p1.y,p1.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[0],&yy[0],&zz[0]);
                            gluProject(p2.x,p1.y,p1.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[1],&yy[1],&zz[1]);
                            gluProject(p2.x,p2.y,p1.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[2],&yy[2],&zz[2]);
                            gluProject(p1.x,p2.y,p1.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[3],&yy[3],&zz[3]);
                            gluProject(p1.x,p1.y,p2.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[4],&yy[4],&zz[4]);
                            gluProject(p2.x,p1.y,p2.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[5],&yy[5],&zz[5]);
                            gluProject(p2.x,p2.y,p2.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[6],&yy[6],&zz[6]);
                            gluProject(p1.x,p2.y,p2.z,modelViewMatrix_, projectionMatrix_,viewport_,&xx[7],&yy[7],&zz[7]);

                            int nnn = 0;

                            for(int iii=0; iii<8; ++iii)
                            {
                                if(rect.contains(xx[iii], yy[iii], false))
                                    ++nnn;
                            }

                            if(nnn == 8)
                                m_idToAddInSelection << (CT_AbstractCloudIndexT<CT_Point>*)indexes;
                            else if (nnn > 0)
                                m_fakeG.drawPointCloud(PS_REPOSITORY->globalPointCloud(), indexes, 10);
                        }
                    }
                }
            }

            m_fakeG.endNewDraw();

            if(colorVBOManager() != NULL)
                colorVBOManager()->postDraw();

            unlockPaint();

            checkAndShowOpenGLErrors();
            return;
        }
        else
        {
            m_fakeG.beginNewDraw();
            m_fakeG.setDrawMode(G3DFakePainter::DrawPointsWithName);
            selectItems = false;
        }
    }
    else if(mustSelectFaces())
    {
        m_fakeG.beginNewDraw();
        m_fakeG.setDrawMode(G3DFakePainter::DrawFacesWithName);
        selectItems = false;
    }
    else if(mustSelectEdges())
    {
        m_fakeG.beginNewDraw();
        m_fakeG.setDrawMode(G3DFakePainter::DrawEdgesWithName);
        selectItems = false;
    }

    if(selectItems)
    {
        _g.beginNewDraw();
        _g.setDrawFastest(mustDrawFastestNow());
        _g.setPointSize(getOptions().getPointSize());
        _g.setPointFastestIncrement(m_fastestIncrementOptimizer.fastestIncrement());

        int i = 0;

        QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

        while(it.hasNext())
        {
            glPushName(i);
            it.next()->draw(*this, _g);
            glPopName();

            ++i;
        }

        _g.endNewDraw();
    }
    else
    {
        m_fakeG.setDrawFastest(mustDrawFastestNow());
        m_fakeG.setPointSize(getOptions().getPointSize());
        m_fakeG.setPointFastestIncrement(m_fastestIncrementOptimizer.fastestIncrement());

        QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

        while(it.hasNext())
            it.next()->draw(*this, m_fakeG);

        m_fakeG.endNewDraw();
    }

    if(colorVBOManager() != NULL)
        colorVBOManager()->postDraw();

    unlockPaint();

    checkAndShowOpenGLErrors();
}

void G3DGraphicsView::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    preDraw();
    draw();
    postDraw();
}

void G3DGraphicsView::endSelection(const QPoint &p)
{
    SelectionMode mode = selectionModeToBasic();

    if((mode != ADD_ONE)
        && (mode != REMOVE_ONE)
        && (mode != SELECT_ONE))
    {
        // Flush GL buffers
        glFlush();

        // Get the number of objects that were seen through the pick matrix frustum. Reset GL_RENDER mode.
        GLint nbHits = glRenderMode(GL_RENDER);

        if(mode == GraphicsViewInterface::SELECT)
            removeAllIdFromSelection();

        if(mustSelectPoints())
        {
            if(mode == REMOVE)
                m_pointsSelectionManager->beginRemoveMultipleIDFromSelection(nbHits);

            // Interpret results : each object created 4 values in the selectBuffer().
            // (selectBuffer())[4*i+3] is the id pushed on the stack.
            for (int i=0; i<nbHits; ++i)
            {
                switch (mode)
                {
                    case GraphicsViewInterface::SELECT :
                    case ADD    : addPointsIDToSelection((selectBuffer())[4*i+3]); break;
                    case REMOVE : removePointsIDFromSelection((selectBuffer())[4*i+3]);  break;
                    default : break;
                }
            }

            if(mode == REMOVE)
            {
                m_pointsSelectionManager->endRemoveMultipleIDFromSelection();
                m_pointsSelectionManager->removeCloudIndexFromSelection(m_idToAddInSelection);
            }
            else if((mode == ADD) || (mode == GraphicsViewInterface::SELECT))
            {
                m_pointsSelectionManager->addCloudIndexToSelection(m_idToAddInSelection);

                if(nbHits > 0)
                    setLastPointIdSelected((selectBuffer())[4*(nbHits-1)+3]);
            }
        }
        else if(mustSelectFaces())
        {
            if(mode == REMOVE)
                m_facesSelectionManager->beginRemoveMultipleIDFromSelection(nbHits);

            // Interpret results : each object created 4 values in the selectBuffer().
            // (selectBuffer())[4*i+3] is the id pushed on the stack.
            for (int i=0; i<nbHits; ++i)
            {
                switch (mode)
                {
                    case GraphicsViewInterface::SELECT :
                    case ADD    : addFacesIDToSelection((selectBuffer())[4*i+3]); break;
                    case REMOVE : removeFacesIDFromSelection((selectBuffer())[4*i+3]);  break;
                    default : break;
                }
            }

            if(mode == REMOVE)
            {
                m_facesSelectionManager->endRemoveMultipleIDFromSelection();
                m_facesSelectionManager->removeCloudIndexFromSelection(m_idToAddInSelection);
            }
            else if((mode == ADD) || (mode == GraphicsViewInterface::SELECT))
            {
                m_facesSelectionManager->addCloudIndexToSelection(m_idToAddInSelection);

                if(nbHits > 0)
                    setLastFaceIdSelected((selectBuffer())[4*(nbHits-1)+3]);
            }
        }
        else if(mustSelectEdges())
        {
            if(mode == REMOVE)
                m_edgesSelectionManager->beginRemoveMultipleIDFromSelection(nbHits);

            // Interpret results : each object created 4 values in the selectBuffer().
            // (selectBuffer())[4*i+3] is the id pushed on the stack.
            for (int i=0; i<nbHits; ++i)
            {
                switch (mode)
                {
                    case GraphicsViewInterface::SELECT :
                    case ADD    : addEdgesIDToSelection((selectBuffer())[4*i+3]); break;
                    case REMOVE : removeEdgesIDFromSelection((selectBuffer())[4*i+3]);  break;
                    default : break;
                }
            }

            if(mode == REMOVE)
            {
                m_edgesSelectionManager->endRemoveMultipleIDFromSelection();
                m_edgesSelectionManager->removeCloudIndexFromSelection(m_idToAddInSelection);
            }
            else if((mode == ADD) || (mode == GraphicsViewInterface::SELECT))
            {
                m_edgesSelectionManager->addCloudIndexToSelection(m_idToAddInSelection);

                if(nbHits > 0)
                    setLastEdgeIdSelected((selectBuffer())[4*(nbHits-1)+3]);
            }
        }
        else
        {
            // Interpret results : each object created 4 values in the selectBuffer().
            // (selectBuffer())[4*i+3] is the id pushed on the stack.
            for (int i=0; i<nbHits; ++i)
            {
                switch (mode)
                {
                    case GraphicsViewInterface::SELECT :
                    case ADD    : addItemsIDToSelection((selectBuffer())[4*i+3]); break;
                    case REMOVE : removeItemsIDFromSelection((selectBuffer())[4*i+3]);  break;
                    default : break;
                }
            }

            if(((mode == ADD) || (mode == GraphicsViewInterface::SELECT)) && (nbHits > 0))
                setLastItemIdSelected((selectBuffer())[4*(nbHits-1)+3]);
        }

        setSelectionMode(NONE);
        m_idToAddInSelection.clear();
    }
    else
    {
        QGLViewer::endSelection(p);
    }
}

void G3DGraphicsView::postSelection(const QPoint& point)
{
    SelectionMode mode = selectionModeToBasic();

    if((mode == ADD_ONE)
        || (mode == REMOVE_ONE)
        || (mode == SELECT_ONE))
    {
        if(mode == SELECT_ONE)
        {
            removeAllIdFromSelection();
        }

        if(selectedName() != -1)
        {
            if((mode == ADD_ONE)
                || (mode == SELECT_ONE))
            {
                int s = selectedName();
                addIdToSelection(s);

                if(mustSelectItems())
                    setLastItemIdSelected(s);
            }
            else if(mode == REMOVE_ONE)
            {
                removeIdFromSelection(selectedName());
            }
        }
    }

    setSelectionMode(NONE);

    QGLViewer::postSelection(point);
}

bool G3DGraphicsView::event(QEvent *e)
{
    actionsHandler()->eventToAction(e);

    return QGLViewer::event(e);
}

void G3DGraphicsView::enterEvent(QEvent *e)
{
    if(((GDocumentView&)getDocumentView()).getSubWindow()->isActiveWindow())
        actionsHandler()->enterEvent(e);

    QGLViewer::enterEvent(e);
}

void G3DGraphicsView::leaveEvent(QEvent *e)
{
    if(((GDocumentView&)getDocumentView()).getSubWindow()->isActiveWindow())
        actionsHandler()->leaveEvent(e);

    QGLViewer::leaveEvent(e);
}

void G3DGraphicsView::mousePressEvent(QMouseEvent *e)
{
    _forceDrawMode = false;

    if(!actionsHandler()->mousePressEvent(e))
    {
        if((e->button() == Qt::RightButton) && (e->modifiers() & Qt::CTRL))
            showContextMenu(mapToGlobal(e->pos()));
        else
            QGLViewer::mousePressEvent(e);
    }

    if(!_forceDrawMode)
    {
        setDrawModeChangeTime(0);
        internalSetDrawMode(GraphicsViewInterface::FAST);
    }
}

void G3DGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    _forceDrawMode = false;

    if(!actionsHandler()->mouseMoveEvent(e))
        QGLViewer::mouseMoveEvent(e);

    if(!_forceDrawMode
            && (e->buttons() != Qt::NoButton))
    {
        setDrawModeChangeTime(0);
        internalSetDrawMode(GraphicsViewInterface::FAST);
    }
}

void G3DGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    _forceDrawMode = false;

    if(!actionsHandler()->mouseReleaseEvent(e))
        QGLViewer::mouseReleaseEvent(e);

    if(!_forceDrawMode)
    {
        setDrawModeChangeTime(getOptions().getFastDrawTime());
        internalSetDrawMode(GraphicsViewInterface::FAST);
    }

}

void G3DGraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{
    _forceDrawMode = false;

    if(!actionsHandler()->mouseDoubleClickEvent(e))
        QGLViewer::mouseDoubleClickEvent(e);

    if(!_forceDrawMode)
    {
        setDrawModeChangeTime(getOptions().getFastDrawTime());
        internalSetDrawMode(GraphicsViewInterface::FAST);
    }
}

void G3DGraphicsView::wheelEvent(QWheelEvent *e)
{
    _forceDrawMode = false;

    if(!actionsHandler()->wheelEvent(e))
        QGLViewer::wheelEvent(e);

    if(!_forceDrawMode)
    {
        setDrawModeChangeTime(getOptions().getFastDrawTime());
        internalSetDrawMode(GraphicsViewInterface::FAST);
    }
}

void G3DGraphicsView::keyPressEvent(QKeyEvent *e)
{
    /*if(e->key() == Qt::Key_Delete)
    {
        m_pointsSelectionManager->setAllSelectedInvisible();
        redraw();
        return;
    }*/

    if(!actionsHandler()->keyPressEvent(e))
        QGLViewer::keyPressEvent(e);
}

void G3DGraphicsView::keyReleaseEvent(QKeyEvent *e)
{
    if(!actionsHandler()->keyReleaseEvent(e))
        QGLViewer::keyReleaseEvent(e);
}

void G3DGraphicsView::setDrawModeStartTimerAndRedraw()
{
    if(!_forceDrawMode)
        internalSetDrawMode(FAST);

    startRedrawTimer();

    emit mustRedraw();
}

void G3DGraphicsView::changeStateFileName()
{
    setStateFileName(_2dActive ? ".qglviewer2d.xml" : ".qglviewer3d.xml");
}

bool G3DGraphicsView::mustSelectPoints() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_POINTS) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_POINT));
}

bool G3DGraphicsView::mustSelectFaces() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_FACES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_FACE));
}

bool G3DGraphicsView::mustSelectEdges() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT_EDGES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_EDGE));
}

bool G3DGraphicsView::mustSelectItems() const
{
    return ((selectionMode() >= GraphicsViewInterface::SELECT) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE));
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> G3DGraphicsView::getSelectedPoints() const
{
    return m_pointsSelectionManager->selected();
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> G3DGraphicsView::getSelectedFaces() const
{
    return m_facesSelectionManager->selected();
}

QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> G3DGraphicsView::getSelectedEdges() const
{
    return m_edgesSelectionManager->selected();
}

GraphicsViewInterface::SelectionMode G3DGraphicsView::selectionModeToBasic() const
{
    int m = selectionMode();

    while(m > REMOVE_ONE)
        m -= REMOVE_ONE;

    return (GraphicsViewInterface::SelectionMode)m;
}

void G3DGraphicsView::checkAndShowOpenGLErrors()
{
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        GUI_LOG->addMessage(LogInterface::error, LogInterface::gui, QString("OpenGL error (%1) : %2").arg((int)err).arg((char*)gluErrorString(err)));
    }
}

void G3DGraphicsView::startRedrawTimer()
{
    if(drawModeChangeTime() > 0)
        _timerChangeDrawMode.start();
}

QMenu* G3DGraphicsView::constructContextMenu() const
{
    QMenu *menu = new QMenu();
    QMenu *menuColorPoints = menu->addMenu(tr("Colorer les points par..."));
    QMenu *menuColorFaces = menu->addMenu(tr("Colorer les faces par..."));
    QMenu *menuColorEdges = menu->addMenu(tr("Colorer les edges par..."));

    QList<CT_VirtualAbstractStep*> steps = GUI_MANAGER->getStepManager()->getStepRootList();

    while(!steps.isEmpty())
    {
        CT_VirtualAbstractStep *st = steps.takeFirst();

        constructContextMenuAction<CT_AbstractPointsAttributes>(menuColorPoints, st);
        constructContextMenuAction<CT_AbstractFaceAttributes>(menuColorFaces, st);
        constructContextMenuAction<CT_AbstractEdgeAttributes>(menuColorEdges, st);
    }

    if(menuColorPoints->isEmpty())
        delete menuColorPoints;
    else {
        menuColorPoints->addSeparator();
        menuColorPoints->addAction(tr("Configurer"), m_docGV, SLOT(showAttributesOptions()));
    }

    if(menuColorFaces->isEmpty())
        delete menuColorFaces;
    else {
        menuColorFaces->addSeparator();
        menuColorFaces->addAction(tr("Configurer"), m_docGV, SLOT(showAttributesOptions()));
    }

    if(menuColorEdges->isEmpty())
        delete menuColorEdges;
    else {
        menuColorEdges->addSeparator();
        menuColorEdges->addAction(tr("Configurer"), m_docGV, SLOT(showAttributesOptions()));
    }

    return menu;
}

template<typename CT_TypeAttributes>
void G3DGraphicsView::constructContextMenuAction(QMenu *menu, CT_VirtualAbstractStep *st) const
{
    DM_AttributesBuildingCollectionT<CT_TypeAttributes> builderPoints;
    builderPoints.buildFrom(st);
    QListIterator<CT_TypeAttributes*> it(builderPoints.attributesCollection());

    while(it.hasNext())
    {
        CT_TypeAttributes *pa = it.next();
        DM_AbstractAttributes *aa = attributesManager()->getAttributesFromInterface(pa);

        if(aa == NULL)
        {
            if(dynamic_cast<CT_AbstractAttributesScalar*>(pa) != NULL) {
                DM_AttributesScalarT<CT_TypeAttributes> *tmp = new DM_AttributesScalarT<CT_TypeAttributes>();
                tmp->setTypeAttributes(pa, dynamic_cast<CT_AbstractAttributesScalar*>(pa));
                aa = tmp;
            } else if(dynamic_cast<CT_AttributesColor*>(pa) != NULL) {
                DM_AttributesColorT<CT_TypeAttributes> *tmp = new DM_AttributesColorT<CT_TypeAttributes>();
                tmp->setTypeAttributes(pa, dynamic_cast<CT_AttributesColor*>(pa));
                aa = tmp;
            }

            if(aa != NULL)
                attributesManager()->addAttributes(aa);
        }
        else
        {
            if(dynamic_cast<CT_AttributesNormal*>(aa->abstractAttributes()) != NULL)
                aa = NULL;
        }

        if(aa != NULL)
        {
            QAction *act = menu->addAction(aa->displayableName(), this, SLOT(applyAttributes()));
            act->setData(qVariantFromValue((void*)aa));
        }
    }
}

void G3DGraphicsView::internalSetDrawMode(GraphicsViewInterface::DrawMode dMode)
{
    DM_GraphicsViewOptions &options = dynamic_cast<DM_GraphicsViewOptions&>(GGraphicsView::getOptions());

    bool valid = false;

    if(options.drawFastest() == DM_GraphicsViewOptions::Always)
    {
        if(dMode == FAST)
            valid = true;
    }
    else if(options.drawFastest() == DM_GraphicsViewOptions::Never)
    {
        if(dMode == NORMAL)
            valid = true;
    }
    else
        valid = true;

    if(valid)
    {
        _forceDrawMode = false;
        _drawModeToUse = dMode;

        startRedrawTimer();
    }
}

void G3DGraphicsView::changeDrawMethodToNormal()
{
    internalSetDrawMode(NORMAL);

    _timerChangeDrawMode.stop();

    if(drawMode() != NORMAL)
        emit mustRedraw();
}

void G3DGraphicsView::initIndexCloudRegistered()
{
    m_pointsSelectionManager->init();
    m_facesSelectionManager->init();
    m_edgesSelectionManager->init();
}

void G3DGraphicsView::itemDrawableToBeRemoved(CT_AbstractItemDrawable &item)
{
    lockPaint();

    G3DFakePainter fakeP;

    fakeP.beginNewDraw();
    fakeP.setDrawMode(G3DFakePainter::BackupPointCloudIndex | G3DFakePainter::BackupFaceCloudIndex | G3DFakePainter::BackupEdgeCloudIndex);

    item.draw(*this, fakeP);

    fakeP.endNewDraw();

    m_pointsSelectionManager->removeCloudIndexFromSelection(fakeP.pointCloudIndexBackup());
    m_edgesSelectionManager->removeCloudIndexFromSelection(fakeP.edgeCloudIndexBackup());
    m_facesSelectionManager->removeCloudIndexFromSelection(fakeP.faceCloudIndexBackup());

    unlockPaint();
}

void G3DGraphicsView::applyAttributes()
{
    QAction *act = (QAction*)sender();
    DM_AbstractAttributes *aa = (DM_AbstractAttributes*)act->data().value<void*>();

    m_docGV->applyAttributes(aa);
}

// SIGNAL EMITTER

void G3DGraphicsViewSignalEmitter::setGraphicsView(const G3DGraphicsView *gv)
{
    m_graphicsView = (G3DGraphicsView*)gv;
}

GraphicsViewInterface *G3DGraphicsViewSignalEmitter::graphicsView() const
{
    return m_graphicsView;
}

void G3DGraphicsViewSignalEmitter::emitDrawingStarted()
{
    emit drawingStarted();
}

void G3DGraphicsViewSignalEmitter::emitDrawingFinished()
{
    emit drawingFinished();
}
