/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)
              Alexandre PIBOULE (ONF)

 This file is part of PluginONFENSAM library 2.0.

 PluginONFENSAM is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginONFENSAM is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginONFENSAM.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "tufr_pluginentry.h"
#include "tufr_steppluginmanager.h"

TUFR_PluginEntry::TUFR_PluginEntry()
{
    _stepPluginManager = new TUFR_StepPluginManager();
    _stepPluginManager->init();
}

TUFR_PluginEntry::~TUFR_PluginEntry()
{
    delete _stepPluginManager;
}

QString TUFR_PluginEntry::getVersion() const
{
    return "1.0";
}

StepPluginInterface* TUFR_PluginEntry::getStepPluginManager()
{
    return _stepPluginManager;
}

Q_EXPORT_PLUGIN2(plug_tutorialfr, TUFR_PluginEntry)
