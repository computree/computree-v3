#ifndef DM_2DCAMERAMANIPULATOR_H
#define DM_2DCAMERAMANIPULATOR_H

#include <osgGA/OrbitManipulator>

class DM_2DCameraManipulator : public osgGA::OrbitManipulator
{
public:
    DM_2DCameraManipulator();

protected:
    virtual ~DM_2DCameraManipulator() {}

    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
};

#endif // DM_2DCAMERAMANIPULATOR_H
