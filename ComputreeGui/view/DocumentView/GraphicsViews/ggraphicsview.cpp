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

#include "ggraphicsview.h"

#include "gdocumentview.h"
#include "dm_guimanager.h"

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

GGraphicsView::GGraphicsView() : DM_GraphicsView()
{
    m_actionsHandler = new DM_ActionsHandler(*GUI_MANAGER->getActionsManager());
}

GGraphicsView::~GGraphicsView()
{
    delete m_actionsHandler;
}

void GGraphicsView::setCurrentAction(CT_AbstractActionForGraphicsView *action)
{
    if(action != NULL)
    {
        action->setDocument(&getDocumentView());
        action->setGraphicsView(this);
    }

    m_actionsHandler->setCurrentAction(action);
}

void GGraphicsView::setDefaultAction(CT_AbstractActionForGraphicsView *action)
{
    if(action != NULL)
    {
        action->setDocument(&getDocumentView());
        action->setGraphicsView(this);
    }

    m_actionsHandler->setDefaultAction(action);
}

DM_ActionsHandler* GGraphicsView::actionsHandler() const
{
    return m_actionsHandler;
}
