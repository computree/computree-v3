#include "dm_2dcameramanipulator.h"

DM_2DCameraManipulator::DM_2DCameraManipulator() : osgGA::OrbitManipulator()
{
    m_zoom = 1.0;
    setMinimumDistance(1);
}

void DM_2DCameraManipulator::setCamera(osg::Camera *cam)
{
    m_camera = cam;
}

void DM_2DCameraManipulator::updateCameraOrthographic()
{
    const osg::GraphicsContext::Traits* traits = m_camera->getGraphicsContext()->getTraits();

    double w = traits->width;
    double h = traits->height;

    double scale = w/h;

    m_camera->setProjectionMatrixAsOrtho(-1*scale*_distance, 1*scale*_distance, -1*_distance, 1*_distance, -1, 1);
}

bool DM_2DCameraManipulator::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    return performMovementRightMouseButton(eventTimeDelta, dx, dy);
}

bool DM_2DCameraManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // pan model
    float scale = -0.5f * _distance * getThrowScale( eventTimeDelta );
    panModel( dx*scale, dy*scale );
    return true;
}

bool DM_2DCameraManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // zoom model
    zoomModel( dy * getThrowScale( eventTimeDelta ), true );
    return true;
}

void DM_2DCameraManipulator::zoomModel(const float dy, bool pushForwardIfNeeded)
{
    if(_distance > _minimumDistance)
        _distance -= dy*fabs(_distance);
    else
        _distance -= dy*_minimumDistance;

    if(_distance < 0)
        _distance = 0.000000001;

    updateCameraOrthographic();
}
