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

#ifndef DM_ITEMDRAWABLEMANAGEROPTIONS_H
#define DM_ITEMDRAWABLEMANAGEROPTIONS_H

#include <QString>
#include <QColor>

#include "cdm_configfile.h"

class DM_ItemDrawableManagerOptions
{
public:
    DM_ItemDrawableManagerOptions();
    DM_ItemDrawableManagerOptions(const DM_ItemDrawableManagerOptions &options);

    void filterString(QString text);
    void filterOnNLevel(int nLevel);
    void setShowOnlySelected(bool enable);

    inline QString filterString() const { return _filterString; }
    inline int nLevelFiltering() const { return _nLevelFiltering; }
    inline bool showOnlySelected() const { return _showOnlySelected; }

    void addColorToList(QColor color);
    void setColorList(QList<QColor> list);
    bool loadColorListFromFile(QString filePath);
    bool saveColorListToFile(QString filePath);
    QList<QColor> getColorList();
    QColor getNextColor();

    QList<QString> getColorListFileExtension();

    void load();
    bool save();

private:

    int             _nLevelFiltering;
    QString         _filterString;
    bool            _showOnlySelected;
    QList<QColor>   _automaticColorList;
    int             _indexColorList;

    bool loadColorList(CDM_ConfigFile *settings);
    bool saveColorList(CDM_ConfigFile *settings);
};

#endif // DM_ITEMDRAWABLEMANAGEROPTIONS_H
