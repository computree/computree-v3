#ifndef G3DCAMERACONTROLLER_H
#define G3DCAMERACONTROLLER_H

#include "dm_graphicsview.h"

#include <osgGA/OrbitManipulator>

class GOsgGraphicsView;

class G3DCameraController : public DM_GraphicsViewCamera
{
public:
    G3DCameraController();

    void setRealCameraManipulator(const osgGA::OrbitManipulator *manipulator);
    void setView(const GOsgGraphicsView *view);

    double x() const;
    double y() const;
    double z() const;
    double cx() const;
    double cy() const;
    double cz() const;
    double rx() const;
    double ry() const;
    double rz() const;
    double rw() const;

    CameraInterface::CameraType type() const;

    float focusDistance() const;

    const GLdouble* orientationMatrix() const;
    const GLdouble* orientationMatrixInversed() const;

    void modelViewMatrix(GLdouble m[16]) const;

    void getOrientation(double &q0, double &q1, double &q2, double &q3) const;

    Eigen::Vector3d upVector() const;
    Eigen::Vector3d rightVector() const;

    void setLastItemSelectedCameraCenter(double x, double y, double z);

private:


    class DM_CameraControllerEventHandler : public osgGA::GUIEventHandler {
    public:
        DM_CameraControllerEventHandler(G3DCameraController *c) : m_c(c), m_buttonPressed(false) {}

        bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {

            if((ea.getEventType() == osgGA::GUIEventAdapter::DRAG) || (ea.getEventType() == osgGA::GUIEventAdapter::SCROLL))
                m_c->emitCoordinatesChanged();

            return false;
        }

    private:
        G3DCameraController *m_c;
        bool m_buttonPressed;
    };

    osg::ref_ptr<osgGA::OrbitManipulator>   m_camManipulator;
    GOsgGraphicsView                        *m_view;


    mutable osg::Matrixd                    m_tmpMatrix;
    osg::Vec3d                              m_lastItemSelectedCenter;

    void emitCoordinatesChanged();
    void redrawTheView();

public slots:

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setCX(double cx);
    void setCY(double cy);
    void setCZ(double cz);
    void setRX(double xRot);
    void setRY(double yRot);
    void setRZ(double zRot);
    void setRW(double wRot);

    void setType(CameraInterface::CameraType type);
    void setPointOfView(double cx, double cy, double cz, double distance, double rx, double ry, double rz, double rw, bool redrawView = true);
    void setPosition(double x, double y, double z, bool redrawView = true);
    void setSceneCenter(double cx, double cy, double cz, bool redrawView = true);
    void setViewDirection(double rx, double ry, double rz,  bool redrawView = true);
    void setOrientation(double q0, double q1, double q2, double q3, bool redrawView = true);

    //void centerScene();
    void showEntireScene();
    void syncWithCamera(const DM_GraphicsViewCamera *cam);
    void alignCameraToInvXAxis();
    void alignCameraToInvYAxis();
    void alignCameraToInvZAxis();
    void alignCameraToXAxis();
    void alignCameraToYAxis();
    void alignCameraToZAxis();
    void initCameraCenter();
    void fixCameraCenterToItemsBarycenter();
    void fixCameraCenterToSelectedItemsBarycenter();
    void fitCameraToVisibleItems();
    void fitToSpecifiedBox(const Eigen::Vector3d &bot, const Eigen::Vector3d &top);

};

#endif // G3DCAMERACONTROLLER_H
