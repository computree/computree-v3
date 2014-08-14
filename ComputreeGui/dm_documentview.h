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


#ifndef DM_DOCUMENTVIEW_H
#define DM_DOCUMENTVIEW_H

#include "dm_document.h"

class DM_DocumentManagerView;

class DM_DocumentView : public DM_Document
{
public:
    DM_DocumentView(DM_DocumentManagerView &manager, QString title);

    virtual QString getType() const = 0;

    virtual QByteArray saveGeometry() const = 0;
    virtual bool restoreGeometry(const QByteArray &geometry) = 0;

    virtual void addActionOptions(ActionOptionsInterface *options) = 0;
    virtual void removeActionOptions(ActionOptionsInterface *options) = 0;

    virtual bool isVisible() const = 0;
};

#endif // DM_DOCUMENTVIEW_H
