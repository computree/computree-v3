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


#include "dm_asynchroneprogress.h"

DM_AsynchroneProgress::DM_AsynchroneProgress()
{
    _progress = 0;
}

void DM_AsynchroneProgress::setProgress(float progress)
{
    setProgress((int)progress);
}

void DM_AsynchroneProgress::setProgress(int progress)
{
    if(progress != _progress)
    {
        _progress = progress;

        emit inProgress(_progress);
    }
}

void DM_AsynchroneProgress::setText(QString text)
{
    if(_text != text)
    {
        _text = text;

        emit textChanged(_text);
    }
}

void DM_AsynchroneProgress::finish()
{
    emit inProgress(100);
    emit finished();
}
