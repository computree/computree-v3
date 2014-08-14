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


#ifndef DM_POINTOFVIEWMANAGER_H
#define DM_POINTOFVIEWMANAGER_H

#include "tools/graphicsview/dm_pointofview.h"
#include "cdm_configfile.h"

#include <QMap>

class DM_PointOfViewManager
{
public:
    DM_PointOfViewManager();
    ~DM_PointOfViewManager();

    QString defaultFilePath() const;
    QStringList extensionOfFile() const;

    bool addPointOfView(QString key, DM_PointOfView pof);
    bool removePointOfView(QString key, DM_PointOfView pof);
    void removeAll(QString key);
    void removeAll();

    int numberPointOfViewAddedFromBeginning(QString key);
    void resetNumberAdded(QString key);

    QList<DM_PointOfView> getPointOfViewList(QString key);

    void loadDefault();
    void saveDefault();

    bool loadFromFile(QString filepath);
    bool saveToFile(QString filepath);

    bool createNewFile(QString filepath);

private:

    QMap<QString, QList<DM_PointOfView> >   _map;
    QMap<QString, int >                     _mapNumberAdded;
    QString                                 _defaultFilePath;

    static QList<DM_PointOfViewManager*>    _managerToSync;
    bool                                    _isSync;

    bool internalAddPointOfView(QString key, DM_PointOfView pof);

    void loadPointOfView(CDM_ConfigFile *configFile);
    void savePointOfView(CDM_ConfigFile *configFile);

    void syncAllManager();
    void syncFile(QString filepath);
};

#endif // DM_POINTOFVIEWMANAGER_H
