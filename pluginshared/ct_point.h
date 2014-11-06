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

enum RotationAxis
{
    OX, OY, OZ
};

typedef enum RotationAxis RotationAxis;

//#define USE_EIGEN_POINT

#ifdef USE_EIGEN_POINT
#include <Eigen/Core>

class CT_Point : public Eigen::Vector3f {

public:
    inline float& getX() { return (*this)(0); }
    inline float& getY() { return (*this)(1); }
    inline float& getZ() { return (*this)(2); }

    inline float getX() const { return (*this)(0); }
    inline float getY() const { return (*this)(1); }
    inline float getZ() const { return (*this)(2); }

    inline void setX(const float &_x) { (*this)(0) = _x; }
    inline void setY(const float &_y) { (*this)(1) = _y; }
    inline void setZ(const float &_z) { (*this)(2) = _z; }
};

#else

#ifdef USE_PCL

#define PCL_NO_PRECOMPILE
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/octree/octree_search.h>
#include <pcl/octree/impl/octree_search.hpp>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/impl/voxel_grid.hpp>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/impl/statistical_outlier_removal.hpp>

struct _CT_Point
{
    union EIGEN_ALIGN16 {
        float data[4];
        struct {
            float x;
            float y;
            float z;
        };
    };
    inline Eigen::Map<Eigen::Vector3f> getVector3fMap () { return (Eigen::Vector3f::Map (data)); }
    inline const Eigen::Map<const Eigen::Vector3f> getVector3fMap () const { return (Eigen::Vector3f::Map (data)); }
    inline Eigen::Map<Eigen::Vector4f, Eigen::Aligned> getVector4fMap () { return (Eigen::Vector4f::MapAligned (data)); }
    inline const Eigen::Map<const Eigen::Vector4f, Eigen::Aligned> getVector4fMap () const { return (Eigen::Vector4f::MapAligned (data)); }
    inline Eigen::Map<Eigen::Array3f> getArray3fMap () { return (Eigen::Array3f::Map (data)); }
    inline const Eigen::Map<const Eigen::Array3f> getArray3fMap () const { return (Eigen::Array3f::Map (data)); }
    inline Eigen::Map<Eigen::Array4f, Eigen::Aligned> getArray4fMap () { return (Eigen::Array4f::MapAligned (data)); }
    inline const Eigen::Map<const Eigen::Array4f, Eigen::Aligned> getArray4fMap () const { return (Eigen::Array4f::MapAligned (data)); }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

struct EIGEN_ALIGN16 CT_Point : public _CT_Point
{

#else
class PLUGINSHAREDSHARED_EXPORT CT_Point
{
public:
    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
#endif

    inline CT_Point()
    {
        x = y = z = 0.0f;
        #ifdef USE_PCL
        data[3] = 1.0f;
        #endif
    }

    inline CT_Point(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        #ifdef USE_PCL
        data[3] = 1.0f;
        #endif
    }

    inline float getX() const { return x; }
    inline float getY() const { return y; }
    inline float getZ() const { return z; }

    inline void setX(const float &_x) { x = _x; }
    inline void setY(const float &_y) { y = _y; }
    inline void setZ(const float &_z) { z = _z; }

    inline float* vertex() const { return const_cast<float*>(&x); }

    #ifndef USE_PCL
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
    #endif

    // For Mesh
    inline CT_Point& P() { return *this; }

    inline float dot (const CT_Point &p) const { return ( p.x*x + p.y*y + p.z*z ); }

    static inline float dotProduct(const CT_Point &p1, const CT_Point &p2) { return p1.dot(p2); }

    inline CT_Point cross (const CT_Point &p) const
    {
        CT_Point rslt;
        rslt.x = y*p.z - z*p.y;
        rslt.y = z*p.x - x*p.z;
        rslt.z = x*p.y - y*p.x;
        return rslt;
    }

    static inline CT_Point crossProduct(const CT_Point &p1, const CT_Point &p2) { return p1.cross(p2); }

    inline float length() const { return ( sqrt( x*x + y*y + z*z ) ); }

    inline void  normalize ()
    {
        if ( x != 0 || y != 0 || z != 0 )
        {
            float norme = length();
            x /= norme;
            y /= norme;
            z /= norme;
        }
    }

    inline CT_Point  normalized () const
    {
        CT_Point p(*this);

        if ( p.x != 0 || p.y != 0 || p.z != 0 )
        {
            float norme = length();
            p.x /= norme;
            p.y /= norme;
            p.z /= norme;
        }

        return p;
    }

