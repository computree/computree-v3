/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#include "ct_referencepoint.h"
#include "qdebug.h"

const CT_StandardReferencePointDrawManager CT_ReferencePoint::REFERENCEPOINT_DRAW_MANAGER;
int CT_ReferencePoint::_currentRefId = 0;

CT_ReferencePoint::CT_ReferencePoint() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _refId = -1; // referencePoint non valide

    setBaseDrawManager(&REFERENCEPOINT_DRAW_MANAGER);
}

CT_ReferencePoint::CT_ReferencePoint(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result,
                                     double x, double y, double z, double buffer) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    CT_AbstractItemDrawableWithoutPointCloud::setCenterX(x);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterY(y);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterZ(z);

    _minCoordinates(0) = x;
    _minCoordinates(1) = y;
    _minCoordinates(2) = z;

    _maxCoordinates(0) = x;
    _maxCoordinates(1) = y;
    _maxCoordinates(2) = z;

    setXYBuffer(buffer);
    _refId = _currentRefId++;

    setBaseDrawManager(&REFERENCEPOINT_DRAW_MANAGER);
}

// Contructeur privé pour la copie
CT_ReferencePoint::CT_ReferencePoint(const CT_OutAbstractSingularItemModel *model,
                                     const CT_AbstractResult *result,
                                     double x, double y, double z, double buffer, int refId) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    CT_AbstractItemDrawableWithoutPointCloud::setCenterX(x);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterY(y);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterZ(z);

    _minCoordinates(0) = x;
    _minCoordinates(1) = y;
    _minCoordinates(2) = z;

    _maxCoordinates(0) = x;
    _maxCoordinates(1) = y;
    _maxCoordinates(2) = z;

    setXYBuffer(buffer);
    _refId = refId;

    setBaseDrawManager(&REFERENCEPOINT_DRAW_MANAGER);
}

CT_ReferencePoint::CT_ReferencePoint(const QString &modelName,
                                     const CT_AbstractResult *result,
                                     double x, double y, double z, double buffer, int refId) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    CT_AbstractItemDrawableWithoutPointCloud::setCenterX(x);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterY(y);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterZ(z);

    _minCoordinates(0) = x;
    _minCoordinates(1) = y;
    _minCoordinates(2) = z;

    _maxCoordinates(0) = x;
    _maxCoordinates(1) = y;
    _maxCoordinates(2) = z;

    setXYBuffer(buffer);
    _refId = refId;

    setBaseDrawManager(&REFERENCEPOINT_DRAW_MANAGER);
}

CT_ReferencePoint::CT_ReferencePoint(const QString &modelName,
                                     const CT_AbstractResult *result,
                                     double x, double y, double z, double buffer) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    CT_AbstractItemDrawableWithoutPointCloud::setCenterX(x);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterY(y);
    CT_AbstractItemDrawableWithoutPointCloud::setCenterZ(z);

    _minCoordinates(0) = x;
    _minCoordinates(1) = y;
    _minCoordinates(2) = z;

    _maxCoordinates(0) = x;
    _maxCoordinates(1) = y;
    _maxCoordinates(2) = z;

    setXYBuffer(buffer);
    _refId = _currentRefId++;

    setBaseDrawManager(&REFERENCEPOINT_DRAW_MANAGER);
}

QString CT_ReferencePoint::getType() const
{
    return staticGetType();
}

QString CT_ReferencePoint::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_ReferencePoint";
}

void CT_ReferencePoint::setXYBuffer(double buffer)
{
    _xyBuffer = buffer;
}

void CT_ReferencePoint::setCenterX(double x)
{
    Q_UNUSED(x)

    qDebug() << "setCenterX n'a aucun aucun effet dans le cas d'un CT_ReferencePoint";
}

void CT_ReferencePoint::setCenterY(double y)
{
    Q_UNUSED(y)

    qDebug() << "setCenterY n'a aucun aucun effet dans le cas d'un CT_ReferencePoint";
}

void CT_ReferencePoint::setCenterZ(double z)
{
    Q_UNUSED(z)

    qDebug() << "setCenterZ n'a aucun aucun effet dans le cas d'un CT_ReferencePoint";
}

void CT_ReferencePoint::setCenterCoordinate(const Eigen::Vector3d &center)
{
    Q_UNUSED(center)

    qDebug() << "setCenterCoordinate n'a aucun aucun effet dans le cas d'un CT_ReferencePoint";
}

CT_AbstractItemDrawable* CT_ReferencePoint::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_ReferencePoint *ref = new CT_ReferencePoint((const CT_OutAbstractSingularItemModel *)model, result, x(), y(), z(), xyBuffer(), _refId);
    ref->setId(id());

    ref->setAlternativeDrawManager(getAlternativeDrawManager());

    return ref;
}

CT_AbstractItemDrawable *CT_ReferencePoint::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    CT_ReferencePoint *ref = new CT_ReferencePoint(modelName, result, x(), y(), z(), xyBuffer(), _refId);
    ref->setId(id());

    ref->setAlternativeDrawManager(getAlternativeDrawManager());

    return ref;
}
