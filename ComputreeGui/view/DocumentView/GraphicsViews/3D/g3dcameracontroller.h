#ifndef G3DCAMERACONTROLLER_H
#define G3DCAMERACONTROLLER_H

#include "dm_graphicsviewcamera.h"

#include "qglviewer.h"
#include "dm_graphicsview.h"

class G3DCameraController : public DM_GraphicsViewCamera
{
public:
    G3DCameraController();

    void setRealCamera(const qglviewer::Camera *camera);
    void setView(const DM_GraphicsView *view);

    double x() const;
    double y() const;
    double z() const;
    double cx() const;
    double cy() const;
    double cz() const;
    double rx() const;
    double ry() const;
    double rz() const;

    CameraInterface::CameraType type() const;

    float focusDistance() const;

    const GLdouble* orientationMatrix() const;
    const GLdouble* orientationMatrixInversed() const;

    void modelViewMatrix(GLdouble m[16]) const;

    void getOrientation(double &q0, double &q1, double &q2, double &q3) const;
    bool getCameraFrustumPlanesCoefficients(GLdouble coef[6][4]) const;

    Eigen::Vector3d projectedCoordinatesOf(const Eigen::Vector3d &src) const;
    Eigen::Vector3d openGLProjectedCoordinatesOf(const Eigen::Vector3d &src) const;
    Eigen::Vector3d unprojectedCoordinatesOf(const Eigen::Vector3d &src) const;

    Eigen::Vector3d upVector() const;
    Eigen::Vector3d rightVector() const;

    void setLastItemSelectedCameraCenter(double x, double y, double z);

private:

    qglviewer::Camera   *_realCamera;
    DM_GraphicsView     *_view;

    qglviewer::Vec      _lastItemSelectedCenter;

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

    void setType(CameraInterface::CameraType type);
    void setPosition(double x, double y, double z, bool redrawView = true);
    void setSceneCenter(double cx, double cy, double cz, bool redrawView = true);
    void setViewDirection(double rx, double ry, double rz,  bool redrawView = true);
    void setOrientation(double q0, double q1, double q2, double q3, bool redrawView = true);

    //void centerScene();
    void showEntireScene();
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

};

#endif // G3DCAMERACONTROLLER_H
