#ifndef DM_2DCAMERAMANIPULATOR_H
#define DM_2DCAMERAMANIPULATOR_H

#include <osgGA/OrbitManipulator>

class DM_2DCameraManipulator : public osgGA::OrbitManipulator
{
public:
    DM_2DCameraManipulator();

    void setCamera(osg::Camera *cam);

    void updateCameraOrthographic();

protected:
    virtual ~DM_2DCameraManipulator() {}

    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual void zoomModel(const float dy, bool pushForwardIfNeeded);

private:
    osg::ref_ptr<osg::Camera>   m_camera;
    double                      m_zoom;
};

#endif // DM_2DCAMERAMANIPULATOR_H
