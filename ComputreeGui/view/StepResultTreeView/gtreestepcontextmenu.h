/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef GTREESTEPCONTEXTMENU_H
#define GTREESTEPCONTEXTMENU_H

#include <QMenu>
#include "myqaction.h"
#include "view/StepResultTreeView/myqmenu.h"

class CT_VirtualAbstractStep;
class CT_AbstractStepSerializable;
class CDM_StepManager;
class CT_StepSeparator;
class CT_StepCanBeAddedFirstSeparator;

/**
  * Classe reprsentant un menu pour l'ajout/suppression/autres actions pour les tapes
  */
class GTreeStepContextMenu : public QMenu
{
    Q_OBJECT

public:
    GTreeStepContextMenu(CDM_StepManager &stepManager, QWidget *parent = 0);

    /*!
     *  \brief Dfinir l'tape qui subira l'action
     */
    void setSelectedStep(CT_VirtualAbstractStep *step);

private:

    CDM_StepManager *_stepManager;
    CT_VirtualAbstractStep            *_selectedStep;

    CT_VirtualAbstractStep* selectedStep();

signals:

    void executeSelectedStep(CT_VirtualAbstractStep *selectedStep);
    void executeModifySelectedStep(CT_VirtualAbstractStep *selectedStep);
    void configureInputResultOfSelectedStep(CT_VirtualAbstractStep *selectedStep);
    void configureSelectedStep(CT_VirtualAbstractStep *selectedStep);
    void deleteSelectedStep(CT_VirtualAbstractStep *selectedStep);
    void loadResultOfSelectedStep(CT_AbstractStepSerializable *selectedStep);
    void locateSelectedStepInMenu(CT_VirtualAbstractStep *selectedStep);
    void expand();
    void expandAll();
    void collapse();
    void collapseAll();

private slots:

    void reload();

    void executeStepRequired();
    void executeModifyStepRequired();
    void configureInputResultOfStepRequired();
    void configureStepRequired();
    void showStepInformations();
    void deleteStepRequired();
    void loadResultRequired();
    void locateStepInMenu();
};

#endif // TREESTEPCONTEXTMENU_H
