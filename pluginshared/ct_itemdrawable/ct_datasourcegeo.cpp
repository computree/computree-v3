/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ct_datasourcegeo.h"


CT_DataSourceGeo::CT_DataSourceGeo() : CT_DataSource()
{
}

CT_DataSourceGeo::CT_DataSourceGeo(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result, CT_AbstractReader* readerPrototype) : CT_DataSource(model, result, readerPrototype)
{

}

CT_DataSourceGeo::CT_DataSourceGeo(const QString &modelName,
                                   const CT_AbstractResult *result, CT_AbstractReader* readerPrototype) : CT_DataSource(modelName, result, readerPrototype)
{

}

CT_DataSourceGeo::~CT_DataSourceGeo()
{
}

QString CT_DataSourceGeo::getType() const
{
    return staticGetType();
}

QString CT_DataSourceGeo::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_DataSourceGeo";
}

bool CT_DataSourceGeo::addReader(CT_AbstractReader *reader)
{
    if (reader->getHeader()->hasBoundingBox())
    {
        if (CT_DataSource::addReader(reader))
        {
            Eigen::Vector3d min, max;
            reader->getHeader()->getBoundingBox(min, max);

            if (min(0) < _minCoordinates(0)) {_minCoordinates(0) = min(0);}
            if (min(1) < _minCoordinates(1)) {_minCoordinates(1) = min(1);}
            if (min(2) < _minCoordinates(2)) {_minCoordinates(2) = min(2);}

            if (max(0) > _maxCoordinates(0)) {_maxCoordinates(0) = max(0);}
            if (max(1) > _maxCoordinates(1)) {_maxCoordinates(1) = max(1);}
            if (max(2) > _maxCoordinates(2)) {_maxCoordinates(2) = max(2);}

            return true;
        }
    } else {
        PS_LOG->addMessage(LogInterface::info, LogInterface::reader, tr("Impossible d'ajouter un reader sans BoundingBox à une CT_DataSourceGeo"));
    }
    return false;
}

QList<const CT_AbstractReader *> CT_DataSourceGeo::getReadersIntersecting(const CT_Shape2DData &data)
{
    Eigen::Vector3d min, max;
    data.getBoundingBox(min, max);

    QList<const CT_AbstractReader *> list;

    if (min(0) > _maxCoordinates(0)) {return list;}
    if (min(1) > _maxCoordinates(1)) {return list;}
    if (max(0) < _minCoordinates(0)) {return list;}
    if (max(1) < _minCoordinates(1)) {return list;}

    QMapIterator<int, CT_AbstractReader*> it(_readers);

    while (it.hasNext())
    {
        it.next();
        CT_AbstractReader* reader = it.value();

        Eigen::Vector3d minR, maxR;
        reader->getHeader()->getBoundingBox(min, max);

        bool intersection = true;

        if (min(0) > maxR(0)) {intersection = false;}
        if (min(1) > maxR(1)) {intersection = false;}
        if (max(0) < minR(0)) {intersection = false;}
        if (max(1) < minR(1)) {intersection = false;}

        if (intersection) {list.append(it.value());}
    }

    return list;
}

bool CT_DataSourceGeo::intersects(const CT_Shape2DData &data)
{
    Eigen::Vector3d min, max;
    data.getBoundingBox(min, max);
    if (min(0) > _maxCoordinates(0)) {return false;}
    if (min(1) > _maxCoordinates(1)) {return false;}
    if (max(0) < _minCoordinates(0)) {return false;}
    if (max(1) < _minCoordinates(1)) {return false;}

    return true;
}

CT_AbstractItemDrawable *CT_DataSourceGeo::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_DataSourceGeo((const CT_OutAbstractSingularItemModel*)model, result, _readerPrototype->copy());
}

