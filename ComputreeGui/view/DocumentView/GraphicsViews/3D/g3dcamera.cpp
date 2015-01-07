#include "g3dcamera.h"
#include "manipulatedCameraFrame.h"

#include <limits>

G3DCamera::G3DCamera() : qglviewer::Camera()
{
}

float G3DCamera::zNear() const
{
    float z = 0.95f*getZNearDistance();
    if (z < 0.05f)
    {
        switch (type())
        {
            case Camera::PERSPECTIVE  : z = 0.05f; break;
            case Camera::ORTHOGRAPHIC : z = 0.00f; break;
        }
    }

    return z;
}

float G3DCamera::zFar() const
{
    return distanceToSceneCenter() + 2.0 * sceneRadius();
}

void G3DCamera::setBoundingBox(const qglviewer::Vec& min, const qglviewer::Vec& max)
{
    setSceneBoundingBox(min, max);
    _min = min;
    _max = max;
}

float G3DCamera::getZNearDistance() const
{
    double minDist = std::numeric_limits<double>::max();
    double dist;

    qglviewer::Vec point;

    point.x = _min.x; point.y = _min.y; point.z = _min.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _min.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _min.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _min.y; point.z = _max.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _min.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _max.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _max.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _max.z;
    dist = fabs((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    return sqrt(minDist);
}
