#include "g3dcameracontroller.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <limits>

#include "dm_guimanager.h"

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_iterator/ct_pointiterator.h"

#include "view/DocumentView/GraphicsViews/3D/Camera/dm_3dcameramanipulator.h"
#include "view/DocumentView/GraphicsViews/3D/gosggraphicsview.h"

G3DCameraController::G3DCameraController() : DM_GraphicsViewCamera()
{
    m_camManipulator = NULL;
    m_view = NULL;
}

void G3DCameraController::setRealCameraManipulator(const osgGA::OrbitManipulator *manipulator)
{
    m_camManipulator = (osgGA::OrbitManipulator*)manipulator;
}

void G3DCameraController::setView(const GOsgGraphicsView *view)
{
    m_view = (GOsgGraphicsView*)view;
    m_view->getCompositeViewer()->getView(0)->addEventHandler(new G3DCameraController::DM_CameraControllerEventHandler(this));
}

void G3DCameraController::setLastItemSelectedCameraCenter(double x, double y, double z)
{
    m_lastItemSelectedCenter.set(x, y, z);
}

void G3DCameraController::emitCoordinatesChanged()
{
    emit coordinatesChanged();
}

double G3DCameraController::x() const
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);

    return eye.x();
}
double G3DCameraController::y() const
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);

    return eye.y();
}

double G3DCameraController::z() const
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);

    return eye.z();
}

double G3DCameraController::cx() const
{
    return m_camManipulator->getCenter().x();
}

double G3DCameraController::cy() const
{
    return m_camManipulator->getCenter().y();
}

double G3DCameraController::cz() const
{
    return m_camManipulator->getCenter().z();
}

double G3DCameraController::rx() const
{
    return m_camManipulator->getRotation().x();
}

double G3DCameraController::ry() const
{
    return m_camManipulator->getRotation().y();
}

double G3DCameraController::rz() const
{
    return m_camManipulator->getRotation().z();
}

double G3DCameraController::rw() const
{
    return m_camManipulator->getRotation().w();
}

float G3DCameraController::focusDistance() const
{
    return m_camManipulator->getDistance();
}

CameraInterface::CameraType G3DCameraController::type() const
{
    return ((dynamic_cast<DM_3DCameraManipulator*>(m_camManipulator.get()) != NULL) ? CameraInterface::PERSPECTIVE : CameraInterface::ORTHOGRAPHIC);
}

const GLdouble* G3DCameraController::orientationMatrix() const
{
    m_tmpMatrix = m_camManipulator->getMatrix();
    return m_tmpMatrix.ptr();
}

const GLdouble* G3DCameraController::orientationMatrixInversed() const
{
    m_tmpMatrix = m_camManipulator->getInverseMatrix();
    return m_tmpMatrix.ptr();
}

void G3DCameraController::modelViewMatrix(GLdouble m[16]) const
{
    osg::Matrixd matrix = m_camManipulator->getMatrix();

    for(int i=0; i<16; ++i)
        m[i] = matrix.ptr()[i];
}

void G3DCameraController::getOrientation(double &q0, double &q1, double &q2, double &q3) const
{
    const osg::Quat &rot = m_camManipulator->getRotation();
    q0 = rot.x();
    q1 = rot.y();
    q2 = rot.z();
    q3 = rot.w();
}

Eigen::Vector3d G3DCameraController::upVector() const
{
    Eigen::Vector3d ret;

    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;

    m_camManipulator->getHomePosition(eye, center, up);

    ret(0) = up.x();
    ret(1) = up.y();
    ret(2) = up.z();

    return ret;
}

Eigen::Vector3d G3DCameraController::rightVector() const
{
    // TODO
    Eigen::Vector3d ret;

    return ret;
}

////////////// SLOTS ////////////////

void G3DCameraController::setX(double x)
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);
    eye.x() = x;
    m_camManipulator->setTransformation(eye, quat);

    redrawTheView();
}

void G3DCameraController::setY(double y)
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);
    eye.y() = y;
    m_camManipulator->setTransformation(eye, quat);
    redrawTheView();
}

void G3DCameraController::setZ(double z)
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);
    eye.z() = z;
    m_camManipulator->setTransformation(eye, quat);
    redrawTheView();
}

void G3DCameraController::setCX(double cx)
{
    osg::Vec3d c = m_camManipulator->getCenter();
    c.x() = cx;

    m_camManipulator->setCenter(c);

    redrawTheView();
}

void G3DCameraController::setCY(double cy)
{
    osg::Vec3d c = m_camManipulator->getCenter();
    c.y() = cy;

    m_camManipulator->setCenter(c);

    redrawTheView();
}

