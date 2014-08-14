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

#include "ct_abstractitemdrawablewithpointcloud.h"

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <limits>

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

CT_StandardAbstractItemDrawableWithPointCloudDrawManager CT_AbstractItemDrawableWithPointCloud::IDWITHPC_DRAW_MANAGER;
CT_INIT_DEFAULT_IA(0, CT_AbstractItemDrawableWithPointCloud)

CT_AbstractItemDrawableWithPointCloud::CT_AbstractItemDrawableWithPointCloud() : CT_AbstractSingularItemDrawable()
{
    m_apci = NULL;
    setBaseDrawManager(&IDWITHPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithPointCloud::CT_AbstractItemDrawableWithPointCloud(const CT_OutAbstractItemModel *model,
                                                                             const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{
    m_apci = NULL;
    setBaseDrawManager(&IDWITHPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithPointCloud::CT_AbstractItemDrawableWithPointCloud(const CT_OutAbstractItemModel *model,
                                                                             const CT_AbstractResult *result,
                                                                             CT_AbstractCIR pcir) : CT_AbstractSingularItemDrawable(model, result)
{
    setPointCloudIndexRegisteredProtected(pcir);
    setBaseDrawManager(&IDWITHPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithPointCloud::CT_AbstractItemDrawableWithPointCloud(const QString &modelName,
                                                                             const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{
    m_apci = NULL;
    setBaseDrawManager(&IDWITHPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithPointCloud::CT_AbstractItemDrawableWithPointCloud(const QString &modelName,
                                                                             const CT_AbstractResult *result,
                                                                             CT_AbstractCIR pcir) : CT_AbstractSingularItemDrawable(modelName, result)
{
    setPointCloudIndexRegisteredProtected(pcir);
    setBaseDrawManager(&IDWITHPC_DRAW_MANAGER);
}

CT_AbstractItemDrawableWithPointCloud::~CT_AbstractItemDrawableWithPointCloud()
{
    clearFromMemory();
}

QString CT_AbstractItemDrawableWithPointCloud::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_AbstractItemDrawableWithPointCloud";
}

void CT_AbstractItemDrawableWithPointCloud::updateBoundingBox()
{
    _minCoordinates.setX(std::numeric_limits<float>::max());
    _minCoordinates.setY(std::numeric_limits<float>::max());
    _minCoordinates.setZ(std::numeric_limits<float>::max());

    _maxCoordinates.setX(-std::numeric_limits<float>::max());
    _maxCoordinates.setY(-std::numeric_limits<float>::max());
    _maxCoordinates.setZ(-std::numeric_limits<float>::max());

    const CT_AbstractCloudIndexT<CT_Point>* pointCloudIndex = getPointCloudIndexT();

    if (pointCloudIndex == NULL) {return;}

    size_t size = pointCloudIndex->size();
    for (size_t i = 0 ; i < size ; i++)
    {
        const CT_Point& point = pointCloudIndex->constTAt(i);
        if (point.x < _minCoordinates.x()) {_minCoordinates.setX(point.x);}
        if (point.y < _minCoordinates.y()) {_minCoordinates.setY(point.y);}
        if (point.z < _minCoordinates.z()) {_minCoordinates.setZ(point.z);}

        if (point.x > _maxCoordinates.x()) {_maxCoordinates.setX(point.x);}
        if (point.y > _maxCoordinates.y()) {_maxCoordinates.setY(point.y);}
        if (point.z > _maxCoordinates.z()) {_maxCoordinates.setZ(point.z);}
    }

    updateCenterFromBoundingBox();
}

const CT_AbstractPointCloud* CT_AbstractItemDrawableWithPointCloud::getPointCloud() const
{
    return PS_REPOSITORY->globalPointCloud();
}

const CT_AbstractPointCloudIndex* CT_AbstractItemDrawableWithPointCloud::getPointCloudIndex() const
{
    return m_apci;
}

const CT_AbstractCloudIndexT<CT_Point>* CT_AbstractItemDrawableWithPointCloud::getPointCloudIndexT() const
{
    if (_pcir.isNull()) {return NULL;}

    return _pcir->abstractCloudIndexT();
}

CT_AbstractItemDrawableWithPointCloud::CT_AbstractCIR CT_AbstractItemDrawableWithPointCloud::getPointCloudIndexRegistered() const
{
    return _pcir;
}

size_t CT_AbstractItemDrawableWithPointCloud::getPointCloudIndexSize() const
{
    return getPointCloudIndex()->size();
}

void CT_AbstractItemDrawableWithPointCloud::clearFromMemory()
{
    deletePointCloud();

    clearFromMemoryProtected();
}

int CT_AbstractItemDrawableWithPointCloud::getFastestIncrement() const
{
    return 16;
}

///////////// PROTECTED /////////////

void CT_AbstractItemDrawableWithPointCloud::deletePointCloud()
{
    _pcir = CT_AbstractCIR();
    m_apci = NULL;
}

CT_AbstractPointCloud* CT_AbstractItemDrawableWithPointCloud::getPointCloud()
{
    return PS_REPOSITORY->globalPointCloud();
}

void CT_AbstractItemDrawableWithPointCloud::setPointCloudIndexRegisteredProtected(CT_AbstractCIR pcir)
{
    deletePointCloud();

    _pcir = pcir;

    if(_pcir.data() != NULL)
        m_apci = dynamic_cast<CT_AbstractPointCloudIndex*>(_pcir->abstractCloudIndexT());
    else
        m_apci = NULL;
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_AbstractItemDrawableWithPointCloud)
#endif
