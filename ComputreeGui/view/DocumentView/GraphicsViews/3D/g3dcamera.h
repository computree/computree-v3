#ifndef G3DCAMERA_H
#define G3DCAMERA_H

#include "qglviewer.h"

class G3DCamera : public qglviewer::Camera
{
public:
    G3DCamera();

    virtual float zNear() const;
    virtual float zFar() const;

    void setBoundingBox(const qglviewer::Vec& min, const qglviewer::Vec& max);

    void loadModelViewMatrix(bool reset=true) const;

private:
    qglviewer::Vec     _min;
    qglviewer::Vec     _max;

    float getZNearDistance() const;
    float getZFarDistance() const;
};

#endif // G3DCAMERA_H
