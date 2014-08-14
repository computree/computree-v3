/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef PB_PLUGINENTRY_H
#define PB_PLUGINENTRY_H

/*! \mainpage Computree
 *
 * \section Copyright
 *
 * Office National des Forets (ONF), France
 *
 * Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Superieure d'Arts et Metiers (ENSAM), Cluny, France
 *
 * \section Authors
 * \li Alexandre Piboule (ONF)
 *
 * \li Michael Krebs (ARTS/ENSAM)
 *
 * \section Licence
 *
 * This library is distributed under the LGPL licence
 *
 */


/*! \defgroup Steps_PB PluginBase (PB) steps
 */


#include "interfaces.h"

class PB_StepPluginManager;

class PB_PluginEntry : public PluginEntryInterface
{
    Q_OBJECT

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID PluginEntryInterface_iid)
#endif

    Q_INTERFACES(PluginEntryInterface)

public:
    PB_PluginEntry();
    ~PB_PluginEntry();

    QString getVersion() const;
    PluginInterface* getPlugin();

private:
    PB_StepPluginManager *_stepPluginManager;
};

#endif // PB_PLUGINENTRY_H
