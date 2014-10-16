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

#include "ct_global/ct_context.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include "dm_iteminfoforgraphics.h"

#include <limits>

G3DGraphicsView::G3DGraphicsView(QWidget *parent) : QGLViewer(QGLFormat(QGL::SampleBuffers), parent), GGraphicsView()
{
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);

    m_painter = NULL;

    _mutex = new QMutex(QMutex::Recursive);
    _g.setGraphicsView(this);
    m_fakeG.setGraphicsView(this);

    _selectionMode = NONE;
    _drawModeToUse = NORMAL;
    _drawModeUsed = FAST;
    _2dActive = false;
    _forceDrawMode = false;
    m_selectOctreeCells = false;

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
    GDocumentViewForGraphics *gv = dynamic_cast<GDocumentViewForGraphics*>((DM_DocumentView*)doc);

    m_pointsSelectionManager->setDocument(gv);
    m_facesSelectionManager->setDocument(gv);
    m_edgesSelectionManager->setDocument(gv);

    connect(gv, SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), this, SLOT(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), Qt::DirectConnection);

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
        return _g.currentPointCloudColor();
    else if(type == GraphicsViewInterface::CFaceCloud)
        return _g.currentFaceCloudColor();
    else if(type == GraphicsViewInterface::CEdgeCloud)
        return _g.currentEdgeCloudColor();

    return QSharedPointer<CT_StandardColorCloudRegistered>(NULL);
}

QSharedPointer<CT_StandardNormalCloudRegistered> G3DGraphicsView::normalCloudOf(GraphicsViewInterface::NormalCloudType type) const
{
    if(type == GraphicsViewInterface::NPointCloud)
        return _g.currentPointCloudNormal();
    else if(type == GraphicsViewInterface::NFaceCloud)
        return _g.currentFaceCloudNormal();
    else if(type == GraphicsViewInterface::NEdgeCloud)
        return _g.currentEdgeCloudNormal();

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
    /*return ((getOptions().drawFastest() == DM_GraphicsViewOptions::Always)
            || (((drawMode() == FAST) || (QGLViewer::camera()->frame()->isSpinning()))
                && (getOptions().drawFastest() != DM_GraphicsViewOptions::Never)));*/
    return (drawModeToUse() == FAST);
}

void G3DGraphicsView::setCurrentPointCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    _g.setCurrentPointCloudColor(cc);
}

void G3DGraphicsView::setCurrentFaceCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    _g.setCurrentFaceCloudColor(cc);
}

void G3DGraphicsView::setCurrentEdgeCloudColor(QSharedPointer<CT_StandardColorCloudRegistered> cc)
{
    _g.setCurrentEdgeCloudColor(cc);
}

void G3DGraphicsView::setCurrentPointCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    _g.setCurrentPointCloudNormal(nn);
}

void G3DGraphicsView::setCurrentFaceCloudNormal(QSharedPointer<CT_StandardNormalCloudRegistered> nn)
{
    _g.setCurrentFaceCloudNormal(nn);
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
    if(!_2dActive)
        restoreStateFromFile();

    initOptions();
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
}

