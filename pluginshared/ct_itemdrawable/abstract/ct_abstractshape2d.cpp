/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#include "ct_abstractshape2d.h"
#include "ct_global/ct_context.h"


CT_AbstractShape2D::CT_AbstractShape2D() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _data = NULL;
    _zValue = 0;
    _zValueDefined = false;
}

CT_AbstractShape2D::CT_AbstractShape2D(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   CT_Shape2DData *data) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _data = data;
    _zValue = 0;
    _zValueDefined = false;

    if (_data != NULL)
    {
        data->getBoundingBox(_minCoordinates, _maxCoordinates);

        const Eigen::Vector2d &center2D = data->getCenter();
        Eigen::Vector3d center(center2D(0), center2D(1), 0);
        CT_AbstractItemDrawableWithoutPointCloud::setCenterCoordinate(center);
    }

}

CT_AbstractShape2D::CT_AbstractShape2D(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   CT_Shape2DData *data) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _data = data;
    _zValue = 0;
    _zValueDefined = false;

    if (_data != NULL)
    {
        data->getBoundingBox(_minCoordinates, _maxCoordinates);

        _minCoordinates(2) = 0;
        _maxCoordinates(2) = 0;

        const Eigen::Vector2d &center2D = data->getCenter();
        Eigen::Vector3d center(center2D(0), center2D(1), 0);
        CT_AbstractItemDrawableWithoutPointCloud::setCenterCoordinate(center);
    }
}

CT_AbstractShape2D::~CT_AbstractShape2D()
{
    delete _data;
}

QString CT_AbstractShape2D::getType() const
{
    return staticGetType();
}

QString CT_AbstractShape2D::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_AbstractShape2D";
}

void CT_AbstractShape2D::setCenterX(double x)
{
    getDataNotConst()->_center(0) = x;
}

void CT_AbstractShape2D::setCenterY(double y)
{
    getDataNotConst()->_center(1) = y;
}

void CT_AbstractShape2D::setCenterCoordinate(const Eigen::Vector3d &center)
{
    getDataNotConst()->_center(0) = center(0);
    getDataNotConst()->_center(1) = center(1);

    Eigen::Vector3d center3D(center(0), center(1), 0);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterCoordinate(center3D);
}

void CT_AbstractShape2D::setZValue(double z)
{
    _zValue = z;
    _zValueDefined = true;
    setCenterZ(z);
}

double CT_AbstractShape2D::getZValue() const
{
    return _zValue;
}

bool CT_AbstractShape2D::isZValueDefined() const
{
    return _zValueDefined;
}

double CT_AbstractShape2D::getCenterX() const
{
    return getDataNotConst()->getCenter()(0);
}

double CT_AbstractShape2D::getCenterY() const
{
    return getDataNotConst()->getCenter()(1);
}

const CT_Shape2DData* CT_AbstractShape2D::getPointerData() const
{
    return _data;
}

const CT_Shape2DData& CT_AbstractShape2D::getData() const
{
    return *_data;
}

const Eigen::Vector2d &CT_AbstractShape2D::getCenter() const
{
    return _data->getCenter();
}

void CT_AbstractShape2D::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    CT_AbstractItemDrawableWithoutPointCloud::getBoundingBox(min, max);
    double zplane = CT_Context::staticInstance()->getZPlaneFor2DShapes();

    min(2) = zplane;
    max(2) = zplane;
}

CT_Shape2DData* CT_AbstractShape2D::getDataNotConst() const
{
    return _data;
}
