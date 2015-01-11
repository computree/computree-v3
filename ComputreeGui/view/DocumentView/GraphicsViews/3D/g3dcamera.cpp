#include "g3dcamera.h"
#include "manipulatedCameraFrame.h"

#include <limits>

#include <QDebug>

G3DCamera::G3DCamera() : qglviewer::Camera()
{
}

float G3DCamera::zNear() const
{
//    if (type() == Camera::PERSPECTIVE)
//    {
//        float z = 0.95f*getZNearDistance();
//        if (z < 1.0f) {z = 1.0f;}
//        return z;
//    } else {
        const float zNearScene = zClippingCoefficient() * sceneRadius();
        float z = distanceToSceneCenter() - zNearScene;
        const float zMin = zNearCoefficient() * zNearScene;
        if (z < zMin){z = 0.0;}
        return z;
//    }
}

float G3DCamera::zFar() const
{
//    if (type() == Camera::PERSPECTIVE)
//    {
//        float z = 1.05 * getZFarDistance();
//        return z;
//    } else {
        return distanceToSceneCenter() + zClippingCoefficient() * sceneRadius();
//    }
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
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _min.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist < minDist) {minDist = dist;}

    return minDist;
}

float G3DCamera::getZFarDistance() const
{
    double maxDist = -std::numeric_limits<double>::max();
    double dist;

    qglviewer::Vec point;

    point.x = _min.x; point.y = _min.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _min.x; point.y = _min.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _min.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _min.x; point.y = _max.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _max.x; point.y = _min.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    point.x = _max.x; point.y = _max.y; point.z = _max.z;
    dist = -((frame()->coordinatesOf(point)).z);
    if (dist > maxDist) {maxDist = dist;}

    return maxDist;
}

