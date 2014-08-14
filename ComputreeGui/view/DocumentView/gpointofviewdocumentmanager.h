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


#ifndef GPOINTOFVIEWDOCUMENTMANAGER_H
#define GPOINTOFVIEWDOCUMENTMANAGER_H

#include <QMenu>
#include "tools/graphicsview/dm_pointofviewmanager.h"

class GPointOfViewDocumentManager : public QMenu
{
    Q_OBJECT
public:
    explicit GPointOfViewDocumentManager(DM_PointOfViewManager &pofManager,
                                         QString key,
                                         QWidget *parent = 0);

private:

    QString                 _key;
    DM_PointOfView          _pofToSet;
    DM_PointOfViewManager   *_pofManager;

    QString createFileExtensionAvailable();

signals:

    void addActualPointOfView();
    void setPointOfView(DM_PointOfView *pof);

public slots:

    void reload();
    void openFile();
    void saveToFile();
    void createNew();

private slots:

    void actionTriggered(QAction *action);
    void removeAllPointOfView();
};

#endif // GPOINTOFVIEWDOCUMENTMANAGER_H
