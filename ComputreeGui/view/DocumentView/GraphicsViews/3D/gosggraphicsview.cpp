#include "gosggraphicsview.h"

#include <osg/Geometry>
#include <osgManipulator/TrackballDragger>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Point>
#include <osgDB/WriteFile>

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/3D/Camera/dm_2dcameramanipulator.h"
#include "view/DocumentView/GraphicsViews/3D/Camera/dm_3dcameramanipulator.h"

#include "view/DocumentView/GraphicsViews/3D/dm_osgscenemanager.h"
#include "view/DocumentView/GraphicsViews/3D/Picking/dm_osgpicker.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_fakepainter.h"

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include <QApplication>
#include <QEvent>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>

uint GOsgGraphicsView::NUMBER_OF_VIEWS = 0;

/**
 * @brief Just create a line (beam) from 0, 0, 0 to x, y, z with color qtColor and return it
 */
osg::ref_ptr<osg::Geometry> createBeam(double x, double y, double z, QColor qtColor)
{
    osg::Vec3 sp(0,0,0);
    osg::Vec3 ep(x,y,z);

    osg::ref_ptr<osg::Geometry> beam(new osg::Geometry);

    osg::ref_ptr<osg::Vec3Array> points(new osg::Vec3Array);

    points->push_back(sp);
    points->push_back(ep);

    osg::ref_ptr<osg::Vec4Array> color(new osg::Vec4Array);

    color->push_back(osg::Vec4(qtColor.redF(),qtColor.greenF(),qtColor.blueF(),qtColor.alphaF()));

    beam->setVertexArray(points.get());
    beam->setColorArray(color.get());
    beam->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    beam->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2));

    return beam;
}

osgGA::KeySwitchMatrixManipulator* createManipulatorSwitch() {
    osgGA::KeySwitchMatrixManipulator *sw = new osgGA::KeySwitchMatrixManipulator();
    sw->addMatrixManipulator('1', "3D", new DM_3DCameraManipulator());
    sw->addMatrixManipulator('2', "2D", new DM_2DCameraManipulator());
    sw->setHomePosition(osg::Vec3(6, 0, 0), osg::Vec3(0, 0, 0), osg::Z_AXIS);

    return sw;
}

osg::Camera* createMainCamera(osg::GraphicsContext *gc) {
    const osg::GraphicsContext::Traits* traits = gc->getTraits();

    osg::Camera* camera = new osg::Camera;
    camera->setCullingMode(camera->getCullingMode() & ~osg::CullSettings::SMALL_FEATURE_CULLING);
    camera->setClearColor( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.f ) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    camera->setGraphicsContext(gc);

    return camera;
}

osgViewer::GraphicsWindowEmbedded* createGraphicsWindow(QWidget *w) {
    osgViewer::GraphicsWindowEmbedded *gw = new osgViewer::GraphicsWindowEmbedded(w->x(), w->y(), w->width(), w->height());
    return gw;
}

osgViewer::View* createView(osg::Camera *camera, osgGA::CameraManipulator *cameraManipulator, osg::Node *scene) {
    osgViewer::View *view = new osgViewer::View;
    view->setCamera(camera);
    view->setCameraManipulator(cameraManipulator);
    view->setSceneData(scene);

    return view;
}

osgViewer::CompositeViewer* createCompositeViewer(osgViewer::View *view) {
    osgViewer::CompositeViewer *cv = new osgViewer::CompositeViewer;
    cv->addView(view);
    cv->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
    cv->setKeyEventSetsDone(0); // disable the default setting of viewer.done() by pressing Escape.
    cv->realize();

    return cv;
}

osg::Node* createAxis()
{
    osg::Geode *geode = new osg::Geode;

    geode->addDrawable(createBeam(1, 0, 0, Qt::red).get());
    geode->addDrawable(createBeam(0, 1, 0, Qt::green).get());
    geode->addDrawable(createBeam(0, 0, 1, Qt::blue).get());

    return geode;
}

osg::Group* createScene(osg::Node *axis) {
    osg::Point *point = new osg::Point;
    point->setSize(1);

    osg::Group *scene = new osg::Group;
    scene->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
    scene->getOrCreateStateSet()->setAttribute(point);

    if(axis != NULL)
        scene->addChild(axis);

    return scene;
}

