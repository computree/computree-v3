/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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

#include "ct_abstractresult.h"

#include "ct_resultinitializedata.h"

#include "ct_virtualabstractstep.h"
#include "ct_abstractitemdrawable.h"

#include "ct_outabstractresultmodel.h"

CT_AbstractResult::CT_AbstractResult()
{
    _dataInit = NULL;
    _parentStep = NULL;
    _numberOfItemVisible = 0;
    _clearFromMemoryProgress = 100;
    _autoDelete = true;
    _model = NULL;
    _mutexBusy = new QMutex(QMutex::Recursive);
}

CT_AbstractResult::CT_AbstractResult(const CT_OutAbstractResultModel *model,
                                     CT_ResultInitializeData &data,
                                     CT_VirtualAbstractStep &parentStep)
{
    _dataInit = &data;
    _parentStep = &parentStep;
    _numberOfItemVisible = 0;
    _clearFromMemoryProgress = 100;
    _autoDelete = true;
    _model = (CT_OutAbstractResultModel*)model;
    _mutexBusy = new QMutex(QMutex::Recursive);
}

CT_AbstractResult::~CT_AbstractResult()
{
    delete _dataInit;
    delete _mutexBusy;
}

void CT_AbstractResult::setBusy(bool busy)
{
    if(busy)
    {
        _mutexBusy->lock();

        emit busyStateChanged(true);
    }
    else
    {
        _mutexBusy->unlock();

        emit busyStateChanged(false);
    }
}

bool CT_AbstractResult::isBusy() const
{
    bool l = _mutexBusy->tryLock();

    if(l)
        _mutexBusy->unlock();

    return !l;
}

CT_OutAbstractResultModel* CT_AbstractResult::model() const
{
    return _model;
}

CT_VirtualAbstractStep* CT_AbstractResult::parentStep() const
{
    return _parentStep;
}

bool CT_AbstractResult::isVisible() const
{
    return (_numberOfItemVisible > 0);
}

QString CT_AbstractResult::getName() const
{
    if(_name.isEmpty())
        return getType();

    return _name;
}

QString CT_AbstractResult::getToolTip() const
{
    return getName();
}

QString CT_AbstractResult::getType() const
{
    return staticGetType();
}

QString CT_AbstractResult::staticGetType()
{
    return "Result/CT_AbstractResult";
}

void CT_AbstractResult::setName(const QString &name)
{
    _name = name.simplified();
}

int CT_AbstractResult::getClearFromMemoryProgress() const
{
    return (100-_clearFromMemoryProgress);
}

bool CT_AbstractResult::isClearedFromMemory() const
{
    return (_clearFromMemoryProgress > 0);
}

void CT_AbstractResult::clearFromMemory()
{
    setClearFromMemoryProgress(1);

    clearFromMemoryProtected();

    setClearFromMemoryProgress(100);
}

///////////// PROTECTED ///////////////

void CT_AbstractResult::setComplete()
{
    _clearFromMemoryProgress = 0;
}

void CT_AbstractResult::incrementItemVisible()
{
    ++_numberOfItemVisible;

    if(_numberOfItemVisible == 1)
        emit visibilityChange(isVisible());
}

void CT_AbstractResult::decrementItemVisible()
{
    --_numberOfItemVisible;

    if(_numberOfItemVisible == 0)
        emit visibilityChange(isVisible());
}

void CT_AbstractResult::setAutoDelete(bool autoDelete)
{
    _autoDelete = autoDelete;
}

bool CT_AbstractResult::isAutoDelete() const
{
    return _autoDelete;
}

void CT_AbstractResult::setClearFromMemoryProgress(int progress)
{
    if((progress > 0)
        && (progress != _clearFromMemoryProgress))
    {
        _clearFromMemoryProgress = progress;

        emit clearFromMemoryInProgress(100-progress);
    }
}
