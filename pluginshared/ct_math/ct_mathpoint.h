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

#ifndef CT_MATHPOINT_H
#define CT_MATHPOINT_H

#include "pluginShared_global.h"

#include "ct_point.h"
#include <math.h>
#include <QMatrix4x4>

class PLUGINSHAREDSHARED_EXPORT CT_MathPoint
{
public:

    enum RotationAxis
    {
        OX, OY, OZ
    };

    typedef enum RotationAxis RotationAxis;

    template<typename xyzPoint>
    inline static float distance2DZAxis(const xyzPoint &p1, const xyzPoint &p2)
    {
        return sqrt(pow(pX(p1)-pX(p2), 2) + pow(pY(p1)-pY(p2), 2));
    }

    template<typename xyzPoint>
    inline static float distance2D(const xyzPoint &p1, const xyzPoint &p2)
    {
        return sqrt(pow(pX(p1)-pX(p2), 2) + pow(pY(p1)-pY(p2), 2));
    }

    template<typename xyzPoint>
    inline static float distance3D(const xyzPoint &p1, const xyzPoint &p2)
    {
        return sqrt(pow(pX(p1)-pX(p2), 2) + pow(pY(p1)-pY(p2), 2) + pow(pZ(p1)-pZ(p2), 2));
    }

    template<typename xyzPoint>
    inline static float angle(const xyzPoint &v1, const xyzPoint &v2)
    {
        float norm1 = v1.length();
        float norm2 = v2.length();

        if ( norm1 == 0 || norm2 == 0 )
        {
            return 0;
        }

        float cosAlpha = xyzPoint::dotProduct(v1,v2) / (norm1*norm2);

        if (cosAlpha > 1 && cosAlpha < 1.000001)
        {
            return 0;
        }
        return acos( cosAlpha );
    }

    template<typename xyzPoint>
    inline static float angleUnsigned(const xyzPoint &v1, const xyzPoint &v2)
    {
        float norm1 = v1.length();
        float norm2 = v2.length();

        if ( norm1 == 0 || norm2 == 0 )
        {
            return 0;
        }

        float cosAlpha = fabs(xyzPoint::dotProduct(v1,v2) ) / (norm1*norm2);

        if (cosAlpha > 1 && cosAlpha < 1.000001)
        {
            return 0;
        }
        return acos( cosAlpha );
    }


    template<typename xyzPoint>
    inline static float distancePointLine ( const xyzPoint &p, const xyzPoint &lineDirection, xyzPoint &pointOnLine )
    {
        // Vector from pointOnLine to p
        xyzPoint polP;
        polP.setX(pX(pointOnLine) - pX(p));
        polP.setY(pY(pointOnLine) - pY(p));
        polP.setZ(pZ(pointOnLine) - pZ(p));

        xyzPoint crossP = xyzPoint::crossProduct( polP, lineDirection );

        return ( crossP.length() / lineDirection.length() );
    }

    template<typename xyzPoint>
    inline static float distancePointPlane ( const xyzPoint &p, const xyzPoint &planeNormal, xyzPoint &pointOnPlane )
    {
        // Vector from pointOnPlane to p
        xyzPoint popP;
        popP.setX(pX(p) - pX(pointOnPlane));
        popP.setY(pY(p) - pY(pointOnPlane));
        popP.setZ(pZ(p) - pZ(pointOnPlane));

        return ( fabs( xyzPoint::dotProduct( popP, planeNormal ) ) / planeNormal.length() );
    }

    template<typename xyzPoint>
    inline static float distancePointPlaneSigned ( const xyzPoint &p, const xyzPoint &planeNormal, xyzPoint &pointOnPlane )
    {
        // Vector from pointOnPlane to p
        xyzPoint popP;
        popP.setX(pX(p) - pX(pointOnPlane));
        popP.setY(pY(p) - pY(pointOnPlane));
        popP.setZ(pZ(p) - pZ(pointOnPlane));

        return ( xyzPoint::dotProduct( popP, planeNormal ) / planeNormal.length() );
    }