osgViewer::ScreenCaptureHandler* createScreenCaptureHandler() {
    osgViewer::ScreenCaptureHandler *screenCapture = new osgViewer::ScreenCaptureHandler(new GOsgCaptureOperation);

    return screenCapture;
}

/////////////////// GOsgGraphicsView /////////////////////

GOsgGraphicsView::GOsgGraphicsView(QWidget *parent) : Q_GL_WIDGET( parent ), GGraphicsView()
{
    m_mutex = new QMutex(QMutex::Recursive);

    m_signalEmitter.setGraphicsView(this);

    m_mouseButtonPressed = false;
    m_defaultStateFileName = "osgViewer.config";
    m_uniqueIndex = NUMBER_OF_VIEWS++;

    m_graphicsWindow = createGraphicsWindow(this);

    m_manSwitch = createManipulatorSwitch();
    m_axis = createAxis();
    m_scene = createScene(m_axis.get());

    m_captureScreenHandler = createScreenCaptureHandler();
    m_view = createView(createMainCamera(m_graphicsWindow), m_manSwitch.get(), m_scene.get());
    m_view->addEventHandler(m_captureScreenHandler);
    m_view->addEventHandler(new osgViewer::StatsHandler);

    m_compositeViewer = createCompositeViewer(m_view);

    m_sceneManager = NULL;

    m_camController.setView(this);
    m_camController.setRealCameraManipulator((const osgGA::OrbitManipulator*)m_manSwitch->getCurrentMatrixManipulator());

    m_picker = NULL;

    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(100, 100);
    setMouseTracking(true);

    //DM_PainterToOsgElements g;
    /*double initPos = 1000000;

    g.drawCube(initPos, initPos, initPos, initPos+.001, initPos+.001, initPos+.001);
    g.drawCircle(initPos, initPos, initPos, 0.001);
    g.drawCircle3D(Eigen::Vector3d(initPos, initPos, initPos), Eigen::Vector3d(0.5, 0.5, 1), 0.001);
    g.drawCircle(initPos, initPos, initPos, 0.5);
    g.drawCylinder(initPos, initPos, initPos, 3, 5);
    g.drawCylinder3D(Eigen::Vector3d(initPos, initPos, initPos), Eigen::Vector3d(0.5, 0.5, 1), 0.0005, 0.001);
    g.drawEllipse(initPos, initPos, initPos, 0.001, 0.0005);
    g.drawPoint(initPos, initPos, initPos);
    g.drawPoint(initPos, initPos, initPos+.0005);*/
    /*g.drawTriangle(0, 0, 0,
                    1, 1, 0,
                    -1, 1, 0);*/

    /*g.drawCylinder3D(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 1, 1), 0.1, 0.6);
    g.drawCylinder(2, 0, 0, 1, 2);
    g.drawCylinder3D(Eigen::Vector3d(0, 10, 0), Eigen::Vector3d(1, 0, 2), 0.3, 3.7);
    g.drawCylinder(0, 4, 0, 3, 5);

    for(int i=0; i<4; ++i)
        g.drawCylinder(0, 4, 0, 3, 5);

    g.drawPartOfSphere(0, 0, 0, 0.8, -M_PI, M_PI, -M_PI, M_PI, true);
    g.drawPartOfSphere(1, 1, 1, 0.2, 0, 360, 0, 360, false);

    ((DM_3DCameraManipulator*)m_manSwitch->getMatrixManipulatorWithIndex(0))->setCenter(osg::Vec3d(initPos, initPos, initPos));*/

    //m_scene->addChild(g.results().get());
}

GOsgGraphicsView::~GOsgGraphicsView()
{
    saveStateToFile();

    delete m_picker;
    delete m_sceneManager;
    delete m_mutex;
}

