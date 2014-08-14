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

#ifndef CT_ABSTRACTSTEP_H
#define CT_ABSTRACTSTEP_H

#include "ct_virtualabstractstep.h"

/**
 * @brief Represent a step that can only be added after another step
 *
 * You must redefine in your step at least methods :
 * -------------------------------------------------
 *
 * CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit)   // create a empty copy of your method
 * void createInResultModelListProtected()                                      // create INPUT models
 * void createOutResultModelListProtected()                                     // create OUPUT models
 * void compute()                                                               // your process !
 *
 * If you want you can overload this methods to inform user what your step do :
 * -----------------------------------------------------------------------------
 *
 * QString getStepDescription() const                                           // a brief description of your step
 * QString getStepDetailledDescription() const                                  // a detailled description of your step
 *
 * If you want to add a standard configuration dialog :
 * ----------------------------------------------------
 *
 * void createPreConfigurationDialog()                                          // create a standard pre configuration dialog
 * void createPostConfigurationDialog()                                         // create a standard post configuration dialog
 *
 * If you want to use manual mode :
 * --------------------------------
 *
 * void initManualMode()                                                        // called when manual mode is activate for the first time
 * void useManualMode(bool quit = false)                                        // called every time the manual mode is activated or quitted
 *
 * If you want to use debug mode :
 * --------------------------------
 *
 * Call "setDebuggable(true)" in your constructor and call "waitForAckIfInDebugMode()" in your compute to set a break point
 *
 * void preWaitForAckIfInDebugMode()                                            // Called from gui before step go in debug mode
 * void postWaitForAckIfInDebugMode()                                           // Called from gui after user click next button in debug mode
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractStep : public CT_VirtualAbstractStep
{
    Q_OBJECT

public:
    CT_AbstractStep(CT_StepInitializeData &dataInit);

    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit) = 0;

protected:

    virtual void createInResultModelListProtected() = 0;

    virtual void createOutResultModelListProtected() = 0;

    virtual void compute() = 0;
};

#endif // CT_ABSTRACTSTEP_H
