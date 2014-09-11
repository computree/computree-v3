/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GGUIMANAGER_H
#define GGUIMANAGER_H

#include "dm_guimanager.h"
#include "gmainwindow.h"

#include "cdm_internationalization.h"

class GGuiManager : public DM_GuiManager
{
public:
    GGuiManager(CDM_Internationalization *language);
    ~GGuiManager();

    // GuiManagerInterface
    DocumentManagerInterface* documentManager() const;

    void initUi();

    DM_DocumentManagerView* getDocumentManagerView() const;
    CDM_StepManager* getStepManager() const;
    DM_ActionsManager* getActionsManager() const;

protected:

    DM_MainWindow* getMainWindow() const;
    DM_MultipleItemDrawableModelManager* getItemDrawableModelManager() const;

private:

    GMainWindow                 *_mainWindow;
    CDM_Internationalization    *m_language;
};

#endif // GGUIMANAGER_H