void GOsgGraphicsView::init()
{
    if(m_sceneManager == NULL) {
        m_sceneManager = new DM_OsgSceneManager(*this, m_scene);

        connect(actionsHandler(), SIGNAL(currentActionChanged(CT_AbstractAction*)), m_sceneManager, SLOT(setCurrentAction(CT_AbstractAction*)), Qt::DirectConnection);
    }

    if(m_picker == NULL)
        m_picker = new DM_OsgPicker(this, m_view, m_sceneManager);

    // disable axis from picking
    DM_OsgSceneManager::staticSetEnablePicking(m_axis.get(), false);

    connect(getDocumentView(), SIGNAL(itemDrawableAdded(CT_AbstractItemDrawable&)), m_sceneManager, SLOT(addItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);
    connect(getDocumentView(), SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), m_sceneManager, SLOT(removeItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);

    restoreStateFromFile();
}

void GOsgGraphicsView::stopSpinning()
{
    DM_3DCameraManipulator *manipulator = dynamic_cast<DM_3DCameraManipulator*>(m_manSwitch->getCurrentMatrixManipulator());

    if(manipulator != NULL) {
        osg::Quat rot(manipulator->getRotation());
        osg::Vec3d ctr(manipulator->getCenter());
        double dist(manipulator->getDistance());
        manipulator->home(0);
        manipulator->setDistance(dist);
        manipulator->setCenter(ctr);
        manipulator->setRotation(rot); // fairly long winded
    }
}

QString GOsgGraphicsView::stateFileName() const
{
    QString name = m_defaultStateFileName;

    if (!name.isEmpty() && m_uniqueIndex > 0)
    {
        QFileInfo fi(name);
        if (fi.suffix().isEmpty())
            name += QString::number(m_uniqueIndex);
        else
            name = fi.absolutePath() + '/' + fi.completeBaseName() + QString::number(m_uniqueIndex) + "." + fi.suffix();
    }

    return name;
}

void GOsgGraphicsView::saveStateToFile()
{
    QString name = stateFileName();

    if (name.isEmpty())
        return;

    QFileInfo fileInfo(name);

    if (fileInfo.isDir())
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : State file name (%1) references a directory instead of a file.").arg(name));
        return;
    }

    const QString dirName = fileInfo.absolutePath();
    if (!QFileInfo(dirName).exists())
    {
        QDir dir;
        if (!(dir.mkdir(dirName)))
        {
            GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : Unable to create directory %1").arg(dirName));
            return;
        }
    }

    // Write the DOM tree to file
    QFile f(name);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream out(&f);
        QDomDocument doc("OSGVIEWER");

        QDomElement nodeCustom = doc.createElement("GraphicsOptions");
        constGetOptionsInternal().saveToXml(nodeCustom, doc);
        doc.appendChild(nodeCustom);

        doc.save(out, 2);

        f.flush();
        f.close();
    }
    else
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : Unable to save to file %1").arg(name) + ":\n" + f.errorString());
}

bool GOsgGraphicsView::restoreStateFromFile()
{
    QString name = stateFileName();

    if (name.isEmpty())
        return false;

    QFileInfo fileInfo(name);

    if (!fileInfo.isFile())
        // No warning since it would be displayed at first start.
        return false;

    if (!fileInfo.isReadable())
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Problem in state restoration : File %1 is not readable.").arg(name));
        return false;
    }

    // Read the DOM tree form file
    QFile f(name);
    if (f.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        doc.setContent(&f);
        f.close();

        DM_GraphicsViewOptions opt;
        opt.updateFromOtherOptions(constGetOptionsInternal());

        QDomNodeList l = doc.elementsByTagName("GraphicsOptions");

        if(!l.isEmpty()) {
            QDomElement el = l.at(0).toElement();
            opt.loadFromXml(el);
        }

        setOptions(opt);
    }
    else
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Open file error : Unable to open file %1").arg(name) + ":\n" + f.errorString());
        return false;
    }

    return true;
}

void GOsgGraphicsView::active2dView(bool enable)
{
    if(enable) {
        // TODO : problem 2D camera !
        m_view->getCamera()->setViewMatrixAsLookAt(-osg::Z_AXIS*3, osg::Vec3(0, 0, 0), osg::Y_AXIS);
        m_view->getCamera()->setProjectionMatrixAsOrtho(-1, 1, -1, 1, 1.0f, 10000.0f);
        m_manSwitch->selectMatrixManipulator(1);
        m_camController.setRealCameraManipulator((osgGA::OrbitManipulator*)m_manSwitch->getCurrentMatrixManipulator());
    } else {
        m_view->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f );
        m_manSwitch->selectMatrixManipulator(0);
        m_camController.setRealCameraManipulator((osgGA::OrbitManipulator*)m_manSwitch->getCurrentMatrixManipulator());
    }
}

osgViewer::CompositeViewer* GOsgGraphicsView::getCompositeViewer() const
{
    return m_compositeViewer.get();
}

