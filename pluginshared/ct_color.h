#ifndef CT_COLOR_H
#define CT_COLOR_H

#ifdef USE_PCL

#include <pcl/point_types.h>

#define CT_Color pcl::RGB

#else

#include <QtGlobal>
#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_Color
{
public:
    union
    {
        union
        {
            struct
            {
                quint8 b;
                quint8 g;
                quint8 r;
                quint8 a;
            };
            float rgb;
        };
        quint32 rgba;
    };

    inline CT_Color()
    {
        r = g = b = a = 255;
    }

    inline void copy(CT_Color &color) const
    {
        color.b = b;
        color.g = g;
        color.r = r;
        color.a = a;
    }

    inline void setColor(const CT_Color &color)
    {
        color.copy(*this);
    }
};
#endif

#endif // CT_COLOR_H
