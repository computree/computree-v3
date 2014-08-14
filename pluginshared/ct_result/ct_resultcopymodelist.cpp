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

#include "ct_resultcopymodelist.h"

CT_ResultCopyModeList::CT_ResultCopyModeList()
{
}

bool CT_ResultCopyModeList::isEmpty() const
{
    return _copyModeList.isEmpty();
}

int CT_ResultCopyModeList::size() const
{
    return _copyModeList.size();
}

const CT_ResultCopyModeList::CopyMode& CT_ResultCopyModeList::at(int i) const
{
    return _copyModeList.at(i);
}

CT_ResultCopyModeList::CopyMode CT_ResultCopyModeList::takeFirst()
{
    return _copyModeList.takeFirst();
}

bool CT_ResultCopyModeList::append(CT_ResultCopyModeList::CopyMode copyMode)
{
    if(copyMode == CT_ResultCopyModeList::CopyItemDrawableCompletely)
    {
        if(!_copyModeList.isEmpty()
                && (_copyModeList.last() != CT_ResultCopyModeList::CopyItemDrawableCompletely))
        {
            qFatal("Erreur : Vous ne pouvez pas ajouter le mode \"CopyItemDrawableCompletely\" dans une liste de mode ou le dernier mode est différent de \"CopyItemDrawableCompletely\".");

            return false;
        }
    }
    else if(copyMode == CT_ResultCopyModeList::DontCopyItemDrawable)
    {
        if(!_copyModeList.isEmpty())
        {
            qFatal("Erreur : Vous ne pouvez pas ajouter le mode \"DontCopyItemDrawable\" dans une liste non vide.");

            return false;
        }
    }
    else
    {
        if(!_copyModeList.isEmpty()
                && (_copyModeList.last() == CT_ResultCopyModeList::DontCopyItemDrawable))
        {
            qFatal("Erreur : Vous ne pouvez plus ajouter de mode après le mode \"DontCopyItemDrawable\".");

            return false;
        }
    }

    _copyModeList.append(copyMode);

    return true;
}

CT_ResultCopyModeList& CT_ResultCopyModeList::operator<<(const CT_ResultCopyModeList::CopyMode &value)
{
    append(value);

    return *this;
}

bool CT_ResultCopyModeList::operator==(const CT_ResultCopyModeList &other) const
{
    if(size() != other.size())
    {
        return false;
    }

    QListIterator<CT_ResultCopyModeList::CopyMode> it(_copyModeList);
    QListIterator<CT_ResultCopyModeList::CopyMode> it2(other._copyModeList);

    while(it.hasNext() && it2.hasNext())
    {
        if(it.hasNext() != it2.hasNext())
        {
            return false;
        }
    }

    return true;
}

bool CT_ResultCopyModeList::operator!=(const CT_ResultCopyModeList &other) const
{
    if(size() != other.size())
    {
        return true;
    }

    QListIterator<CT_ResultCopyModeList::CopyMode> it(_copyModeList);
    QListIterator<CT_ResultCopyModeList::CopyMode> it2(other._copyModeList);

    while(it.hasNext() && it2.hasNext())
    {
        if(it.hasNext() != it2.hasNext())
        {
            return true;
        }
    }

    return false;
}
