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

#ifndef GGRAPHICSVIEW_H
#define GGRAPHICSVIEW_H

#include <QWidget>

#include "dm_graphicsview.h"
#include "dm_actionshandler.h"

class GDocumentView;

class GGraphicsView : public DM_GraphicsView
{
public:
    GGraphicsView();
    ~GGraphicsView();

    void setCurrentAction(ActionForGraphicsViewInterface *action);
    void setDefaultAction(ActionForGraphicsViewInterface *action);

    virtual QWidget* getViewWidget() const = 0;

    DM_ActionsHandler* actionsHandler() const;

private:
    DM_ActionsHandler               *m_actionsHandler;
};

#endif // GGRAPHICSVIEW_H