void G3DCameraController::setCZ(double cz)
{
    osg::Vec3d c = m_camManipulator->getCenter();
    c.z() = cz;

    m_camManipulator->setCenter(c);

    redrawTheView();
}

void G3DCameraController::setRX(double xRot)
{
    osg::Quat quat = m_camManipulator->getRotation();
    quat.x() = xRot;
    m_camManipulator->setRotation(quat);

    redrawTheView();
}

void G3DCameraController::setRY(double yRot)
{
    osg::Quat quat = m_camManipulator->getRotation();
    quat.y() = yRot;
    m_camManipulator->setRotation(quat);

    redrawTheView();
}

void G3DCameraController::setRZ(double zRot)
{
    osg::Quat quat = m_camManipulator->getRotation();
    quat.z() = zRot;
    m_camManipulator->setRotation(quat);

    redrawTheView();
}

void G3DCameraController::setRW(double wRot)
{
    osg::Quat quat = m_camManipulator->getRotation();
    quat.w() = wRot;
    m_camManipulator->setRotation(quat);

    redrawTheView();
}

void G3DCameraController::setType(CameraInterface::CameraType type)
{
    m_view->setCameraType(type);
}

void G3DCameraController::setPointOfView(double cx, double cy, double cz, double distance, double rx, double ry, double rz, double rw, bool redrawView)
{
    m_camManipulator->setCenter(osg::Vec3d(cx, cy, cz));
    m_camManipulator->setRotation(osg::Quat(rx, ry, rz, rw));
    m_camManipulator->setDistance(distance);

    if(redrawView)
        redrawTheView();

    emit coordinatesChanged();
}

void G3DCameraController::setPosition(double x, double y, double z, bool redrawView)
{
    osg::Vec3d eye;
    osg::Quat quat;

    m_camManipulator->getTransformation(eye, quat);
    eye.set(x, y ,z);

    m_camManipulator->setTransformation(eye, quat);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setSceneCenter(double cx, double cy, double cz, bool redrawView)
{
    m_camManipulator->setCenter(osg::Vec3d(cx, cy, cz));

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setViewDirection(double rx, double ry, double rz, bool redrawView)
{
    // TODO

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::setOrientation(double q0, double q1, double q2, double q3, bool redrawView)
{
    osg::Quat quat;
    quat.set(q0, q1, q2, q3);

    m_camManipulator->setRotation(quat);

    if(redrawView)
        redrawTheView();
}

void G3DCameraController::showEntireScene()
{
    m_camManipulator->computeHomePosition(m_view->getCompositeViewer()->getCameraWithFocus());
    m_camManipulator->home(0);

    redrawTheView();
}

void G3DCameraController::syncWithCamera(const DM_GraphicsViewCamera *cam)
{
    const G3DCameraController *controller = dynamic_cast<const G3DCameraController*>(cam);

    if((controller != NULL) && controller->m_camManipulator.valid()) {

        const osg::Vec3d center = controller->m_camManipulator->getCenter();
        const osg::Quat rotation = controller->m_camManipulator->getRotation();
        double d = controller->m_camManipulator->getDistance();

        m_camManipulator->setCenter(center);
        m_camManipulator->setRotation(rotation);
        m_camManipulator->setDistance(d);

        /*osg::Vec3d eye;
        osg::Quat quat;

        controller->m_camManipulator->getTransformation(eye, quat);
        m_camManipulator->setTransformation(eye, quat);*/
        redrawTheView();
    }
}

double eps() { return 1e-3; }

void G3DCameraController::alignCameraToInvXAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue de dos
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0,0.0,0.0));
    m_camManipulator->setRotation(osg::Quat(-90.0f, osg::Vec3f(1.0f,0.0f,0.0f)));
    m_camManipulator->setCenter(center);
    m_camManipulator->setElevation(0.0f);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::alignCameraToInvYAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue coté droit
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0, 0.0, 0.0));
    m_camManipulator->setRotation(osg::Quat(90.0f, osg::Vec3f(0.0f, 1.0f, 0.0f)));
    m_camManipulator->setCenter(center);
    m_camManipulator->setElevation(0.0f);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::alignCameraToInvZAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue de dessous
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0f, 0.0f, 0.0f));
    m_camManipulator->setRotation(osg::Quat(1, 0, 0, 0));
    m_camManipulator->setCenter(center);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::alignCameraToXAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue de face
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0,0.0,0.0));
    m_camManipulator->setRotation(osg::Quat(90.0f, osg::Vec3f(1.0f,0.0f,0.0f)));
    m_camManipulator->setCenter(center);
    m_camManipulator->setElevation(0.0f);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::alignCameraToYAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue coté gauche
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0, 0.0, 0.0));
    m_camManipulator->setRotation(osg::Quat(-90.0f, osg::Vec3f(0.0f, 1.0f, 0.0f)));
    m_camManipulator->setCenter(center);
    m_camManipulator->setElevation(0.0f);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::alignCameraToZAxis()
{
    osg::Vec3d center = m_camManipulator->getCenter();

    double distance = m_camManipulator->getFusionDistanceValue();

    // vue de dessus
    m_camManipulator->home(0);
    m_camManipulator->setCenter(osg::Vec3d(0.0f, 0.0f, 0.0f));
    m_camManipulator->setRotation(osg::Quat(0.0f, osg::Vec3f(0.0f, 0.0f, 1.0f)));
    m_camManipulator->setCenter(center);
    m_camManipulator->setDistance(distance);

    redrawTheView();
}