    template<typename xyzPoint>
    inline static float distanceOnLineForPointProjection ( const xyzPoint &origin, const xyzPoint &direction, const xyzPoint &point, xyzPoint &projectedPoint)
    {
        float num = pX(direction)*(pX(point) - pX(origin)) + pY(direction)*(pY(point) - pY(origin)) + pZ(direction)*(pZ(point) - pZ(origin));
        float den = pow(pX(direction), 2) + pow(pY(direction), 2) + pow(pZ(direction), 2);

        if (den == 0) {return -1;}
        float length = num / den ;

        projectedPoint.setX(pX(origin) + pX(direction) * length);
        projectedPoint.setY(pY(origin) + pY(direction) * length);
        projectedPoint.setZ(pZ(origin) + pZ(direction) * length);

        return length * sqrt(den);
    }

    // *********************************************************** //
    // Rotation
    // *********************************************************** //

    template<typename xyzPoint>
    inline static xyzPoint rotate( const xyzPoint& p, RotationAxis axis, float alpha )
    {
        float c = cos( alpha );
        float s = sin( alpha );

        return rotate( p, axis, c, s );
    }

    template<typename xyzPoint>
    inline static xyzPoint rotate( const xyzPoint& p, RotationAxis axis, float cosAlpha, float sinAlpha )
    {
        xyzPoint rslt;

        switch ( axis )
        {
            case OX :
            {
                rslt.setX(pX(p));
                rslt.setY((pY(p) * cosAlpha) + ( pZ(p) * (-sinAlpha)));
                rslt.setZ((pY(p) * sinAlpha) + ( pZ(p) * cosAlpha ));
                break;
            }

            case OY :
            {
                rslt.setY(pY(p));
                rslt.setZ((pZ(p) * cosAlpha) + ( pX(p) * (-sinAlpha)));
                rslt.setX((pZ(p) * sinAlpha) + ( pX(p) * cosAlpha ));
                break;
            }

            case OZ :
            {
                rslt.setZ(pZ(p));
                rslt.setX((pX(p) * cosAlpha) + ( pY(p) * (-sinAlpha)));
                rslt.setY((pX(p) * sinAlpha) + ( pY(p) * cosAlpha ));
                break;
            }

            default :
                break;
        }

        return rslt;
    }

    template<typename xyzPoint>
    inline static xyzPoint rotate( const xyzPoint& p, const xyzPoint& rotationAxis, float alpha )
    {
        float c = cos( alpha );
        float s = sin( alpha );
        xyzPoint rotationUnitAxis = rotationAxis / norm(rotationAxis);

        return rotate( p, rotationUnitAxis, c, s );
    }

    template<typename xyzPoint>
    inline static xyzPoint rotate( const xyzPoint& p, const xyzPoint& unitRotationAxis, float cosAlpha, float sinAlpha )
    {
        xyzPoint rslt;
        rslt.setX(  ( ( (pX(unitRotationAxis) * pX(unitRotationAxis)) + ( (1 - (pX(unitRotationAxis) * pX(unitRotationAxis)) ) * cosAlpha) ) * (pX(p)) ) +
                    ( ( (pX(unitRotationAxis) * pY(unitRotationAxis) * ( 1 - cosAlpha ) ) - (pZ(unitRotationAxis) * sinAlpha) ) * (pY(p)) ) +
                    ( ( (pX(unitRotationAxis) * pZ(unitRotationAxis) * ( 1 - cosAlpha ) ) + (pY(unitRotationAxis) * sinAlpha) ) * (pZ(p)) ));

        rslt.setY(  ( ( (pX(unitRotationAxis) * pY(unitRotationAxis) * ( 1 - cosAlpha ) ) + (pZ(unitRotationAxis) * sinAlpha) ) * (pX(p)) ) +
                    ( ( (pY(unitRotationAxis) * pY(unitRotationAxis)) + ( (1 - (pY(unitRotationAxis) * pY(unitRotationAxis)) ) * cosAlpha) ) * (pY(p)) ) +
                    ( ( (pY(unitRotationAxis) * pZ(unitRotationAxis) * ( 1 - cosAlpha ) ) - (pX(unitRotationAxis) * sinAlpha) ) * (pZ(p)) ));

        rslt.setZ(  ( ( (pX(unitRotationAxis) * pZ(unitRotationAxis) * ( 1 - cosAlpha ) ) - (pY(unitRotationAxis) * sinAlpha) ) * (pX(p)) ) +
                    ( ( (pY(unitRotationAxis) * pZ(unitRotationAxis) * ( 1 - cosAlpha ) ) + (pX(unitRotationAxis) * sinAlpha) ) * (pY(p)) ) +
                    ( ( (pZ(unitRotationAxis) * pZ(unitRotationAxis)) + ( (1 - (pZ(unitRotationAxis) * pZ(unitRotationAxis)) ) * cosAlpha) ) * (pZ(p)) ));

        return rslt;
    }

