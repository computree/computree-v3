#ifndef DM_3DCAMERAMANIPULATOR_H
#define DM_3DCAMERAMANIPULATOR_H

#include <osgGA/TrackballManipulator>

class DM_3DCameraManipulator : public osgGA::TrackballManipulator
{
public:
    DM_3DCameraManipulator();

protected:
    virtual ~DM_3DCameraManipulator() {}

    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual void zoomModel(const float dy, bool pushForwardIfNeeded);
};

#endif // DM_3DCAMERAMANIPULATOR_H
