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


#include "dm_pointofviewmanager.h"

#include <QFile>

QList<DM_PointOfViewManager*> DM_PointOfViewManager::_managerToSync = QList<DM_PointOfViewManager*>();

DM_PointOfViewManager::DM_PointOfViewManager()
{
    _defaultFilePath = "";
    _isSync = false;

    _managerToSync.append(this);
}

DM_PointOfViewManager::~DM_PointOfViewManager()
{
    saveDefault();
    _managerToSync.removeOne(this);
}

QString DM_PointOfViewManager::defaultFilePath() const
{
    if(_defaultFilePath.isEmpty())
    {
        return ".";
    }

    return _defaultFilePath;
}

QStringList DM_PointOfViewManager::extensionOfFile() const
{
    return QStringList() << ".ctpof";
}

bool DM_PointOfViewManager::addPointOfView(QString key, DM_PointOfView pof)
{
    if(internalAddPointOfView(key, pof))
    {
        syncAllManager();

        return true;
    }

    return false;
}

bool DM_PointOfViewManager::removePointOfView(QString key, DM_PointOfView pof)
{
    QList<DM_PointOfView> list = getPointOfViewList(key);
    int index;

    if((index = list.indexOf(pof)) >= 0)
    {
        list.removeAt(index);

        _map.insert(key, list);

        syncAllManager();

        return true;
    }

    return false;
}

void DM_PointOfViewManager::removeAll(QString key)
{
    _map.insert(key, QList<DM_PointOfView>());

    syncAllManager();
}

void DM_PointOfViewManager::removeAll()
{
    _map.clear();

    syncAllManager();
}

int DM_PointOfViewManager::numberPointOfViewAddedFromBeginning(QString key)
{
    return _mapNumberAdded.value(key, 0);
}

void DM_PointOfViewManager::resetNumberAdded(QString key)
{
    _mapNumberAdded.insert(key, 0);
}

QList<DM_PointOfView> DM_PointOfViewManager::getPointOfViewList(QString key)
{
    return _map.value(key, QList<DM_PointOfView>());
}

void DM_PointOfViewManager::loadDefault()
{
    CONFIG_FILE->beginGroup("PointOfViewManager");
    _defaultFilePath = CONFIG_FILE->value("defaultPoinOfViewFilePath", _defaultFilePath).toString();
    CONFIG_FILE->endGroup();

    if(QFile::exists(_defaultFilePath))
    {
        loadFromFile(_defaultFilePath);
    }
    else
    {
        _defaultFilePath = "";
        loadPointOfView(CONFIG_FILE);
    }
}

void DM_PointOfViewManager::saveDefault()
{
    QString path = "";

    if(QFile::exists(_defaultFilePath))
    {
        path = _defaultFilePath;

        saveToFile(_defaultFilePath);
    }
    else
    {
        savePointOfView(CONFIG_FILE);
    }

    CONFIG_FILE->beginGroup("PointOfViewManager");
    CONFIG_FILE->setValue("defaultPoinOfViewFilePath", path);
    CONFIG_FILE->endGroup();
    CONFIG_FILE->sync();
}

bool DM_PointOfViewManager::loadFromFile(QString filepath)
{
    if(filepath == ".")
    {
        loadPointOfView(CONFIG_FILE);
    }
    else if(QFile::exists(filepath))
    {
        _defaultFilePath = filepath;

        CDM_ConfigFile configFile(filepath, QSettings::IniFormat);

        loadPointOfView(&configFile);

        return true;
    }

    return false;
}

bool DM_PointOfViewManager::saveToFile(QString filepath)
{
    _defaultFilePath = filepath;

    CDM_ConfigFile configFile(filepath, QSettings::IniFormat);

    savePointOfView(&configFile);

    return true;
}

bool DM_PointOfViewManager::createNewFile(QString filepath)
{
    if(QFile::exists(filepath))
    {
        return false;
    }

    _defaultFilePath = filepath;
    _mapNumberAdded.clear();

    removeAll();

    QFile f(filepath);
    f.open(QFile::WriteOnly);
    f.close();

    return true;
}

bool DM_PointOfViewManager::internalAddPointOfView(QString key, DM_PointOfView pof)
{
    QList<DM_PointOfView> list = getPointOfViewList(key);

    if(!list.contains(pof))
    {
        list.insert(0, pof);

        _map.insert(key, list);
        _mapNumberAdded.insert(key, numberPointOfViewAddedFromBeginning(key)+1);

        return true;
    }

    return false;
}

void DM_PointOfViewManager::loadPointOfView(CDM_ConfigFile *configFile)
{
    _map.clear();

    configFile->beginGroup("PointOfViewManager");

    QStringList children = configFile->childGroups();
    QListIterator<QString> it(children);

    while(it.hasNext())
    {
        QString key = it.next();
        configFile->beginGroup(key);

        QString pKey = "P1";
        int n = 1;

        while(configFile->contains(pKey))
        {
            bool ok;

            DM_PointOfView pof = DM_PointOfView::loadFromString(configFile->value(pKey, "").toString(), ok);

            if(ok)
            {
                internalAddPointOfView(key, pof);
            }

            ++n;
            pKey = QString("P%1").arg(n);
        }

        _mapNumberAdded.insert(key, configFile->value("numberAdded", 0).toInt());

        configFile->endGroup();
    }

    configFile->endGroup();
}

void DM_PointOfViewManager::savePointOfView(CDM_ConfigFile *configFile)
{
    configFile->remove("PointOfViewManager");

    QMapIterator<QString, QList<DM_PointOfView> > it(_map);

    configFile->beginGroup("PointOfViewManager");

    while(it.hasNext())
    {
        it.next();

        QList<DM_PointOfView> list = it.value();
        QListIterator<DM_PointOfView> itL(list);
        int n = 1;

        QString key = it.key();
        configFile->beginGroup(key);
        itL.toBack();

        while(itL.hasPrevious())
        {
            QString val = itL.previous().toSaveString();
            configFile->setValue(QString("P%1").arg(n), val);
            ++n;
        }

        configFile->setValue("numberAdded", numberPointOfViewAddedFromBeginning(key));

        configFile->endGroup();
    }

    configFile->endGroup();
}

void DM_PointOfViewManager::syncAllManager()
{
    if(!_isSync)
    {
        saveDefault();

        QListIterator<DM_PointOfViewManager*> it(_managerToSync);

        while(it.hasNext())
        {
            DM_PointOfViewManager *manager = it.next();

            if(manager != this)
            {
                manager->syncFile(defaultFilePath());
            }
        }
    }
}

void DM_PointOfViewManager::syncFile(QString filepath)
{
    _isSync = true;

    if(filepath == defaultFilePath())
    {
        loadFromFile(filepath);
    }

    _isSync = false;
}
