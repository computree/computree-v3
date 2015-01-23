#include "g3dcamera.h"
#include "manipulatedCameraFrame.h"

#include <limits>

#include <QDebug>

G3DCamera::G3DCamera() : qglviewer::Camera()
{
}

qreal G3DCamera::zNear() const
{
    /*float z = 0.95f*getZNearDistance();
    if (type() == Camera::PERSPECTIVE)
    {
        float z = 0.95f*getZNearDistance();
        if (z < 1.0f) {z = 1.0f;}
        return z;
    } else {
        const float zNearScene = zClippingCoefficient() * sceneRadius();
        float z = distanceToSceneCenter() - zNearScene;
        const float zMin = zNearCoefficient() * zNearScene;
        if (z < zMin){z = 0.0;}
        return z;
    }
    return z;*/
    return 0.001;
}

qreal G3DCamera::zFar() const
{
    //return distanceToSceneCenter() + 2.0 * sceneRadius();
    if (type() == Camera::PERSPECTIVE)
    {
        float z = 1.05 * getZFarDistance();
        return z;
    } else {
        return distanceToSceneCenter() + zClippingCoefficient() * sceneRadius();
    }
}

void G3DCamera::setBoundingBox(const qglviewer::Vec& min, const qglviewer::Vec& max)
{
    setSceneBoundingBox(min, max);
    _min = min;
    _max = max;
}

void G3DCamera::loadModelViewMatrix(bool reset) const
{
    // WARNING: makeCurrent must be called by every calling method
    glMatrixMode(GL_MODELVIEW);

    // Identity matrix
    GLdouble m[16];

    for(int i=0; i<16; ++i)
        m[i] = 0;

    m[0] = 1;
    m[5] = 1;
    m[10] = 1;
    m[15] = 1;

    if (reset)
        glLoadMatrixd(m);
    else
        glMultMatrixd(m);
}

double G3DCamera::getZNearDistance() const
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

double G3DCamera::getZFarDistance() const
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

