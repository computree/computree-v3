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


#include "gguimanager.h"

GGuiManager::GGuiManager(CDM_Internationalization *languageManager)
{
    _uniqueInstance = this;
    _mainWindow = NULL;
    m_languageManager = languageManager;
}

GGuiManager::~GGuiManager()
{
    m_languageManager->saveConfiguration();
    delete _mainWindow;
}

DocumentManagerInterface* GGuiManager::documentManager() const
{
    return getDocumentManagerView();
}

DM_MainWindow* GGuiManager::getMainWindow() const
{
    return _mainWindow;
}

DM_DocumentManagerView* GGuiManager::getDocumentManagerView() const
{
    return _mainWindow->getDocumentManagerView();
}

CDM_StepManager* GGuiManager::getStepManager() const
{
    return _mainWindow->getStepManager();
}

DM_ActionsManager* GGuiManager::getActionsManager() const
{
    return (DM_ActionsManager*)_mainWindow->actionsManager();
}

CDM_Internationalization *GGuiManager::getLanguageManager() const
{
    return m_languageManager;
}

DM_ItemDrawableConfigurationManagerView* GGuiManager::getItemDrawableConfigurationManagerView() const
{
    return _mainWindow->getItemDrawableConfigurationManager();
}

DM_MultipleItemDrawableModelManager* GGuiManager::getItemDrawableModelManager() const
{
    return _mainWindow->getItemDrawableModelManager();
}

void GGuiManager::initUi()
{
    _mainWindow = new GMainWindow();
    _mainWindow->init();
    _mainWindow->getPluginManager()->setGuiManager(this);
    _mainWindow->showMaximized();

    _mainWindow->loadConfiguration();

    init();
}
