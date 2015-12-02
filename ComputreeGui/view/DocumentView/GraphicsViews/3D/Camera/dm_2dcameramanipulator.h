#ifndef DM_2DCAMERAMANIPULATOR_H
#define DM_2DCAMERAMANIPULATOR_H

#include "view/DocumentView/GraphicsViews/3D/Camera/dm_orthographiccameramanipulator.h"
#include <osgGA/OrbitManipulator>

class DM_2DCameraManipulator : public osgGA::OrbitManipulator, public DM_OrthographicCameraManipulator
{
public:
    DM_2DCameraManipulator(osg::Camera *camera);

protected:
    virtual ~DM_2DCameraManipulator() {}

    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual void zoomModel(const float dy, bool pushForwardIfNeeded);

    void setDistance(double distance);
    double getDistance() const;
};

#endif // DM_2DCAMERAMANIPULATOR_H
