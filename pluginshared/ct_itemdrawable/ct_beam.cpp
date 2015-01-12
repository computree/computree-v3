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
    _origin(0) = 0;
    _origin(1) = 0;
    _origin(2) = 0;

    _direction(0) = 1;
    _direction(1) = 0;
    _direction(2) = 0;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _origin(0) = 0;
    _origin(1) = 0;
    _origin(2) = 0;

    _direction(0) = 1;
    _direction(1) = 0;
    _direction(2) = 0;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result, const Eigen::Vector3d &origin, const Eigen::Vector3d &direction) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    assert( !(direction(0) == 0 && direction(1) == 0 && direction(2) == 0) );

    _origin = origin;

    // Normalizing direction
    _direction = direction.normalized();

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterCoordinate(_origin);

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}


CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _origin(0) = 0;
    _origin(1) = 0;
    _origin(2) = 0;

    _direction(0) = 1;
    _direction(1) = 0;
    _direction(2) = 0;

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterX( 0 );
    setCenterY( 0 );
    setCenterZ( 0 );

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}

CT_Beam::CT_Beam(const QString &modelName, const CT_AbstractResult *result, const Eigen::Vector3d &origin, const Eigen::Vector3d &direction) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    assert( !(direction(0) == 0 && direction(1) == 0 && direction(2) == 0) );

    _origin = origin;

    // Normalizing direction
    _direction = direction.normalized();

    // Setting the center attribute from the CT_AbstractItemDrawableWithoutPointCloud class
    setCenterCoordinate(_origin);

    setBaseDrawManager(&BEAM_DRAW_MANAGER);
}


CT_Beam::~CT_Beam()
{
    // The destructor does not do anything. The destructor from the CT_AbstractItemDrawableWithoutPointCloud class will also be called
}

bool CT_Beam::intersect(const Eigen::Vector3d& bot, const Eigen::Vector3d& top, Eigen::Vector3d &nearP, Eigen::Vector3d &farP) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot(0), top(0), _origin(0), _direction(0), t0, t1)) {return false;}
    if (!updateIntervals(bot(1), top(1), _origin(1), _direction(1), t0, t1)) {return false;}
    if (!updateIntervals(bot(2), top(2), _origin(2), _direction(2), t0, t1)) {return false;}


    nearP = _origin + _direction*t0;
    farP  = _origin + _direction*t1;

    return true;
}

bool CT_Beam::intersect(const Eigen::Vector3d& bot, const Eigen::Vector3d& top) const
{
    double t0 = 0;
    double t1 = std::numeric_limits<double>::max();

    if (!updateIntervals(bot(0), top(0), _origin(0), _direction(0), t0, t1)) {return false;}
    if (!updateIntervals(bot(1), top(1), _origin(1), _direction(1), t0, t1)) {return false;}
    if (!updateIntervals(bot(2), top(2), _origin(2), _direction(2), t0, t1)) {return false;}

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
    CT_Beam *ray = new CT_Beam((const CT_OutAbstractSingularItemModel *)model, result, _origin, _direction);
    ray->setId(id());

    ray->setAlternativeDrawManager(getAlternativeDrawManager());

    return ray;
}
