#ifndef CT_COLOR_H
#define CT_COLOR_H

#include <QtGlobal>
#include "pluginShared_global.h"

#include <osg/Array>

class PLUGINSHAREDSHARED_EXPORT CT_Color : public osg::Vec4ub
{
public:
    inline CT_Color() : osg::Vec4ub(255, 255, 255, 255)
    {
    }

    inline CT_Color(value_type r, value_type g, value_type b, value_type a) : osg::Vec4ub(r, g, b ,a)
    {
    }

    inline value_type& operator()(unsigned int i) { return _v[i]; }
    inline const value_type& operator()(unsigned int i) const { return _v[i]; }

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