void GOsgGraphicsView::setCameraType(CameraInterface::CameraType type)
{
    if(!is2DView()) {
        if(type == CameraInterface::ORTHOGRAPHIC) {
            m_view->getCamera()->setProjectionMatrixAsOrtho(-1, 1, -1, 1, 1.0f, 10000.0f);
        } else {
            m_view->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f );
        }
    }
}

QWidget* GOsgGraphicsView::getViewWidget() const
{
    return const_cast<GOsgGraphicsView*>(this);
}

bool GOsgGraphicsView::is2DView() const
{
    return (dynamic_cast<DM_2DCameraManipulator*>(m_manSwitch->getCurrentMatrixManipulator()) != NULL);
}

void GOsgGraphicsView::setOptions(const DM_GraphicsViewOptions &newOptions)
{
    GGraphicsView::setOptions(newOptions);

    const DM_GraphicsViewOptions &opt = constGetOptionsInternal();

    DM_OsgSceneManager::staticSetVisible(m_axis.get(), opt.drawAxis());

    m_view->getCamera()->setClearColor(osg::Vec4(opt.getBackgroundColor().redF(),
                                                 opt.getBackgroundColor().greenF(),
                                                 opt.getBackgroundColor().blueF(),
                                                 opt.getBackgroundColor().alphaF()));

    osg::Point *point = (osg::Point*)m_scene->getOrCreateStateSet()->getAttribute(osg::StateAttribute::POINT);

    if(point == NULL) {
        point = new osg::Point;
        m_scene->getOrCreateStateSet()->setAttribute(point);
    }

    point->setSize(opt.getPointSize());
}

void GOsgGraphicsView::unlockPaint()
{
    m_mutex->unlock();
}

void GOsgGraphicsView::lockPaint()
{
    m_mutex->lock();
}

GraphicsViewSignalEmitterInterface *GOsgGraphicsView::signalEmitter() const
{
    return const_cast<GOsgGraphicsViewSignalEmitter*>(&m_signalEmitter);
}

QSharedPointer<CT_StandardColorCloudRegistered> GOsgGraphicsView::colorCloudOf(GraphicsViewInterface::ColorCloudType type) const
{
    if(type == GraphicsViewInterface::CPointCloud)
        return dynamic_cast<GDocumentViewForGraphics*>(getDocumentView())->getGlobalColorArrayRegisteredForPoints();

    // TODO : returns color cloud if type different that points
    return QSharedPointer<CT_StandardColorCloudRegistered>();
}

QSharedPointer<CT_StandardNormalCloudRegistered> GOsgGraphicsView::normalCloudOf(GraphicsViewInterface::NormalCloudType type) const
{
    // TODO : returns normal cloud of type passed in parameter
    return QSharedPointer<CT_StandardNormalCloudRegistered>();
}

CameraInterface* GOsgGraphicsView::camera() const
{
    return getCamera();
}

GraphicsViewInterface::GraphicsViewType GOsgGraphicsView::type() const
{
    if(is2DView())
        return GraphicsViewInterface::GV2D;

    return GraphicsViewInterface::GV3D;
}

void GOsgGraphicsView::setSelectRegionWidth(int width)
{
    m_picker->setSelectRegionWidth(width);
}

void GOsgGraphicsView::setSelectRegionHeight(int height)
{
    m_picker->setSelectRegionHeight(height);
}

void GOsgGraphicsView::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    m_picker->setSelectionMode(mode);
}

GraphicsViewInterface::SelectionMode GOsgGraphicsView::selectionMode() const
{
    return m_picker->selectionMode();
}

bool GOsgGraphicsView::mustSelectPoints() const
{
    return m_picker->mustSelectPoints();
}

bool GOsgGraphicsView::mustSelectEdges() const
{
    return m_picker->mustSelectEdges();
}

bool GOsgGraphicsView::mustSelectFaces() const
{
    return m_picker->mustSelectFaces();
}

bool GOsgGraphicsView::mustSelectItems() const
{
    return m_picker->mustSelectItems();
}

void GOsgGraphicsView::select(const QPoint &point)
{
    m_picker->select(QPoint(point.x(), height()-point.y()));
}

CT_SPCIR GOsgGraphicsView::getSelectedPoints() const
{
    return m_picker->getSelectedPoints();
}

