#include "dm_2dcameramanipulator.h"

DM_2DCameraManipulator::DM_2DCameraManipulator() : osgGA::OrbitManipulator()
{
}

bool DM_2DCameraManipulator::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    return performMovementRightMouseButton(eventTimeDelta, dx, dy);
}

bool DM_2DCameraManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // pan model
    float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
    panModel( dx*scale, dy*scale );
    return true;
}

bool DM_2DCameraManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // zoom model
    zoomModel( dy * getThrowScale( eventTimeDelta ), true );
    return true;
}
