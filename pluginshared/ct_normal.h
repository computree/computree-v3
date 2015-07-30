#ifndef CT_NORMAL_H
#define CT_NORMAL_H

#include "pluginShared_global.h"

#include <osg/Vec4f>

class PLUGINSHAREDSHARED_EXPORT CT_Normal : public osg::Vec4f
{
public:

    inline CT_Normal() : osg::Vec4f()
    {
    }

    inline CT_Normal(value_type x, value_type y, value_type z, value_type curvature) : osg::Vec4f(x, y, z, curvature)
    {
    }

    inline value_type& operator()(unsigned int i) { return _v[i]; }
    inline const value_type& operator()(unsigned int i) const { return _v[i]; }

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
};

#endif // CT_NORMAL_H
