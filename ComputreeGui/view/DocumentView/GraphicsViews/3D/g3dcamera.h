#ifndef G3DCAMERA_H
#define G3DCAMERA_H

#include "qglviewer.h"

class G3DCamera : public qglviewer::Camera
{
public:
    G3DCamera();

    virtual float zNear() const;
    virtual float zFar() const;
};

#endif // G3DCAMERA_H
