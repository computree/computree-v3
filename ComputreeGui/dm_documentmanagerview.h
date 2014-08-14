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


#ifndef DM_DOCUMENTMANAGERVIEW_H
#define DM_DOCUMENTMANAGERVIEW_H

#include "dm_documentmanager.h"
#include "dm_documentview.h"

#include "dm_asynchroneprogress.h"

class Result;

class DM_DocumentManagerView : public DM_DocumentManager
{
public:
    DM_DocumentManagerView();

    virtual bool addAllItemDrawableOfResultToActiveDocument(Result &res, DM_AsynchroneProgress &progress) = 0;
    virtual bool addAllItemDrawableOfModelToDocument(Result &res, IItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress) = 0;
    virtual bool addAllItemDrawableOfResultToDocument(Result &res, DM_DocumentView *doc, DM_AsynchroneProgress &progress) = 0;

    virtual bool addAllItemDrawableOfListToActiveDocument(QList<ItemDrawable*> &itemList, DM_AsynchroneProgress &progress) = 0;
    virtual bool addAllItemDrawableOfListToDocument(QList<ItemDrawable*> &itemList, DM_DocumentView *doc, DM_AsynchroneProgress &progress) = 0;

    virtual bool removeAllItemDrawableOfResultFromDocuments(Result &res, DM_AsynchroneProgress &progress) = 0;
    virtual bool removeAllItemDrawableOfModelFromDocuments(IItemModel &model, DM_AsynchroneProgress &progress) = 0;
    virtual bool removeAllItemDrawableOfModelFromDocument(IItemModel &model, DM_DocumentView *doc, DM_AsynchroneProgress &progress) = 0;
    virtual bool removeAllItemDrawableOfListFromDocuments(QList<ItemDrawable*> &itemList, DM_AsynchroneProgress &progress) = 0;

    virtual DM_DocumentView* getActiveDocumentView() = 0;
    virtual DM_DocumentView* getDocumentView(int index) const = 0;
    virtual int nbDocumentView() const = 0;
};

#endif // DM_DOCUMENTMANAGERVIEW_H
