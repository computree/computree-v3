/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef CT_POINT_H
#define CT_POINT_H

#include <iostream>
#include <string>

#include <cmath>
#include "pluginShared_global.h"

#ifndef USE_PCL
#include <Eigen/Core>

class CT_Point : public Eigen::Vector3f
{
public:

    enum PVAR {
        X = 0,
        Y = 1,
        Z = 2
    };

    inline CT_Point(void):Eigen::Vector3f() {}
    typedef Eigen::Vector3f Base;
    // This constructor allows you to construct CT_Point from Eigen expressions
    template<typename OtherDerived>
    inline CT_Point(const Eigen::MatrixBase<OtherDerived>& other)
        : Eigen::Vector3f(other)
    { }
    // This method allows you to assign Eigen expressions to CT_Point
    template<typename OtherDerived>
    inline CT_Point & operator= (const Eigen::MatrixBase <OtherDerived>& other)
    {
        this->Base::operator=(other);
        return *this;
    }

    inline void copy(CT_Point &point) const
    {
        point(0) = (*this)(0);
        point(1) = (*this)(1);
        point(2) = (*this)(2);
    }

    inline void setX(float x) { (*this)(0) = x; }
    inline void setY(float y) { (*this)(1) = y; }
    inline void setZ(float z) { (*this)(2) = z; }

    inline float length() const { return ( sqrt( (*this)(0)*(*this)(0) + (*this)(1)*(*this)(1) + (*this)(2)*(*this)(2) ) ); }

    inline void setPoint(const CT_Point &point)
    {
        point.copy(*this);
    }

    inline float* vertex() const
    {
        return const_cast<float*>(&(*this)(0));
    }
};

#else

#define PCL_NO_PRECOMPILE
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/octree/octree_search.h>
#include <pcl/octree/impl/octree_search.hpp>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/impl/voxel_grid.hpp>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/impl/statistical_outlier_removal.hpp>

class EIGEN_ALIGN16 CT_Point : public pcl::_PointXYZ
{
public:
    inline CT_Point()
    {
        pcl::_PointXYZ::x = pcl::_PointXYZ::y = pcl::_PointXYZ::z = 0.0f;
    }

    inline CT_Point(float _x, float _y, float _z)
    {
        pcl::_PointXYZ::x = _x;
        pcl::_PointXYZ::y = _y;
        pcl::_PointXYZ::z = _z;
    }

    // This constructor allows construct CT_Point from Eigen expressions
    template<typename OtherDerived>
    inline CT_Point(const Eigen::MatrixBase<OtherDerived>& other)
    {
        Eigen::Vector3f v(other);
        pcl::_PointXYZ::x = v(0);
        pcl::_PointXYZ::y = v(1);
        pcl::_PointXYZ::z = v(2);
    }

    // This method allows assign Eigen expressions to CT_Point
    template<typename OtherDerived>
    inline CT_Point & operator= (const Eigen::MatrixBase <OtherDerived>& other)
    {
        Eigen::Vector3f v(other);
        pcl::_PointXYZ::x = v(0);
        pcl::_PointXYZ::y = v(1);
        pcl::_PointXYZ::z = v(2);

        return *this;
    }

    // TO BE COMPATIBLE DON'T USE variable x, y, z or data but method "[index]" or "(index)"
    inline float& operator[](int i) { return data[i]; }
    inline const float& operator[](int i) const { return data[i]; }

    inline float& operator()(int i) { return data[i]; }
    inline const float& operator()(int i) const { return data[i]; }

    inline CT_Point& operator+= (const CT_Point& o) { x += o.x; y += o.y; z += o.z; return *this; }
    inline CT_Point& operator-= (const CT_Point& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    inline CT_Point& operator/= (const CT_Point& o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
    inline CT_Point& operator*= (const CT_Point& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }

    inline CT_Point& operator+= (const float& o) { x += o; y += o; z += o; return *this; }
    inline CT_Point& operator-= (const float& o) { x -= o; y -= o; z -= o; return *this; }
    inline CT_Point& operator/= (const float& o) { x /= o; y /= o; z /= o; return *this; }
    inline CT_Point& operator*= (const float& o) { x *= o; y *= o; z *= o; return *this; }

    inline CT_Point operator+ (const CT_Point& o) const { CT_Point t; t.x = x + o.x; t.y = y + o.y; t.z = z + o.z; return t; }
    inline CT_Point operator- (const CT_Point& o) const { CT_Point t; t.x = x - o.x; t.y = y - o.y; t.z = z - o.z; return t; }
    inline CT_Point operator/ (const CT_Point& o) const { CT_Point t; t.x = x / o.x; t.y = y / o.y; t.z = z / o.z; return t; }
    inline CT_Point operator* (const CT_Point& o) const { CT_Point t; t.x = x * o.x; t.y = y * o.y; t.z = z * o.z; return t; }

    inline CT_Point operator+ (const float& o) const { CT_Point t; t.x = x + o; t.y = y + o; t.z = z + o; return t; }
    inline CT_Point operator- (const float& o) const { CT_Point t; t.x = x - o; t.y = y - o; t.z = z - o; return t; }
    inline CT_Point operator/ (const float& o) const { CT_Point t; t.x = x / o; t.y = y / o; t.z = z / o; return t; }
    inline CT_Point operator* (const float& o) const { CT_Point t; t.x = x * o; t.y = y * o; t.z = z * o; return t; }

    inline CT_Point& operator= (const CT_Point& o) { x = o.x; y = o.y; z = o.z; return *this; }

    inline void copy(CT_Point &point) const
    {
        point(0) = (*this)(0);
        point(1) = (*this)(1);
        point(2) = (*this)(2);
    }

    inline void setPoint(const CT_Point &point)
    {
        point.copy(*this);
    }

    inline float* vertex() const
    {
        return const_cast<float*>(&(*this)(0));
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW         // make sure our new allocators are aligned
} EIGEN_ALIGN16;                            // enforce SSE padding for correct memory alignment

/*
POINT_CLOUD_REGISTER_POINT_STRUCT (CT_Point,
    (float, x, x)
    (float, y, y)
    (float, z, z)
)*/

POINT_CLOUD_REGISTER_POINT_WRAPPER(CT_Point, pcl::_PointXYZ)

#endif

inline CT_Point createCtPoint(const float &x = 0, const float &y = 0, const float &z = 0)
{
    CT_Point result;
    result(0) = x; result(1) = y; result(2) = z;
    return result;
}

inline void copyCtPoint( CT_Point &dest, const CT_Point &src)
{
    dest(0) = src(0);
    dest(1) = src(1);
    dest(2) = src(2);
}

#endif // CT_POINT_H
