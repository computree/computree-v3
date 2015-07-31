#include "dm_3dcameramanipulator.h"

DM_3DCameraManipulator::DM_3DCameraManipulator() : osgGA::TrackballManipulator()
{
    setMinimumDistance(5);
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
    if(_distance > _minimumDistance)
        _distance -= dy*fabs(_distance);
    else
        _distance -= dy*_minimumDistance;

    // scale
    /*float scale = 1.0f - dy;

    // minimum distance
    float minDist = _minimumDistance;
    if( getRelativeFlag( _minimumDistanceFlagIndex ) )
        minDist *= _modelSize;

    if( _distance*scale > minDist )
    {
        // regular zoom
        _distance *= scale;
    }
    else
    {
        if( pushForwardIfNeeded )
        {
            // push the camera forward
            float scale = -_distance;
            osg::Matrixd rotation_matrix( _rotation );
            osg::Vec3d dv = (osg::Vec3d( 0.0f, 0.0f, -1.0f ) * rotation_matrix) * (dy * scale);
            _center += dv;
        }
        else
        {
            // set distance on its minimum value
            _distance = minDist;
        }
    }*/
}

