#ifndef DM_ORTHOGRAPHICCAMERAMANIPULATOR_H
#define DM_ORTHOGRAPHICCAMERAMANIPULATOR_H

#include <osg/Camera>

class DM_OrthographicCameraManipulator
{
public:
    DM_OrthographicCameraManipulator();

    /**
     * @brief Set the camera to manipulate
     */
    void setCamera(osg::Camera *cam);

    /**
     * @brief Call this method in resizeGL
     */
    void updateCameraOrthographic();

private:
    osg::ref_ptr<osg::Camera>   m_camera;

protected:
    /**
     * @brief Must return the distance between the camera and the scene
     */
    virtual double getDistance() const = 0;
};

#endif // DM_ORTHOGRAPHICCAMERAMANIPULATOR_H
