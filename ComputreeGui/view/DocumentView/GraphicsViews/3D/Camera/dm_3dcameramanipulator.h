#ifndef DM_3DCAMERAMANIPULATOR_H
#define DM_3DCAMERAMANIPULATOR_H

#include "interfaces.h"
#include "view/DocumentView/GraphicsViews/3D/Camera/dm_orthographiccameramanipulator.h"
#include <osgGA/TrackballManipulator>

class DM_3DCameraManipulator : public osgGA::TrackballManipulator, public DM_OrthographicCameraManipulator
{
public:
    DM_3DCameraManipulator(CameraInterface *cameraController, osg::Camera *camera);

protected:
    virtual ~DM_3DCameraManipulator() {}

    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
    virtual void zoomModel(const float dy, bool pushForwardIfNeeded);

    void setDistance( double distance );
    double getDistance() const;

private:
    CameraInterface *m_camController;
};

#endif // DM_3DCAMERAMANIPULATOR_H
