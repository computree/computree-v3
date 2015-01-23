#ifndef G3DCAMERA_H
#define G3DCAMERA_H

#include "qglviewer.h"

class G3DCamera : public qglviewer::Camera
{
public:
    G3DCamera();

    virtual qreal zNear() const;
    virtual qreal zFar() const;

    void setBoundingBox(const qglviewer::Vec& min, const qglviewer::Vec& max);

    void loadModelViewMatrix(bool reset=true) const;

private:
    qglviewer::Vec     _min;
    qglviewer::Vec     _max;

    double getZNearDistance() const;
    double getZFarDistance() const;
};

#endif // G3DCAMERA_H
