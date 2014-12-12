#ifndef CT_COLOR_H
#define CT_COLOR_H

#include <QtGlobal>

#ifdef USE_PCL

#define PCL_NO_PRECOMPILE
#include <pcl/point_types.h>

// pcl::RGB is a bgra structure
struct CT_Color : pcl::RGB {

#else

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_Color
{
public:
    union
    {
        struct
        {
            quint8 b;
            quint8 g;
            quint8 r;
            quint8 a;
        };
        quint8 data[4];
    };
#endif

public:
    inline CT_Color()
    {
        r = g = b = a = 255;
    }

    inline quint8& operator[](int i) { return (&b)[i]; }
    inline const quint8& operator[](int i) const { return (&b)[i]; }

    inline quint8& operator()(int i) { return (&b)[i]; }
    inline const quint8& operator()(int i) const { return (&b)[i]; }

    inline void copy(CT_Color &color) const
    {
        color(0) = (*this)(0);
        color(1) = (*this)(1);
        color(2) = (*this)(2);
        color(3) = (*this)(3);
    }

    inline void setColor(const CT_Color &color)
    {
        color.copy(*this);
    }
};

#endif // CT_COLOR_H
