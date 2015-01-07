#include "g3dcamera.h"


G3DCamera::G3DCamera() : qglviewer::Camera()
{
}

float G3DCamera::zNear() const
{
    float z = distanceToSceneCenter() - 2.0*sceneRadius();
    if (z < 0.05f) {z = 0.05f;}

    return z;
}

float G3DCamera::zFar() const
{
    return distanceToSceneCenter() + 2.0 * sceneRadius();
}
