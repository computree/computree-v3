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


#ifndef DM_CONTEXT_H
#define DM_CONTEXT_H

#include <QObject>

/**
  * Classe qui reprsente une situation. Lorsqu'on demande une action
  * au GUI_MANAGER certaine d'entre elles sont asynchrone (thread) et
  * lorsqu'elle est termin elle appelle la mthode setActionFinished du
  * context. Un signal est mit et on peut rcuprer ce signal pour savoir
  * dans quelle contexte on se trouvait.
  */
class DM_Context : public QObject
{
    Q_OBJECT

public:
    DM_Context(QObject *parent = 0) : QObject(parent)
    {
        _finished = false;
    }

public slots:
    void setActionFinished()
    {
        if(!_finished)
        {
            _finished = true;

            emit actionFinished(this);
        }
    }

private:

    bool _finished;

signals:

    void actionFinished(DM_Context *context);
};

#endif // DM_CONTEXT_H
