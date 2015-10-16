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

#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_step/tools/menu/ct_stepsmenu.h"
#include "ct_step/tools/menu/ct_menulevel.h"

CT_StepCanBeAddedFirstSeparator::CT_StepCanBeAddedFirstSeparator(const QString &title)
{
    _title = title;
}

CT_StepCanBeAddedFirstSeparator::~CT_StepCanBeAddedFirstSeparator()
{
}

bool CT_StepCanBeAddedFirstSeparator::addStep(CT_AbstractStepCanBeAddedFirst *step)
{
    if(step != NULL) {
        CT_MenuLevel *level = m_menuOfSteps->getOrCreateRootLevel(CT_StepsMenu::LO_Other);
        level = level->getOrCreateLevel(_title, level);
        level->addStepToCollectionOrDeleteIt(step);
    }

    return (step != NULL);
}

QString CT_StepCanBeAddedFirstSeparator::getTitle() const
{
    return _title;
}

QList<CT_AbstractStepCanBeAddedFirst*> CT_StepCanBeAddedFirstSeparator::getStepList() const
{
    return QList<CT_AbstractStepCanBeAddedFirst*>();
}
