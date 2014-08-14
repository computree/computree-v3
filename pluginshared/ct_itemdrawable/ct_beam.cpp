/****************************************************************************

 Copyright (C) 2012-2012 Universite de Sherbrooke, Quebec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA
 Adapted by Alexandre Piboule for Computree 2.0

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

#include "ct_beam.h"
#include <limits>

const CT_StandardBeamDrawManager CT_Beam::BEAM_DRAW_MANAGER;

CT_Beam::CT_Beam() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _origin.setX(0);
    _origin.setY(0);
    _origin.setZ(0);

    _direction.setX(1);
    _direction.setY(0);
    _direction.setZ(0);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _origin.setX(0);
    _origin.setY(0);
    _origin.setZ(0);

    _direction.setX(1);
    _direction.setY(0);
    _direction.setZ(0);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, const QVector3D &origin, const QVector3D &direction) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    assert( !(direction.x() == 0 && direction.y() == 0 && direction.z() == 0) );

    _origin.setX(origin.x());
    _origin.setY(origin.y());
    _origin.setZ(origin.z());

    // Normalizing direction
    QVector3D normedDir = direction.normalized();
    _direction.setX(normedDir.x());
    _direction.setY(normedDir.y());
    _direction.setZ(normedDir.z());

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _origin.x() );
    setCenterY( _origin.y() );
    setCenterZ( _origin.z() );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}


CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _origin.setX(0);
    _origin.setY(0);
    _origin.setZ(0);

    _direction.setX(1);
    _direction.setY(0);
    _direction.setZ(0);

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result, const QVector3D &origin, const QVector3D &direction) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    assert( !(direction.x() == 0 && direction.y() == 0 && direction.z() == 0) );

    _origin.setX(origin.x());
    _origin.setY(origin.y());
    _origin.setZ(origin.z());

    // Normalizing direction
    QVector3D normedDir = direction.normalized();
    _direction.setX(normedDir.x());
    _direction.setY(normedDir.y());
    _direction.setZ(normedDir.z());

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( _origin.x() );
    setCenterY( _origin.y() );
    setCenterZ( _origin.z() );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}


CT_Beam::~CT_Beam()
{
    // The destructor does not do anything. The destructor from the CT_AbstractItemDrawableWithoutPointCloud class will also be called
}

bool CT_Beam::intersect(const QVector3D& bot, const QVector3D& top, QVector3D &nearP, QVector3D &farP) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot.x(), top.x(), _origin.x(), _direction.x(), t0, t1)) {return false;}
    if (!updateIntervals(bot.y(), top.y(), _origin.y(), _direction.y(), t0, t1)) {return false;}
    if (!updateIntervals(bot.z(), top.z(), _origin.z(), _direction.z(), t0, t1)) {return false;}

    nearP.setX(_origin.x() + _direction.x()*t0);
    nearP.setY(_origin.y() + _direction.y()*t0);
    nearP.setZ(_origin.z() + _direction.z()*t0);

    farP.setX(_origin.x() + _direction.x()*t1);
    farP.setY(_origin.y() + _direction.y()*t1);
    farP.setZ(_origin.z() + _direction.z()*t1);

    return true;
}

bool CT_Beam::intersect(const QVector3D& bot, const QVector3D& top) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot.x(), top.x(), _origin.x(), _direction.x(), t0, t1)) {return false;}
    if (!updateIntervals(bot.y(), top.y(), _origin.y(), _direction.y(), t0, t1)) {return false;}
    if (!updateIntervals(bot.z(), top.z(), _origin.z(), _direction.z(), t0, t1)) {return false;}

    return true;
}

bool CT_Beam::updateIntervals(const double &bot, const double &top, const double &origin, const double &direction, double &t0, double &t1) const
{
    // Update interval for bounding box slab
    double invRayDir = 1.f / direction;
    double tNear = (bot - origin) * invRayDir;
    double tFar  = (top - origin) * invRayDir;

    // Update parametric interval from slab intersection $t$s
    if (tNear > tFar) std::swap(tNear, tFar);

    t0 = tNear > t0 ? tNear : t0;
    t1 = tFar  < t1 ? tFar  : t1;

    if (t0 > t1 && t0 - t1 > EPSILON_INTERSECTION_RAY ) // t0 being always > t1, (t0-t1) is always positive
    {
        return false;
    }
    return true;
}

QString CT_Beam::getType() const
{
    return staticGetType();
}

QString CT_Beam::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_Beam";
}

CT_Beam* CT_Beam::copy(const CT_OutAbstractItemModel *model,
                     const CT_AbstractResult *result,
                     CT_ResultCopyModeList copyModeList)
{
    CT_Beam *ray = new CT_Beam(model, result, _origin, _direction);
    ray->setId(id());

    ray->setAlternativeDrawManager(getAlternativeDrawManager());

    return ray;
}