void G3DGraphicsView::preDraw()
{
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
    // Restore OpenGL state
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    drawOverlay(*m_painter);

    m_painter->end();

    delete m_painter;
    m_painter = NULL;

    QGLViewer::postDraw();

    _drawModeUsed = _drawModeToUse;
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

    _g.setColor(Qt::white);
    _g.setPointSize(getOptions().getPointSize());
    _g.beginNewDraw();

    OctreeController *octreeC = (OctreeController*)(((GDocumentViewForGraphics&)getDocumentView()).octreeOfPoints());

    const DM_GraphicsViewOptions &options = ((const G3DGraphicsView*)this)->getOptions();

    if(octreeC->hasElements() && !octreeC->mustBeReconstructed() && options.showOctree())
        _g.drawOctreeOfPoints(octreeC, PainterInterface::DrawOctree);

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
            _g.setUseColorCloudForPoints(m_useColorCloud);
            _g.setUseColorCloudForFaces(m_useColorCloud);
            _g.setUseColorCloudForEdges(m_useColorCloud);

            if(info != NULL)
                _g.setColor(info->color());

            item->draw(*this, _g);
        }
    }

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

    bool selectItems = true;
    bool hasOctree = false;

    if(m_selectOctreeCells)
    {
        m_selectOctreeCells = false;
        m_fakeG.beginNewDraw();
        m_fakeG.setDrawMode(G3DFakePainter::DrawPointsWithName);
        selectItems = false;
        hasOctree = true;
    }
    else if(mustSelectPoints())
    {
        selectItems = false;

        OctreeController *octreeC = (OctreeController*)(((GDocumentViewForGraphics&)getDocumentView()).octreeOfPoints());
        hasOctree = (octreeC->hasElements() && !octreeC->mustBeReconstructed());

        if(hasOctree)
        {
            m_selectOctreeCells = true;
            _g.beginNewDraw();
        }
        else
        {
            m_fakeG.beginNewDraw();
            m_fakeG.setDrawMode(G3DFakePainter::DrawPointsWithName);
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
        if(!m_selectOctreeCells)
        {
            m_fakeG.setDrawFastest(mustDrawFastestNow());
            m_fakeG.setPointSize(getOptions().getPointSize());
        }

        if(!hasOctree)
        {
            QListIterator<CT_AbstractItemDrawable*> it(getDocumentView().getItemDrawable());

            while(it.hasNext())
                it.next()->draw(*this, m_fakeG);

            m_fakeG.endNewDraw();
        }
        else
        {
            OctreeController *octree = (OctreeController*)(((GDocumentViewForGraphics&)getDocumentView()).octreeOfPoints());

            if(m_selectOctreeCells)
            {
                QVector3D min = octree->octreeMinCorner();
                double cellSize = octree->cellsSize();
                int s = octree->numberOfCells();

                GLuint i = 0;

                for(int x=0; x<s; ++x)
                {
                    for(int y=0; y<s; ++y)
                    {
                        for(int z=0; z<s; ++z)
                        {
                            const CT_AbstractCloudIndexT<CT_Point> *indexes = dynamic_cast<const CT_AbstractCloudIndexT<CT_Point>*>(octree->at(x, y, z));

                            if((indexes != NULL) && octree->isCellVisibleInFrustrum(x, y, z, m_planeCoefficients))
                            {
                                QVector3D p1(min.x()+(x*cellSize), min.y()+(y*cellSize), min.z()+(z*cellSize));
                                QVector3D p2(min.x()+((x+1)*cellSize), min.y()+((y+1)*cellSize), min.z()+((z+1)*cellSize));

                                glPushName(i);
                                _g.drawCube(p1.x(), p1.y(), p1.z(), p2.x(), p2.y(), p2.z(), GL_FRONT_AND_BACK, GL_FILL);
                                glPopName();
                            }

                            ++i;
                        }
                    }
                }

                _g.endNewDraw();
                m_octreeCellsSelected.clear();
                endSelection(m_centerPointOfSelection);
                postSelection(m_centerPointOfSelection);
                select(m_centerPointOfSelection);
            }
            else
            {
                GLuint size = octree->numberOfCells();
                GLuint size2 = size*size;
                GLuint x, y ,z;
                QListIterator<GLuint> it(m_octreeCellsSelected);

                while(it.hasNext())
                {
                    GLuint index = it.next();

                    x = index / size2;
                    y = (index - (x * size2)) / size;
                    z = index - (y*size) - (x*size2);

                    const CT_AbstractCloudIndexT<CT_Point> *indexes = dynamic_cast<const CT_AbstractCloudIndexT<CT_Point>*>(octree->at(x, y, z));

                    m_fakeG.drawPointCloud(PS_REPOSITORY->globalPointCloud(), indexes, 10);
                }

                m_fakeG.endNewDraw();
            }
        }
    }

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

        if (nbHits > 0)
        {
            if(m_selectOctreeCells)
            {
                for (int i=0; i<nbHits; ++i)
                    m_octreeCellsSelected.append((selectBuffer())[4*i+3]);
            }
            else if(mustSelectPoints())
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
                    m_pointsSelectionManager->endRemoveMultipleIDFromSelection();
                else if(((mode == ADD) || (mode == GraphicsViewInterface::SELECT)) && (nbHits > 0))
                    setLastPointIdSelected((selectBuffer())[4*(nbHits-1)+3]);
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
                    m_facesSelectionManager->endRemoveMultipleIDFromSelection();
                else if(((mode == ADD) || (mode == GraphicsViewInterface::SELECT)) && (nbHits > 0))
                    setLastFaceIdSelected((selectBuffer())[4*(nbHits-1)+3]);
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
                    m_edgesSelectionManager->endRemoveMultipleIDFromSelection();
                else if(((mode == ADD) || (mode == GraphicsViewInterface::SELECT)) && (nbHits > 0))
                    setLastEdgeIdSelected((selectBuffer())[4*(nbHits-1)+3]);
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
        }

        if(!m_selectOctreeCells)
            setSelectionMode(NONE);
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
            if(m_selectOctreeCells)
            {
                m_octreeCellsSelected.append(selectedName());
            }
            else if((mode == ADD_ONE)
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

    if(!m_selectOctreeCells)
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
        QGLViewer::mousePressEvent(e); 

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

    m_fakeG.beginNewDraw();
    m_fakeG.setDrawMode(G3DFakePainter::BackupPointCloudIndex | G3DFakePainter::BackupFaceCloudIndex | G3DFakePainter::BackupEdgeCloudIndex);

    item.draw(*this, m_fakeG);

    m_fakeG.endNewDraw();

    m_pointsSelectionManager->removeCloudIndexFromSelection(m_fakeG.pointCloudIndexBackup());
    m_edgesSelectionManager->removeCloudIndexFromSelection(m_fakeG.edgeCloudIndexBackup());
    m_facesSelectionManager->removeCloudIndexFromSelection(m_fakeG.faceCloudIndexBackup());

    unlockPaint();
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