CT_SFCIR GOsgGraphicsView::getSelectedFaces() const
{
    // TODO : find a way to get selected faces index
    return CT_SFCIR();
}

CT_SECIR GOsgGraphicsView::getSelectedEdges() const
{
    // TODO : find a way to get selected edges index
    return CT_SECIR();
}

QList<CT_AbstractItemDrawable *> GOsgGraphicsView::getSelectedItems() const
{
    return getDocumentView()->getSelectedItemDrawable();
}

void GOsgGraphicsView::addPointsIDToSelection(const size_t &id)
{
    m_picker->addPointIdToSelection(id);
}

void GOsgGraphicsView::addMultiplePointsIDToSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->addPointIdToSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::addFacesIDToSelection(const size_t &id)
{
    m_picker->addFaceIdToSelection(id);
}

void GOsgGraphicsView::addMultipleFacesIDToSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->addFaceIdToSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::addEdgesIDToSelection(const size_t &id)
{
    m_picker->addEdgeIdToSelection(id);
}

void GOsgGraphicsView::addMultipleEdgesIDToSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->addEdgeIdToSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::removePointsIDFromSelection(const size_t &id)
{
    m_picker->removePointIdFromSelection(id);
}

void GOsgGraphicsView::removeMultiplePointsIDFromSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->removePointIdFromSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::removeFacesIDFromSelection(const size_t &id)
{
    m_picker->removeFaceIdFromSelection(id);
}

void GOsgGraphicsView::removeMultipleFacesIDFromSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->removeFaceIdFromSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::removeEdgesIDFromSelection(const size_t &id)
{
    m_picker->removeEdgeIdFromSelection(id);
}

void GOsgGraphicsView::removeMultipleEdgesIDFromSelection(const std::vector<size_t> &idList)
{
    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end= idList.end();

    while(it != end) {
        m_picker->removeEdgeIdFromSelection(*it);
        ++it;
    }
}

void GOsgGraphicsView::setAllPointsSelected(bool select)
{
    m_picker->setAllPointsSelected(select);
}

void GOsgGraphicsView::setAllFacesSelected(bool select)
{
    m_picker->setAllFacesSelected(select);
}

void GOsgGraphicsView::setAllEdgesSelected(bool select)
{
    m_picker->setAllEdgesSelected(select);
}

size_t GOsgGraphicsView::countPoints()
{
    lockPaint();

    DM_FakePainter p;
    p.setComputingMode(DM_FakePainter::CountPoints);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView()->getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, p);

    unlockPaint();

    return p.nPoints();
}

size_t GOsgGraphicsView::countEdges()
{
    lockPaint();

    DM_FakePainter p;
    p.setComputingMode(DM_FakePainter::CountEdges);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView()->getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, p);

    unlockPaint();

    return p.nEdges();
}

size_t GOsgGraphicsView::countFaces()
{
    lockPaint();

    DM_FakePainter p;
    p.setComputingMode(DM_FakePainter::CountFaces);

    QListIterator<CT_AbstractItemDrawable*> it(getDocumentView()->getItemDrawable());

    while(it.hasNext())
        it.next()->draw(*this, p);

    unlockPaint();

    return p.nFaces();
}

size_t GOsgGraphicsView::countItems()
{
    return getDocumentView()->nItemDrawable();
}

