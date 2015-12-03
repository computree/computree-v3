#include "dm_2dcameramanipulator.h"

DM_2DCameraManipulator::DM_2DCameraManipulator(osg::Camera *camera) : osgGA::OrbitManipulator()
{
    setMinimumDistance(1);
    setCamera(camera);
}

bool DM_2DCameraManipulator::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    return performMovementRightMouseButton(eventTimeDelta, dx, dy);
}

bool DM_2DCameraManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // pan model
    float scale = -0.15f * _distance * getThrowScale( eventTimeDelta );
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

void DM_2DCameraManipulator::setDistance(double distance)
{
    osgGA::OrbitManipulator::setDistance(distance);
    updateCameraOrthographic();
}

void DM_2DCameraManipulator::setDistanceWithoutUpdate(double distance)
{
    osgGA::OrbitManipulator::setDistance(distance);
}

double DM_2DCameraManipulator::getDistance() const
{
    return _distance;
}