    inline void rotate(const RotationAxis &axis, const float &cosAlpha, const float &sinAlpha)
    {
        int mapY, mapZ;

        switch ( axis )
        {
            case OX :
            {
                mapY = 1;
                mapZ = 2;
                break;
            }

            case OY :
            {
                mapY = 2;
                mapZ = 0;
                break;
            }

            case OZ :
            {
                mapY = 0;
                mapZ = 1;
                break;
            }

            default :
                break;
        }

        CT_Point tmp;
        for ( int i = 0 ; i < 3 ; i++ )
            tmp.data[i] = data[i];

        data[mapY] = (tmp.data[mapY] * cosAlpha) + ( tmp.data[mapZ] * (-sinAlpha) );
        data[mapZ] = (tmp.data[mapY] * sinAlpha) + ( tmp.data[mapZ] * cosAlpha );
    }

    inline void rotate(const RotationAxis &axis, const float &alpha)
    {
        float c = cos( alpha );
        float s = sin( alpha );
        rotate( axis, c, s );
    }

    inline void rotate(const CT_Point& unitRotationAxis, const float &cosAlpha, const float &sinAlpha)
    {
        // Copie du point courant
        CT_Point tmp;
        for ( int i = 0 ; i < 3 ; i++ )
            tmp.data[i] = data[i];

        x = ( ( (unitRotationAxis.x * unitRotationAxis.x) + ( (1 - (unitRotationAxis.x * unitRotationAxis.x) ) * cosAlpha) ) * (tmp.x) ) +
            ( ( (unitRotationAxis.x * unitRotationAxis.y * ( 1 - cosAlpha ) ) - (unitRotationAxis.z * sinAlpha) ) * (tmp.y) ) +
            ( ( (unitRotationAxis.x * unitRotationAxis.z * ( 1 - cosAlpha ) ) + (unitRotationAxis.y * sinAlpha) ) * (tmp.z) );

        y = ( ( (unitRotationAxis.x * unitRotationAxis.y * ( 1 - cosAlpha ) ) + (unitRotationAxis.z * sinAlpha) ) * (tmp.x) ) +
            ( ( (unitRotationAxis.y * unitRotationAxis.y) + ( (1 - (unitRotationAxis.y * tmp.y) ) * cosAlpha) ) * (tmp.y) ) +
            ( ( (unitRotationAxis.y * unitRotationAxis.z * ( 1 - cosAlpha ) ) - (unitRotationAxis.x * sinAlpha) ) * (tmp.z) );

        z = ( ( (unitRotationAxis.x * unitRotationAxis.z * ( 1 - cosAlpha ) ) - (unitRotationAxis.y * sinAlpha) ) * (tmp.x) ) +
            ( ( (unitRotationAxis.y * unitRotationAxis.z * ( 1 - cosAlpha ) ) + (unitRotationAxis.x * sinAlpha) ) * (tmp.y) ) +
            ( ( (unitRotationAxis.z * unitRotationAxis.z) + ( (1 - (unitRotationAxis.z * unitRotationAxis.z) ) * cosAlpha) ) * (tmp.z) );
    }

    inline void rotate( const CT_Point& rotationAxis, const float &alpha)
    {
        float c = cos( alpha );
        float s = sin( alpha );

        // Normalisation de l,axe de rotation
        CT_Point rotationUnitAxis;
        for ( int i = 0 ; i < 3 ; i++ )
             rotationUnitAxis.data[i] = rotationAxis.data[i] / rotationAxis.length();

        rotate( rotationUnitAxis, c, s );
    }

    inline void print( const std::string &labelBefore = "", const std::string &labelAfter = "") const
    {
        std::cout << labelBefore.c_str() << "(" << x << "," << y << "," << z << ")" << labelAfter << std::endl;
    }

#ifdef USE_PCL
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW       // make sure our new allocators are aligned
} EIGEN_ALIGN16;                        // enforce SSE padding for correct memory alignment

POINT_CLOUD_REGISTER_POINT_STRUCT (CT_Point,           // here we assume a XYZ
                                   (float, x, x)
                                   (float, y, y)
                                   (float, z, z))

POINT_CLOUD_REGISTER_POINT_WRAPPER(CT_Point, _CT_Point)
#else
};
#endif

inline CT_Point createCtPoint(const float &x = 0, const float &y = 0, const float &z = 0)
{
    CT_Point result;
    result.x = x; result.y = y; result.z = z;
    return result;
}

inline void copyCtPoint( CT_Point &dest, const CT_Point &src)
{
    dest.x = src.x;
    dest.y = src.y;
    dest.z = src.z;
}
#endif

#endif // CT_POINT_H