void G3DCameraController::initCameraCenter()
{
    m_camManipulator->setCenter(osg::Vec3d(0,0,0));

    redrawTheView();
}

void G3DCameraController::fixCameraCenterToItemsBarycenter()
{
    Eigen::Vector3d min;
    Eigen::Vector3d max;

    m_view->getBoundingBoxOfAllItemDrawablePresentInView(min, max);

    m_camManipulator->setCenter(osg::Vec3d(min[0]+((max[0]-min[0])/2.0),
                                           min[1]+((max[1]-min[1])/2.0),
                                           min[2]+((max[2]-min[2])/2.0)));
    redrawTheView();
}

void G3DCameraController::fixCameraCenterToSelectedItemsBarycenter()
{
    Eigen::Vector3d min;
    Eigen::Vector3d max;

    m_view->getBoundingBoxOfAllItemDrawableSelectedInView(min, max);

    m_camManipulator->setCenter(osg::Vec3d(min[0]+((max[0]-min[0])/2.0),
                                           min[1]+((max[1]-min[1])/2.0),
                                           min[2]+((max[2]-min[2])/2.0)));

    redrawTheView();
}

void G3DCameraController::fitCameraToVisibleItems()
{
    Eigen::Vector3d min, max;
    m_view->getBoundingBoxOfAllItemDrawablePresentInView(min, max);
    fitToSpecifiedBox(min, max);
}

void G3DCameraController::fitToSpecifiedBox(const Eigen::Vector3d &bot, const Eigen::Vector3d &top)
{
    osg::Vec3d min;
    osg::Vec3d max;

    min.x() = bot(0);
    min.y() = bot(1);
    min.z() = bot(2);

    max.x() = top(0);
    max.y() = top(1);
    max.z() = top(2);

    if (    (min.x() < std::numeric_limits<osg::BoundingBox::value_type>::max()) &&
            (min.y() < std::numeric_limits<osg::BoundingBox::value_type>::max()) &&
            (min.z() < std::numeric_limits<osg::BoundingBox::value_type>::max()) &&
            (max.x() > -std::numeric_limits<osg::BoundingBox::value_type>::max()) &&
            (max.y() > -std::numeric_limits<osg::BoundingBox::value_type>::max()) &&
            (max.z() > -std::numeric_limits<osg::BoundingBox::value_type>::max()))
    {
        osg::BoundingBox bb;
        bb.set(min.x(), min.y(), min.z(), max.x(), max.y(), max.z());

        osg::BoundingSphere boundingSphere;
        boundingSphere.expandBy(bb);

        double dist = boundingSphere.radius();

        osg::Camera *camera = NULL;
        osgViewer::ViewerBase::Cameras cams;
        m_view->getCompositeViewer()->getCameras(cams);

        if(!cams.empty())
            camera = cams.front();

        if (camera)
        {
            // try to compute dist from frustum
            double left,right,bottom,top,zNear,zFar;
            if (camera->getProjectionMatrixAsFrustum(left,right,bottom,top,zNear,zFar))
            {
                double vertical2 = fabs(right - left) / zNear / 2.;
                double horizontal2 = fabs(top - bottom) / zNear / 2.;
                double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
                double viewAngle = atan2(dim,1.);
                dist = boundingSphere.radius() / (sin(viewAngle));
            }
            else
            {
                // try to compute dist from ortho
                if (camera->getProjectionMatrixAsOrtho(left,right,bottom,top,zNear,zFar))
                {
                    dist = fabs(zFar - zNear) / 2.;
                }
            }
        }

        m_camManipulator->setCenter(bb.center());
        m_camManipulator->setDistance(dist);
        redrawTheView();
    }
}

void G3DCameraController::redrawTheView()
{
    if(m_view != NULL)
        m_view->redraw();
}