    // En spherique on stocke r, theta, phi
    template<typename xyzPoint>
    inline static void cartesianToSpherical ( const xyzPoint& pCartesian, xyzPoint& outSpherical)
    {
        // On recupere la longueur du vecteur normalisé
        float norme = pCartesian.length();

        if ( norme == 0 )
        {
            outSpherical.setX(0);
            outSpherical.setY(0);
            outSpherical.setZ(0);
        }
        else
        {
            // r est la norme du veteur
            outSpherical.setX(norme);

            // theta depend du signe de y, on ne peut pas diviser par 0 ce qui arrive quand x et y sont nuls
            if ( pX(pCartesian) == 0 && pY(pCartesian) == 0 )
            {
                outSpherical.setY(0);
            }

            // Petit problème lorsque y est nul, on arrive a acos(x/x) qui plante pck acos(1) marche pas...
            else if ( pY(pCartesian) == 0 )
            {
                outSpherical.setY(0);
            }

            else if ( pY(pCartesian) > 0 )
            {
                outSpherical.setY(acos(pX(pCartesian) / sqrt( pX(pCartesian)*pX(pCartesian) + pY(pCartesian)*pY(pCartesian) ) ));
            }

            else
            {
                outSpherical.setY((2 * M_PI) - acos(pX(pCartesian) / sqrt( pX(pCartesian)*pX(pCartesian) + pY(pCartesian)*pY(pCartesian) ) ));
            }

            // phi depend de la hauteur du point
            outSpherical.setZ(acos( pZ(pCartesian) / norme ));
        }
    }


    template<typename xyzPoint>
    inline static void sphericalToCartesian ( const xyzPoint& spher, xyzPoint& cart )
    {
        // En spherique on stocke r, theta, phi
        cart.setX( spher(0) * cos( spher(1) ) * sin( spher(2) ) );
        cart.setY( spher(0) * sin( spher(1) ) * sin( spher(2) ) );
        cart.setZ( spher(0) * cos( spher(2) ) );
    }


    template<class xyzPoint>
    static void transform(const QMatrix4x4 &matrix, xyzPoint &point)
    {
        float x, y, z, w;
        x = pX(point) * matrix(0,0) +
            pY(point) * matrix(0,1) +
            pZ(point) * matrix(0,2) +
            matrix(0,3);
        y = pX(point) * matrix(1,0) +
            pY(point) * matrix(1,1) +
            pZ(point) * matrix(1,2) +
            matrix(1,3);
        z = pX(point) * matrix(2,0) +
            pY(point) * matrix(2,1) +
            pZ(point) * matrix(2,2) +
            matrix(2,3);
        w = pX(point) * matrix(3,0) +
            pY(point) * matrix(3,1) +
            pZ(point) * matrix(3,2) +
            matrix(3,3);

        if (w == 1.0f)
        {
            point.setX(x);
            point.setY(y);
            point.setZ(z);
        }
        else
        {
            point.setX(x/w);
            point.setY(y/w);
            point.setZ(z/w);
        }
    }

    template<typename xyzPoint>
    inline static float pX(const xyzPoint &p) { return p.x(); }

    template<typename xyzPoint>
    inline static float pY(const xyzPoint &p) { return p.y(); }

    template<typename xyzPoint>
    inline static float pZ(const xyzPoint &p) { return p.z(); }

};

//typename CT_Point;

// specialization for CT_Point
template<>
inline float CT_MathPoint::pX(const CT_Point &p) { return p(0); }

template<>
inline float CT_MathPoint::pY(const CT_Point &p) { return p(1); }

template<>
inline float CT_MathPoint::pZ(const CT_Point &p) { return p(2); }

#endif // CT_MATHPOINT_H
