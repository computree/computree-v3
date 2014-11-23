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

#include "ct_abstractdatasource.h"


CT_AbstractDataSource::CT_AbstractDataSource() : CT_AbstractSingularItemDrawable()
{
    _activeReader = -1;
    _lastReaderIndice = -1;
}

CT_AbstractDataSource::CT_AbstractDataSource(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{

}

CT_AbstractDataSource::CT_AbstractDataSource(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{

}

CT_AbstractDataSource::~CT_AbstractDataSource()
{
}

QString CT_AbstractDataSource::getType() const
{
    return staticGetType();
}

QString CT_AbstractDataSource::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_AbstractDataSource";
}

void CT_AbstractDataSource::addReader(CT_AbstractReader *reader)
{
    _readers.insert(++_lastReaderIndice, reader);
}

const CT_AbstractReader *CT_AbstractDataSource::getActiveReader() const
{
    return _readers.value(_activeReader, NULL);
}

bool CT_AbstractDataSource::activateNextReader()
{
    return (_readers.value(++_activeReader) != NULL);
}

int CT_AbstractDataSource::getNumberOfReader() const
{
    return _readers.size();
}

void CT_AbstractDataSource::init()
{
    _activeReader = 1;
}

QList<const CT_AbstractReader *> CT_AbstractDataSource::getReadersIntersecting(const CT_Shape2DData &data)
{
    QList<const CT_AbstractReader *> list;

    QMapIterator<int, CT_AbstractReader*> it(_readers);

    while (it.hasNext())
    {
        it.next();
        //if (it.value()->intersect(data))
        {
            list.append(it.value());
        }
    }

    return list;
}
