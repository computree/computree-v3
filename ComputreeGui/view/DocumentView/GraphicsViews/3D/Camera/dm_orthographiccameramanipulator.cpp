#include "dm_orthographiccameramanipulator.h"

DM_OrthographicCameraManipulator::DM_OrthographicCameraManipulator()
{
    m_camera = NULL;
}

void DM_OrthographicCameraManipulator::setCamera(osg::Camera *cam)
{
    m_camera = cam;
}

void DM_OrthographicCameraManipulator::updateCameraOrthographic()
{
    const osg::GraphicsContext::Traits* traits = m_camera->getGraphicsContext()->getTraits();

    double w = traits->width;
    double h = traits->height;

    double scale = w/h;
    double d = getDistance()/3.7;

    m_camera->setProjectionMatrixAsOrtho(-scale*d, scale*d, -d, d, -1, 1);
}