Eigen::Vector3d GOsgGraphicsView::pointUnderPixel(const QPoint &pixel, bool &found) const
{
    // TODO : must test it
    float depth;
    const_cast<GOsgGraphicsView*>(this)->glReadPixels(pixel.x(), height()-pixel.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    found = depth < 1.0;

    osg::Matrix MVPW(m_view->getCamera()->getViewMatrix() *
                     m_view->getCamera()->getProjectionMatrix() *
                     m_view->getCamera()->getViewport()->computeWindowMatrix());

    osg::Matrixd inverseMVPW = osg::Matrixd::inverse(MVPW);

    osg::Vec3 point = osg::Vec3(pixel.x(), height()-pixel.y(), depth) * inverseMVPW;

    return Eigen::Vector3d(point.x(), point.y(), point.z());
}

void GOsgGraphicsView::convertClickToLine(const QPoint &pixel, Eigen::Vector3d &orig, Eigen::Vector3d &dir) const
{
    osg::Matrix MVPW(m_view->getCamera()->getViewMatrix() *
                     m_view->getCamera()->getProjectionMatrix() *
                     m_view->getCamera()->getViewport()->computeWindowMatrix());

    osg::Matrixd inverseMVPW = osg::Matrixd::inverse(MVPW);

    osg::Vec3 near_point = osg::Vec3(pixel.x(), height()-pixel.y(), 0.0f) * inverseMVPW;
    osg::Vec3 far_point  = osg::Vec3(pixel.x(), height()-pixel.y(), 1.0f) * inverseMVPW;

    orig[0] = near_point.x();
    orig[1] = near_point.y();
    orig[2] = near_point.z();

    dir[0] = far_point.x()-near_point.x();
    dir[1] = far_point.y()-near_point.y();
    dir[2] = far_point.z()-near_point.z();
}

void GOsgGraphicsView::convert3DPositionToPixel(const Eigen::Vector3d &position, QPoint &pixel) const
{
    osg::Matrix MVPW(m_view->getCamera()->getViewMatrix() *
                     m_view->getCamera()->getProjectionMatrix() *
                     m_view->getCamera()->getViewport()->computeWindowMatrix());

    osg::Vec3 posIn2D = osg::Vec3d(position[0], position[1], position[2]) * MVPW;

    pixel.setX(posIn2D.x());
    pixel.setY(height()-posIn2D.y());
}

int GOsgGraphicsView::width() const
{
    return Q_GL_WIDGET::width();
}

int GOsgGraphicsView::height() const
{
    return Q_GL_WIDGET::height();
}

void GOsgGraphicsView::resizeGL(int width, int height)
{
    getEventQueue()->windowResize( this->x(), this->y(), width, height );
    m_graphicsWindow->resized( this->x(), this->y(), width, height );
    onResize( width, height );
}

void GOsgGraphicsView::onResize(int width, int height)
{
  std::vector<osg::Camera*> cameras;
  m_compositeViewer->getCameras( cameras );

  assert( cameras.size() == 1);

  cameras[0]->setViewport( 0, 0, width, height );
}

osgGA::EventQueue *GOsgGraphicsView::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = m_graphicsWindow->getEventQueue();

    if( eventQueue )
        return eventQueue;
    else
        throw std::runtime_error( "Unable to obtain valid event queue");
}

void GOsgGraphicsView::initializeGL()
{
    Q_GL_FUNCTIONS_INIT();

    Q_GL_WIDGET::initializeGL();
}

void GOsgGraphicsView::paintEvent(QPaintEvent* /*event*/)
{
    m_signalEmitter.emitDrawingStarted();

    lockPaint();

    makeCurrent();

    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );

    paintGL();

    paintOverlay(painter);

    painter.end();

    doneCurrent();

    unlockPaint();

    m_signalEmitter.emitDrawingFinished();
}

void GOsgGraphicsView::paintGL()
{
    m_compositeViewer->frame();
}

void GOsgGraphicsView::paintOverlay(QPainter &painter)
{
    CT_AbstractActionForGraphicsView *action = dynamic_cast<CT_AbstractActionForGraphicsView*>(actionsHandler()->currentAction());

    if(action != NULL)
        action->drawOverlay(*this, painter);

    paintOverlayCameraInformation(painter);
}

void GOsgGraphicsView::paintOverlayCameraInformation(QPainter &painter)
{
    QFontMetrics metrics = fontMetrics();

    QList<QString> stringList;

    const DM_GraphicsViewOptions &options = constGetOptionsInternal();

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraPosition)
    {
        stringList.append(QString("x : %1").arg(getCamera()->x()));
        stringList.append(QString("y : %1").arg(getCamera()->y()));
        stringList.append(QString("z : %1").arg(getCamera()->z()));
    }

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraSceneCenter)
    {
        stringList.append(QString("cx : %1").arg(getCamera()->cx()));
        stringList.append(QString("cy : %1").arg(getCamera()->cy()));
        stringList.append(QString("cz : %1").arg(getCamera()->cz()));
    }

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::CameraViewDirection)
    {
        stringList.append(QString("rx : %1").arg(getCamera()->rx()));
        stringList.append(QString("ry : %1").arg(getCamera()->ry()));
        stringList.append(QString("rz : %1").arg(getCamera()->rz()));
    }

    if(options.getCameraInformationDisplayed() & DM_GraphicsViewOptions::FpsInformation) {
        osg::FrameStamp *fs = m_compositeViewer->getViewerFrameStamp();
        stringList.append(QString("fps : %1").arg((int)(fs->getFrameNumber()/fs->getReferenceTime())));
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
            maxWidth = metrics.width(QString("aaa : %1").arg(std::numeric_limits<double>::max()));

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

    /*painter.setPen( Qt::red );
    painter.drawLine(0, 0, width()/2, height()/2);*/
}

