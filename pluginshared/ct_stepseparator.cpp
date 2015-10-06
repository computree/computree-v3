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

#include "ct_stepseparator.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_StepSeparator::CT_StepSeparator(const QString &customMenu)
{
    m_levelCustom = customMenu;
    m_levelOperation = LO_None;
    m_levelElement = LE_Custom;
}

CT_StepSeparator::CT_StepSeparator(CT_StepSeparator::LevelOperationType levelOperation, const QString &levelElementCustom)
{
    m_levelOperation = levelOperation;
    m_levelCustom =  levelElementCustom;
    m_levelElement = LE_Custom;
}

CT_StepSeparator::CT_StepSeparator(CT_StepSeparator::LevelOperationType levelOperation, LevelElementsType levelElement)
{
    m_levelOperation = levelOperation;
    m_levelElement = levelElement;
}

CT_StepSeparator::CT_StepSeparator(CT_StepSeparator::LevelOperationType levelOperation, LevelElementsType levelElement, const QString &levelElementCustomTitle)
{
    m_levelOperation = levelOperation;
    m_levelElement = levelElement;
    m_levelCustom =  levelElementCustomTitle;
}

CT_StepSeparator::~CT_StepSeparator()
{
    qDeleteAll(_stepList.begin(), _stepList.end());
}

bool CT_StepSeparator::addStep(CT_VirtualAbstractStep *step)
{
    if(step != NULL)
        addStep(*step);

    return (step != NULL);
}

CT_StepSeparator::LevelOperationType CT_StepSeparator::getLevelOperation() const
{
    return m_levelOperation;
}

CT_StepSeparator::LevelElementsType CT_StepSeparator::getLevelElement() const
{
    return m_levelElement;
}

QString CT_StepSeparator::getLevelOperationToString() const
{
    switch(m_levelOperation) {
    default: return "";
    }

    return "";
}

QString CT_StepSeparator::getLevelElementToString() const
{
    switch(m_levelElement) {
    default: return "";
    }

    return "";
}

QString CT_StepSeparator::getTitle() const
{
    return m_levelCustom;
}

QList<CT_VirtualAbstractStep*> CT_StepSeparator::getStepList() const
{
    return _stepList;
}

void CT_StepSeparator::addStep(CT_VirtualAbstractStep &step)
{
    _stepList.append(&step);
}
