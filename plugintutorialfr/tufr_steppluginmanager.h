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

#ifndef TUFR_STEPPLUGINMANAGER_H
#define TUFR_STEPPLUGINMANAGER_H

#include "ct_abstractstepplugin.h"

class TUFR_StepPluginManager : public CT_AbstractStepPlugin
{
public:
    TUFR_StepPluginManager();
    ~TUFR_StepPluginManager();

protected:

    bool loadGenericsStep();
    bool loadOpenFileStep();
    bool loadCanBeAddedFirstStep();

};

#endif // TUFR_STEPPLUGINMANAGER_H
