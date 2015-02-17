#ifndef CT_NORMAL_H
#define CT_NORMAL_H

#ifdef USE_PCL

#define PCL_NO_PRECOMPILE
#include <pcl/point_types.h>

struct CT_Normal : pcl::Normal {

#else

#include "pluginShared_global.h"

 // TODO => pass CT_Normal to double like CT_Point / CT_PointData !
class PLUGINSHAREDSHARED_EXPORT CT_Normal
{
public:
    union
    {
        float data_n[4];
        struct
        {
            float normal_x;
            float normal_y;
            float normal_z;
            float curvature;
        };
    };
#endif

public:
    inline float& operator[](int i) { return data_n[i]; }
    inline const float& operator[](int i) const { return data_n[i]; }

    inline float& operator()(int i) { return data_n[i]; }
    inline const float& operator()(int i) const { return data_n[i]; }

    inline void copy(CT_Normal &normal) const
    {
        normal(0) = (*this)(0);
        normal(1) = (*this)(1);
        normal(2) = (*this)(2);
        normal(3) = (*this)(3);
    }

    inline void setNormal(const CT_Normal &normal)
    {
        normal.copy(*this);
    }

    inline float* vertex() const
    {
        return const_cast<float*>(&(*this)(0));
    }
};

#endif // CT_NORMAL_H
