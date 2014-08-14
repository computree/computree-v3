/****************************************************************************

 Copyright (C) 2012-2012 Université de Sherbrooke, Québec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "ct_scanner.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

// Initializing the draw manager
const CT_StandardScannerDrawManager CT_Scanner::CT_SCANNER_DRAW_MANAGER;

CT_Scanner::CT_Scanner(int scanID, bool clockWise) : CT_AbstractItemDrawableWithoutPointCloud()
{
    _scanID = scanID;
    _position = QVector3D(0,0,0);
    _zVector = QVector3D(0,0,1);
    _hFov = 0;
    _vFov = 0;
    _hRes = 0;
    _vRes = 0;
    _initTheta = 0;
    _initPhi = 0;
    _nHRays = 0;
    _nVRays = 0;
    _clockWise = clockWise;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _position.x() );
    setCenterY( _position.y() );
    setCenterZ( _position.z() );

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, int scanId, bool clocWise) : CT_AbstractItemDrawableWithoutPointCloud (model, result )
{
    _scanID = scanId;
    _position = QVector3D(0,0,0);
    _zVector = QVector3D(0,0,1);
    _hFov = 0;
    _vFov = 0;
    _hRes = 0;
    _vRes = 0;
    _initTheta = 0;
    _initPhi = 0;
    _nHRays = 0;
    _nVRays = 0;
    _clockWise = clocWise;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _position.x() );
    setCenterY( _position.y() );
    setCenterZ( _position.z() );

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const CT_OutAbstractItemModel *model,
                       const CT_AbstractResult *result,
                       int scanID,
                       const QVector3D &origin,
                       const QVector3D &zVector,
                       double hFov,
                       double vFov,
                       double hRes,
                       double vRes,
                       double initTheta,
                       double initPhi,
                       bool clockWise,
                       bool radians) : CT_AbstractItemDrawableWithoutPointCloud (model, result )
{
    _scanID = scanID;
    _position = origin;
    _zVector = zVector;
    _clockWise = clockWise;

    if ( radians )
    {
        _hFov = hFov;
        _vFov = vFov;
        _hRes = hRes;
        _vRes = vRes;

        _clockWise ? _initTheta = -initTheta : _initTheta = initTheta;
        _initPhi = initPhi;
    }

    else
    {
        _hFov = hFov * DEG2RAD;
        _vFov = vFov * DEG2RAD;
        _hRes = hRes * DEG2RAD;
        _vRes = vRes * DEG2RAD;

        _clockWise ? _initTheta = -initTheta*DEG2RAD : _initTheta = initTheta*DEG2RAD;

        _initPhi = initPhi*DEG2RAD;
    }

    // Calculates the number of horizontal and vertical rays
    _nHRays = (int)ceil(fabs(_hFov/_hRes));
    _nVRays = (int)ceil(fabs(_vFov/_vRes));

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _position.x() );
    setCenterY( _position.y() );
    setCenterZ( _position.z() );

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}



CT_Scanner::CT_Scanner(const QString &modelName, const CT_AbstractResult *result, int scanId, bool clocWise) : CT_AbstractItemDrawableWithoutPointCloud (modelName, result )
{
    _scanID = scanId;
    _position = QVector3D(0,0,0);
    _zVector = QVector3D(0,0,1);
    _hFov = 0;
    _vFov = 0;
    _hRes = 0;
    _vRes = 0;
    _initTheta = 0;
    _initPhi = 0;
    _nHRays = 0;
    _nVRays = 0;
    _clockWise = clocWise;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _position.x() );
    setCenterY( _position.y() );
    setCenterZ( _position.z() );

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const QString &modelName,
                       const CT_AbstractResult *result,
                       int scanID,
                       const QVector3D &origin,
                       const QVector3D &zVector,
                       double hFov,
                       double vFov,
                       double hRes,
                       double vRes,
                       double initTheta,
                       double initPhi,
                       bool clockWise,
                       bool radians) : CT_AbstractItemDrawableWithoutPointCloud (modelName, result )
{
    _scanID = scanID;
    _position = origin;
    _zVector = zVector;
    _clockWise = clockWise;

    if ( radians )
    {
        _hFov = hFov;
        _vFov = vFov;
        _hRes = hRes;
        _vRes = vRes;

        _clockWise ? _initTheta = -initTheta : _initTheta = initTheta;
        _initPhi = initPhi;
    }

    else
    {
        _hFov = hFov * DEG2RAD;
        _vFov = vFov * DEG2RAD;
        _hRes = hRes * DEG2RAD;
        _vRes = vRes * DEG2RAD;

        _clockWise ? _initTheta = -initTheta*DEG2RAD : _initTheta = initTheta*DEG2RAD;

        _initPhi = initPhi*DEG2RAD;
    }

    // Calculates the number of horizontal and vertical rays
    _nHRays = (int)ceil(fabs(_hFov/_hRes));
    _nVRays = (int)ceil(fabs(_vFov/_vRes));

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _position.x() );
    setCenterY( _position.y() );
    setCenterZ( _position.z() );

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::~CT_Scanner()
{
    // Nothing to do
}

CT_Beam *CT_Scanner::beam(int i, int j, bool moreStability) const
{
    assert ( i >= 0 && j >= 0 );
    assert ( i <= _nHRays && j <= _nVRays );

    double theta = _initTheta + (i * _hRes);
    double phi = _initPhi + (j * _vRes);

    // If clockwise, then real theta equals opposite to initial
    if ( _clockWise )
    {
        theta *= -1;
    }

    // The direction is calculated using spherical coordinates, the origin is the scaning position
    float sinPhi = sin( phi );
    float cosPhi = cos ( phi );
    float sinTheta = sin ( theta );
    float cosTheta = cos ( theta );

    QVector3D position, direction;
    // The position has to be parsed from Qvector3D to CT_Point
    position.setX(_position.x());
    position.setY(_position.y());
    position.setZ(_position.z());

    if ( moreStability )
    {
        // We avoid too small numbers that leads to numerical instability
        fabs(sinPhi*cosTheta) > SCANNER_EPSILON ? direction.setX(sinPhi*cosTheta) : direction.setZ(0);
        fabs(sinPhi*sinTheta) > SCANNER_EPSILON ? direction.setY(sinPhi*sinTheta) : direction.setZ(0);
        fabs(cosPhi) > SCANNER_EPSILON ? direction.setZ(cosPhi) : direction.setZ(0);
    }

    else
    {
        direction.setX(sinPhi*cosTheta);
        direction.setY(sinPhi*sinTheta);
        direction.setZ(cosPhi);
    }

    return new CT_Beam(NULL, NULL, position, direction );
}

void CT_Scanner::beam(int i, int j, CT_Beam &beam, bool moreStability) const
{
    assert ( i >= 0 && j >= 0 );
    assert ( i <= _nHRays && j <= _nVRays );

    double theta = _initTheta + (i * _hRes);
    double phi = _initPhi + (j * _vRes);

    // If clockwise, then real theta equals opposite to initial
    if ( _clockWise )
    {
        theta *= -1;
    }

    // The direction is calculated using spherical coordinates, the origin is the scaning position
    float sinPhi = sin( phi );
    float cosPhi = cos ( phi );
    float sinTheta = sin ( theta );
    float cosTheta = cos ( theta );

    QVector3D position, direction;
    // The position has to be parsed from Qvector3D to CT_Point
    position.setX(_position.x());
    position.setY(_position.y());
    position.setZ(_position.z());

    if ( moreStability )
    {
        // We avoid too small numbers that leads to numerical instability
        fabs(sinPhi*cosTheta) > SCANNER_EPSILON ? direction.setX(sinPhi*cosTheta) : direction.setZ(0);
        fabs(sinPhi*sinTheta) > SCANNER_EPSILON ? direction.setY(sinPhi*sinTheta) : direction.setZ(0);
        fabs(cosPhi) > SCANNER_EPSILON ? direction.setZ(cosPhi) : direction.setZ(0);
    }

    else
    {
        direction.setX(sinPhi*cosTheta);
        direction.setY(sinPhi*sinTheta);
        direction.setZ(cosPhi);
    }

    beam.setOrigin(position);
    beam.setDirection(direction);
}

QString CT_Scanner::getType() const
{
    return staticGetType();
}

QString CT_Scanner::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_Scanner";
}

CT_Scanner* CT_Scanner::copy(const CT_OutAbstractItemModel *model,
                             const CT_AbstractResult *result,
                             CT_ResultCopyModeList copyModeList)
{
    CT_Scanner *sc = new CT_Scanner(model, result, _scanID, _position, _zVector, _hFov, _vFov, _hRes, _vRes, _initTheta, _initPhi, _clockWise);
    sc->setId(id());
    sc->setAlternativeDrawManager(getAlternativeDrawManager());

    return sc;
}
