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



#include "ct_steploadfileseparator.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/tools/menu/ct_stepsmenu.h"
#include "ct_step/tools/menu/ct_menulevel.h"

CT_StepLoadFileSeparator::CT_StepLoadFileSeparator(const QString &typeOfFile)
{
    _typeOfFile = typeOfFile;
}

CT_StepLoadFileSeparator::~CT_StepLoadFileSeparator()
{
}

bool CT_StepLoadFileSeparator::addStep(CT_AbstractStepLoadFile *step)
{
    if(step != NULL) {
        CT_MenuLevel *level = m_menuOfSteps->getOrCreateRootLevel(CT_StepsMenu::LO_Load);
        level = level->getOrCreateLevel(_typeOfFile, level);
        level->addStepToCollectionOrDeleteIt(step);
    }

    return (step != NULL);
}

QString CT_StepLoadFileSeparator::typeOfFile() const
{
    return _typeOfFile;
}

QList<CT_AbstractStepLoadFile*> CT_StepLoadFileSeparator::getStepList() const
{
    return QList<CT_AbstractStepLoadFile*>();
}
