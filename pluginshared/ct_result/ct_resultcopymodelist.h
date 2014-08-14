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

#ifndef CT_RESULTCOPYMODELIST_H
#define CT_RESULTCOPYMODELIST_H

#include "pluginShared_global.h"

#include <QList>

class PLUGINSHAREDSHARED_EXPORT CT_ResultCopyModeList
{
public:

    enum CopyMode
    {
        DontCopyItemDrawable = 0,
        CopyItemDrawableCompletely = 1,
        CopyItemDrawableReference = 2
    };

    CT_ResultCopyModeList();

    bool isEmpty() const;
    int size() const;
    const CT_ResultCopyModeList::CopyMode& at(int i) const;
    CT_ResultCopyModeList::CopyMode takeFirst();

    bool append(CT_ResultCopyModeList::CopyMode copyMode);
    CT_ResultCopyModeList& operator<<(const CT_ResultCopyModeList::CopyMode &value);

    bool operator==(const CT_ResultCopyModeList &other) const;
    bool operator!=(const CT_ResultCopyModeList &other) const;

private:

    QList<CT_ResultCopyModeList::CopyMode> _copyModeList;
};

#endif // CT_RESULTCOPYMODELIST_H
