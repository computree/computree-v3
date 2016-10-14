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



#ifndef CT_STEPINITIALIZEDATA_H
#define CT_STEPINITIALIZEDATA_H

#include "interfaces.h"
#include "pluginShared_global.h"

#include <QSettings>

class CT_VirtualAbstractStep;
class CT_AbstractStepPlugin;

class PLUGINSHAREDSHARED_EXPORT CT_StepInitializeData
{
public:
    CT_StepInitializeData(const CT_VirtualAbstractStep *parent, const CT_AbstractStepPlugin &pluginManager, QSettings *settings, bool menuStepFlag = false);
    CT_StepInitializeData(const CT_StepInitializeData &data);
    virtual ~CT_StepInitializeData();

    inline CT_VirtualAbstractStep* getParentStep() const { return _parentStep; }
    inline CT_AbstractStepPlugin* getPlugin() const { return _pluginManager; }
    inline QSettings* getSettingsFile() const { return _settings; }

    void setParentStep(const CT_VirtualAbstractStep *parent);

    inline bool getMenuStepFlag() const {return _menuStepFlag;}

private:

    friend class CT_Context;

    CT_StepInitializeData(QSettings *settings);

    CT_VirtualAbstractStep  *_parentStep;
    CT_AbstractStepPlugin   *_pluginManager;
    QSettings               *_settings;
    bool                     _menuStepFlag;
};

#endif // CT_STEPINITIALIZEDATA_H
