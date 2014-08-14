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


#include "dm_itemdrawablemanageroptions.h"

#include <QFile>
#include <QFileInfo>

DM_ItemDrawableManagerOptions::DM_ItemDrawableManagerOptions()
{
    _nLevelFiltering = 10;
    _filterString = "";
    _showOnlySelected = false;

    _automaticColorList.append(QColor(255,255,200)); // Jaune Clair
    _automaticColorList.append(QColor(255,200,255)); // Magenta Clair
    _automaticColorList.append(QColor(200,255,255)); // Cyan Clair
    _automaticColorList.append(QColor(200,200,255)); // Mauve Clair
    _automaticColorList.append(QColor(200,255,200)); // Vert Clair
    _automaticColorList.append(QColor(255,200,200)); // Rouge Clair
    _automaticColorList.append(QColor(255,200,150)); // Orange clair
    _automaticColorList.append(QColor(150,200,255)); // Bleu Clair
    _automaticColorList.append(QColor(200,255,150)); // Vert-Jaune Clair
    _automaticColorList.append(QColor(150,255,200)); // Turquoise Clair
    _automaticColorList.append(QColor(255,150,200)); // Rose Clair
    _automaticColorList.append(QColor(200,150,255)); // Violet Clair
    _automaticColorList.append(QColor(255,255,0  )); // Jaune
    _automaticColorList.append(QColor(255,0  ,255)); // Magenta
    _automaticColorList.append(QColor(0  ,255,255)); // Cyan
    _automaticColorList.append(QColor(0  ,0  ,255)); // Mauve
    _automaticColorList.append(QColor(0  ,255,0  )); // Vert
    _automaticColorList.append(QColor(255,150,0  )); // Orange
    _automaticColorList.append(QColor(0  ,150,255)); // Bleu
    _automaticColorList.append(QColor(150,255,0  )); // Vert-Jaune
    _automaticColorList.append(QColor(0  ,255,150)); // Turquoise
    _automaticColorList.append(QColor(255,0  ,150)); // Rose
    _automaticColorList.append(QColor(150,0  ,255)); // Violet


    _indexColorList = 0;
}

DM_ItemDrawableManagerOptions::DM_ItemDrawableManagerOptions(const DM_ItemDrawableManagerOptions &options)
{
    _nLevelFiltering = options._nLevelFiltering;
    _filterString = options._filterString;
    _showOnlySelected = options._showOnlySelected;
    _automaticColorList = options._automaticColorList;
    _indexColorList = options._indexColorList;
}

void DM_ItemDrawableManagerOptions::filterString(QString text)
{
    _filterString = text;
}

void DM_ItemDrawableManagerOptions::filterOnNLevel(int nLevel)
{
    _nLevelFiltering = nLevel;
}

void DM_ItemDrawableManagerOptions::setShowOnlySelected(bool enable)
{
    _showOnlySelected = enable;
}

void DM_ItemDrawableManagerOptions::addColorToList(QColor color)
{
    if(!_automaticColorList.contains(color))
    {
        _automaticColorList.append(color);
    }
}

void DM_ItemDrawableManagerOptions::setColorList(QList<QColor> list)
{
    _automaticColorList = list;
}

bool DM_ItemDrawableManagerOptions::loadColorListFromFile(QString filePath)
{
    if(QFile::exists(filePath))
    {
        QFileInfo info(filePath);

        if(getColorListFileExtension().contains("." + info.suffix()))
        {
            CDM_ConfigFile settings(filePath, QSettings::IniFormat);

            return loadColorList(&settings);
        }
    }

    return false;
}

bool DM_ItemDrawableManagerOptions::saveColorListToFile(QString filePath)
{
    CDM_ConfigFile settings(filePath, QSettings::IniFormat);

    return saveColorList(&settings);
}

QList<QColor> DM_ItemDrawableManagerOptions::getColorList()
{
    return _automaticColorList;
}

QColor DM_ItemDrawableManagerOptions::getNextColor()
{
    int size = _automaticColorList.size();

    if(_indexColorList < size)
    {
        QColor color = _automaticColorList.at(_indexColorList);
        ++_indexColorList;

        if(_indexColorList >= size)
        {
            _indexColorList = 0;
        }

        return color;
    }

    return Qt::white;
}

QList<QString> DM_ItemDrawableManagerOptions::getColorListFileExtension()
{
    QList<QString> list;

    list << ".col";

    return list;
}

void DM_ItemDrawableManagerOptions::load()
{
    loadColorList(CONFIG_FILE);
}

bool DM_ItemDrawableManagerOptions::save()
{
    return saveColorList(CONFIG_FILE);
}

bool DM_ItemDrawableManagerOptions::loadColorList(CDM_ConfigFile *settings)
{
    if(!settings->childGroups().contains("ItemDrawableManagerOptions"))
    {
        return false;
    }

    QString key = "couleur1";
    int i = 1;

    _automaticColorList.clear();

    settings->beginGroup("ItemDrawableManagerOptions");

    while(settings->contains(key))
    {
        _automaticColorList.append(settings->colorValue(key, QColor(Qt::white)));

        ++i;
        key = QString("couleur%1").arg(i);
    }

    settings->endGroup();

    return true;
}


bool DM_ItemDrawableManagerOptions::saveColorList(CDM_ConfigFile *settings)
{
    settings->beginGroup("ItemDrawableManagerOptions");

    QString key = "couleur1";
    int i = 1;

    while(settings->contains(key))
    {
        settings->remove(key);

        ++i;
        key = QString("couleur%1").arg(i);
    }

    i = 1;

    QListIterator<QColor> it(_automaticColorList);

    while(it.hasNext())
    {
        settings->setColorValue(QString("couleur%1").arg(i), it.next());
        ++i;
    }

    settings->endGroup();

    return true;
}