void GOsgGraphicsView::enterEvent(QEvent *event)
{
    if(actionsHandler()->enterEvent(event))
        return;
}

void GOsgGraphicsView::leaveEvent(QEvent *event)
{
    if(actionsHandler()->leaveEvent(event))
        return;
}

void GOsgGraphicsView::keyPressEvent( QKeyEvent* event )
{
    /*if(event->key() == Qt::Key_A) {

        osg::Program *program = new osg::Program;

        osg::ref_ptr<osg::Shader> shader = new osg::Shader( osg::Shader::VERTEX,    "#version 120\n"
                                                                                    "// selection color of points\n"
                                                                                    "uniform vec4 selectionColor;\n"
                                                                                    "// to check if it is selected\n"
                                                                                    "uniform int checkSelected;\n"
                                                                                    "// info of the point (is selected ?)\n"
                                                                                    "attribute float info;\n"
                                                                                    "void main(void)\n"
                                                                                    "{\n"
                                                                                    "    int infoInt = int(info);\n"
                                                                                    "    if(infoInt == checkSelected)\n"
                                                                                    "    {\n"
                                                                                    "         gl_FrontColor = selectionColor;\n"
                                                                                    "    }\n"
                                                                                    "    else\n"
                                                                                    "    {\n"
                                                                                    "         gl_FrontColor = gl_Color;\n"
                                                                                    "    }\n"
                                                                                    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
                                                                                    "}\n");
        program->addShader(shader);
        program->addBindAttribLocation("info", 6);

        osg::StateSet *ss = new osg::StateSet;
        ss->setAttributeAndModes(program, osg::StateAttribute::ON);

        QColor color = Qt::gray;

        ss->addUniform(new osg::Uniform("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())));
        ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));


        osg::Vec3Array *vArray = new osg::Vec3Array;
        vArray->resize(3);
        (*vArray)[0] = osg::Vec3(0, 0, 0);
        (*vArray)[1] = osg::Vec3(0, 1, 0);
        (*vArray)[2] = osg::Vec3(1, 0, 0);


        osg::Vec4Array *cArray = new osg::Vec4Array;
        cArray->resize(3);
        (*cArray)[0] = osg::Vec4(0, 0, 1, 1);
        (*cArray)[1] = osg::Vec4(1, 0, 0, 1);
        (*cArray)[2] = osg::Vec4(0, 1, 0, 1);

        osg::UByteArray *vaArray = new osg::UByteArray;
        vaArray->resize(3);
        (*vaArray)[0] = 1;
        (*vaArray)[1] = 0;
        (*vaArray)[2] = 1;

        osg::DrawElementsUInt *pSet = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);
        pSet->push_back(0);
        pSet->push_back(1);
        pSet->push_back(2);

        osg::Geometry *geo = new osg::Geometry;
        geo->setStateSet(ss);
        geo->setVertexArray(vArray);
        geo->setColorArray(cArray);
        geo->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
        geo->setVertexAttribArray(6, vaArray, osg::Array::BIND_PER_VERTEX);
        geo->setVertexAttribNormalize(6, false);
        geo->addPrimitiveSet(pSet);

        osg::Geode *geode = new osg::Geode;
        geode->setName("Triangle");
        geode->addDrawable(geo);


        m_scene->addChild(geode);
    }*/

    if(actionsHandler()->keyPressEvent(event))
        return;

    QString keyString   = event->text();
    if(!keyString.isEmpty()) {
        char keyData = keyString.at(0).toLatin1();
        this->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol( keyData ) );
    }
}

void GOsgGraphicsView::keyReleaseEvent( QKeyEvent* event )
{
    if(actionsHandler()->keyReleaseEvent(event))
        return;

    QString keyString   = event->text();
    if(!keyString.isEmpty()) {
        char keyData = keyString.at(0).toLatin1();
        this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( keyData ) );
    }
}

