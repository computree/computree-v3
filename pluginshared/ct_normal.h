#ifndef CT_NORMAL_H
#define CT_NORMAL_H

#ifdef USE_PCL

#include <pcl/point_types.h>

#define CT_Normal pcl::Normal

#else

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_Normal
{
public:
    union
    {
        float data[4];
        float normal[3];
        struct
        {
            float normal_x;
            float normal_y;
            float normal_z;
        };
    };

    union
    {
        struct
        {
            float curvature;
        };
        float data_c[4];
    };

    inline void copy(CT_Normal &normal) const
    {
        normal.normal_x = normal_x;
        normal.normal_y = normal_y;
        normal.normal_z = normal_z;
        normal.curvature = curvature;
    }

    inline void setNormal(const CT_Normal &normal)
    {
        normal.copy(*this);
    }

    inline float* vertex() const
    {
        return const_cast<float*>(&normal_x);
    }
};
#endif

#endif // CT_NORMAL_H
