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

#include "ct_abstractshape.h"


CT_AbstractShape::CT_AbstractShape() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _data = NULL;
}

CT_AbstractShape::CT_AbstractShape(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   CT_ShapeData *data) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _data = data;
}

CT_AbstractShape::CT_AbstractShape(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   CT_ShapeData *data) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _data = data;
}

CT_AbstractShape::~CT_AbstractShape()
{
    delete _data;
}

QString CT_AbstractShape::getType() const
{
    return staticGetType();
}

QString CT_AbstractShape::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_AbstractShape";
}

void CT_AbstractShape::setCenterX(float x)
{
    getDataNotConst()->_center.setX(x);
}

void CT_AbstractShape::setCenterY(float y)
{
    getDataNotConst()->_center.setY(y);
}

void CT_AbstractShape::setCenterZ(float z)
{
    getDataNotConst()->_center.setZ(z);
}

float CT_AbstractShape::getCenterX() const
{
    return getDataNotConst()->getCenter().x();
}

float CT_AbstractShape::getCenterY() const
{
    return getDataNotConst()->getCenter().y();
}

float CT_AbstractShape::getCenterZ() const
{
    return getDataNotConst()->getCenter().z();
}

const CT_ShapeData* CT_AbstractShape::getPointerData() const
{
    return _data;
}

const CT_ShapeData& CT_AbstractShape::getData() const
{
    return *_data;
}

const QVector3D& CT_AbstractShape::getCenter() const
{
    return _data->getCenter();
}

const QVector3D& CT_AbstractShape::getDirection() const
{
    return _data->getDirection();
}

double CT_AbstractShape::getDirectionX() const
{
    return _data->getDirection().x();
}

double CT_AbstractShape::getDirectionY() const
{
    return _data->getDirection().y();
}

double CT_AbstractShape::getDirectionZ() const
{
    return _data->getDirection().z();
}

CT_ShapeData* CT_AbstractShape::getDataNotConst() const
{
    return _data;
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_ShapeData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_AbstractShape)
#endif