void GOsgGraphicsView::mouseMoveEvent( QMouseEvent* event )
{
    if(actionsHandler()->mouseMoveEvent(event))
        return;

    this->getEventQueue()->mouseMotion( static_cast<float>( event->x() ),
                                        static_cast<float>( event->y() ) );
}

void GOsgGraphicsView::mousePressEvent( QMouseEvent* event )
{
    if(actionsHandler()->mousePressEvent(event))
        return;

    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
      button = 1;
      break;

    case Qt::MiddleButton:
      button = 2;
      break;

    case Qt::RightButton:
      button = 3;
      break;

    default:
      break;
    }

    this->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() ),
                                             static_cast<float>( event->y() ),
                                             button );
}

void GOsgGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(actionsHandler()->mouseDoubleClickEvent(event))
        return;
}

void GOsgGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if(actionsHandler()->mouseReleaseEvent(event))
        return;

    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
      button = 1;
      break;

    case Qt::MiddleButton:
      button = 2;
      break;

    case Qt::RightButton:
      button = 3;
      break;

    default:
      break;
    }

    this->getEventQueue()->mouseButtonRelease( static_cast<float>( event->x() ),
                                               static_cast<float>( event->y() ),
                                               button );
}

void GOsgGraphicsView::wheelEvent( QWheelEvent* event )
{
    if(actionsHandler()->wheelEvent((QWheelEvent*)event))
        return;

    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                               : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->getEventQueue()->mouseScroll( motion );
}

bool GOsgGraphicsView::event(QEvent *event)
{
    bool handled = Q_GL_WIDGET::event( event );
    QEvent::Type evType = event->type();

    if(evType == QEvent::MouseButtonPress) {
        m_mouseButtonPressed = true;
    } else if(evType == QEvent::MouseButtonRelease) {
        m_mouseButtonPressed = false;
        stopSpinning();
    } else if(evType == QEvent::Wheel) {

    } else {
        if(actionsHandler()->eventToAction((QEvent*)event))
            return true;
    }

    switch( evType )
    {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::Wheel: this->update();
            break;

        case QEvent::MouseMove: if(m_mouseButtonPressed) { this->update(); }
            break;

        default:
            break;
    }

    return handled;
}

void GOsgGraphicsView::redraw()
{
    update();
}

DM_GraphicsViewCamera* GOsgGraphicsView::getCamera() const
{
    return const_cast<G3DCameraController*>(&m_camController);
}

void GOsgGraphicsView::takeAndSaveScreenshot()
{
    this->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KeySymbol(m_captureScreenHandler->getKeyEventTakeScreenShot()));
    this->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(m_captureScreenHandler->getKeyEventTakeScreenShot()));
    update();
}

void GOsgGraphicsView::updateDrawing3DOfItemDrawables(const QList<CT_AbstractItemDrawable *> &list)
{
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
        m_sceneManager->updateItemDrawable(*it.next());
}

void GOsgGraphicsView::updateItemDrawablesThatColorWasModified()
{
    m_sceneManager->updateItemDrawablesThatColorWasModified();
    update();
}

void GOsgGraphicsView::dirtyColorsOfItemDrawablesWithPoints()
{
    m_sceneManager->dirtyDisplayListOfGlobalElementsOfItemDrawable();
    update();
}

void GOsgGraphicsView::getBoundingBoxOfAllItemDrawablePresentInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    return m_sceneManager->getBoundingBoxOfAllItemDrawablePresentInScene(min, max);
}

void GOsgGraphicsView::getBoundingBoxOfAllItemDrawableSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    return m_sceneManager->getBoundingBoxOfAllItemDrawableSelectedInScene(min, max);
}

// --- GOsgGraphicsViewSignalEmitter --- //

GraphicsViewInterface* GOsgGraphicsViewSignalEmitter::graphicsView() const
{
    return m_graphicsView;
}

// --- GOsgCaptureOperation --- //

void GOsgCaptureOperation::operator()(const osg::Image &image, const unsigned int context_id)
{
    QString filename = QFileDialog::getSaveFileName(NULL, QObject::tr("Capture d'écran"), ".", QObject::tr("Image JPEG (*.jpg)"));

    if(!filename.isEmpty())
        osgDB::writeImageFile(image, filename.toStdString());
}
