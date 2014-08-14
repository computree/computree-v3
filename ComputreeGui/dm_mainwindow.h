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

#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include "view/MainView/gmainprogressdialog.h"

#include "cdm_scriptmanagerabstract.h"
#include "cdm_pluginmanager.h"
#include "cdm_stdiolistener.h"
#include "dm_actionsmanager.h"

class DM_MainWindow : public GuiContextInterface
{
    Q_INTERFACES(GuiContextInterface)

public:
    DM_MainWindow();
    virtual ~DM_MainWindow();

    virtual GMainProgressDialog* createWaitProgressDialog() = 0;

    CDM_PluginManager* getPluginManager() const;
    CDM_ScriptManagerAbstract* getScriptManager() const;
    CDM_StepManager* getStepManager() const;
    ActionsManagerInterface* actionsManager() const;
    DM_ActionsManager* getActionsManager() const;

private:

    CDM_StepManager             *_stepManager;
    CDM_PluginManager           *_pluginManager;
    CDM_ScriptManagerAbstract   *_scriptManager;
    CDM_StdIOListener           *_ioListener;
    CDM_StdIOParser             *_ioParser;
    DM_ActionsManager           m_actionsManager;
};

#endif // DM_MAINWINDOW_H
