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

#include "ct_circle.h"

#include "math.h"

const CT_StandardCircleDrawManager CT_Circle::CIRCLE_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Circle)

CT_Circle::CT_Circle() : CT_AbstractShape()
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
}

CT_Circle::CT_Circle(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_CircleData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
//    if (data!=NULL) {computeBoundingBox();}
}

CT_Circle::CT_Circle(const QString &modelName,
                     const CT_AbstractResult *result, CT_CircleData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&CIRCLE_DRAW_MANAGER);
//    if (data!=NULL) {computeBoundingBox();}
}

QString CT_Circle::getType() const
{
    return staticGetType();
}

QString CT_Circle::staticGetType()
{
    return CT_AbstractShape::staticGetType() + "/CT_Circle";
}

float CT_Circle::getRadius() const
{
    return ((const CT_CircleData&)getData()).getRadius();
}

float CT_Circle::getError() const
{
    return ((const CT_CircleData&)getData()).getError();
}

CT_AbstractItemDrawable* CT_Circle::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Circle *circle = new CT_Circle((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_CircleData&)getData()).clone() : NULL);
    circle->setId(id());
    circle->setAlternativeDrawManager(getAlternativeDrawManager());

    return circle;
}

CT_Circle* CT_Circle::staticCreateZAxisAlignedCircleFromPointCloud(const CT_OutAbstractSingularItemModel *model,
                                                                   quint64 id,
                                                                   const CT_AbstractResult *result,
                                                                   const CT_AbstractPointCloud &pointCloud,
                                                                   const CT_AbstractPointCloudIndex &pointCloudIndex,
                                                                   double z)
{
    Q_UNUSED(id)

    CT_CircleData *data = CT_CircleData::staticCreateZAxisAlignedCircleDataFromPointCloud(pointCloud, pointCloudIndex, z);

    if(data == NULL)
    {
        return NULL;
    }

    return new CT_Circle((const CT_OutAbstractSingularItemModel *)model, result, data);
}

// Code non vérifié...
//void CT_Circle::computeBoundingBox()
//{
//    const CT_CircleData &data = (const CT_CircleData&)getData();

//    const QVector3D &center = data.getCenter();
//    const QVector3D &direction = data.getDirection();
//    float radius = data.getRadius();

//    QVector3D u(0,0,0);

//    if(fabs(direction.x()) >= fabs(direction.y()))
//    {
//        double factor = 1.0/sqrt(direction.x()*direction.x()+direction.z()*direction.z());
//        u.setX(-direction.z()*factor);
//        u.setZ(direction.x()*factor);
//    }
//    else
//    {
//        double factor = 1.0/sqrt(direction.y()*direction.y()+direction.z()*direction.z());
//        u.setY(direction.z()*factor);
//        u.setZ(-direction.y()*factor);
//    }

//    QVector3D v = QVector3D::crossProduct(direction, u);
//    v.normalize();

//    _minCoordinates.setX(center.x() - radius*u.x() - radius*v.x());
//    _minCoordinates.setY(center.y() - radius*u.y() - radius*v.y());
//    _minCoordinates.setZ(center.z() - radius*u.z() - radius*v.z());

//    _maxCoordinates.setX(center.x() + radius*u.x() + radius*v.x());
//    _maxCoordinates.setY(center.y() + radius*u.y() + radius*v.y());
//    _maxCoordinates.setZ(center.z() + radius*u.z() + radius*v.z());
//}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_CircleData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Circle)
#endif
