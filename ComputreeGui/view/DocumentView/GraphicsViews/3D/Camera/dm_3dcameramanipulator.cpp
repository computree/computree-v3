#include "dm_3dcameramanipulator.h"

DM_3DCameraManipulator::DM_3DCameraManipulator(CameraInterface *cameraController, osg::Camera *camera) : osgGA::TrackballManipulator()
{
    setMinimumDistance(5);
    setCamera(camera);
    m_camController = cameraController;
}

bool DM_3DCameraManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
    // rotate camera
    if( getVerticalAxisFixed() )
        rotateWithFixedVertical( dx, dy );
    else
        rotateTrackball( _ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
                         _ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
                         2.0*getThrowScale( eventTimeDelta ) );

    return true;
}

bool DM_3DCameraManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // pan model
    float scale = -0.15f * _distance * getThrowScale( eventTimeDelta );
    panModel( dx*scale, dy*scale );
    return true;
}

bool DM_3DCameraManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // zoom model
    zoomModel( dy * getThrowScale( eventTimeDelta ), true );
    return true;
}

void DM_3DCameraManipulator::zoomModel( const float dy, bool pushForwardIfNeeded )
{
    double minimumDistance = _minimumDistance;

    if(m_camController->type() == CameraInterface::ORTHOGRAPHIC)
        minimumDistance = 1;

    if(_distance > minimumDistance)
        _distance -= dy*fabs(_distance);
    else
        _distance -= dy*minimumDistance;

    if(m_camController->type() == CameraInterface::ORTHOGRAPHIC) {

        if(_distance < 0)
            _distance = 0.000000001;

        updateCameraOrthographic();
    }
}

void DM_3DCameraManipulator::setDistance(double distance)
{
    osgGA::TrackballManipulator::setDistance(distance);
    updateCameraOrthographic();
}

void DM_3DCameraManipulator::setDistanceWithoutUpdate(double distance)
{
    osgGA::TrackballManipulator::setDistance(distance);
}

double DM_3DCameraManipulator::getDistance() const
{
    return _distance;
}
