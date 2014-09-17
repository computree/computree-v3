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


#include "dm_graphicsview.h"

DM_GraphicsView::DM_GraphicsView()
{
    _options = new DM_GraphicsViewOptions();
    _options->load();
    _document = NULL;
}

DM_GraphicsView::~DM_GraphicsView()
{
    delete _options;
}

void DM_GraphicsView::setDocumentView(const DM_DocumentView *doc)
{
    _document = (DM_DocumentView*)doc;
}

DM_DocumentView& DM_GraphicsView::getDocumentView() const
{
    return *_document;
}

DocumentInterface* DM_GraphicsView::document() const
{
    return _document;
}

void DM_GraphicsView::setOptions(const DM_GraphicsViewOptions &newOptions)
{
    _options->updateFromOtherOptions(newOptions);
}

GraphicsViewOptionsInterface& DM_GraphicsView::getOptions()
{
    return *_options;
}

const DM_GraphicsViewOptions& DM_GraphicsView::getOptions() const
{
    return *_options;
}
