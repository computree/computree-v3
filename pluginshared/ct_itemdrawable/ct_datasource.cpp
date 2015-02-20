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

#include "ct_datasource.h"

CT_DEFAULT_IA_INIT(CT_DataSource)

CT_DataSource::CT_DataSource() : CT_AbstractSingularItemDrawable()
{
    _activeReader = -1;
    _lastReaderIndice = -1;
}

CT_DataSource::CT_DataSource(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model, result)
{
    _activeReader = -1;
    _lastReaderIndice = -1;
}

CT_DataSource::CT_DataSource(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName, result)
{
    _activeReader = -1;
    _lastReaderIndice = -1;
}

CT_DataSource::~CT_DataSource()
{
    _readers.clear();
}

QString CT_DataSource::getType() const
{
    return staticGetType();
}

QString CT_DataSource::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_DataSource";
}

bool CT_DataSource::addReader(CT_AbstractReader *reader)
{
    if (getNumberOfReader() > 0)
    {
        QSharedPointer<CT_AbstractReader> firstReader = _readers.first();

        if (reader->GetReaderName() != firstReader->GetReaderName())
        {
            PS_LOG->addMessage(LogInterface::info, LogInterface::reader, tr("Impossible d'ajouter des readers de classe différente"));
            return false;
        }
    }

    if (reader->isValid())
    {
        ++_lastReaderIndice;
        _readers.append(QSharedPointer<CT_AbstractReader>(reader));
    } else {
        PS_LOG->addMessage(LogInterface::info, LogInterface::reader, tr("Fichier (%1) non valide").arg(reader->filepath()));
        return false;
    }
    return true;
}

bool CT_DataSource::activateNextReader()
{
    ++_activeReader;
    if (_activeReader < 0 || _activeReader > _lastReaderIndice) {return false;}

    return true;
}

const QSharedPointer<CT_AbstractReader> CT_DataSource::getActiveReader() const
{
    if (_activeReader < 0 || _activeReader > _lastReaderIndice) {return QSharedPointer<CT_AbstractReader>();}

    return _readers.at(_activeReader);
}


const QSharedPointer<CT_AbstractReader> CT_DataSource::getNextReader()
{
    ++_activeReader;
    if (_activeReader < 0 || _activeReader > _lastReaderIndice) {return QSharedPointer<CT_AbstractReader>();}

    return _readers.at(_activeReader);
}


int CT_DataSource::getNumberOfReader() const
{
    return _readers.size();
}

void CT_DataSource::init(int n) const
{
    _activeReader = n - 1;
}

CT_AbstractItemDrawable *CT_DataSource::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_DataSource *cpy = new CT_DataSource((const CT_OutAbstractSingularItemModel*)model, result);
    cpy->_readers.append(_readers);
    cpy->_lastReaderIndice = _lastReaderIndice;

    return cpy;
}
