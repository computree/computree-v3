/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef PB_STEPCOMPUTEHITGRID_H
#define PB_STEPCOMPUTEHITGRID_H

#include "ct_step/abstract/ct_abstractstep.h"                    // The step inherits from ct_abstractstep

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_tools/ct_monitoredqthread.h"


/** \class  PB_StepComputeHitGrid
    \brief Compute number of point for each cell of a Grid3D
*/
class PB_StepComputeHitGrid : public CT_AbstractStep
{
    /** \def    Q_OBJECT :
        \brief  Macro from the Qt environment. Useful to get the name of the step (among others)*/
    Q_OBJECT

public:
    /*!
    *  \brief Constructor
    *
    *  Constructor of the class
    *  \param dataInit : informations about the step : parent step, plugin manager managing this step and some settings from Qt (for the Q_OBJECT?).
    */
    PB_StepComputeHitGrid(CT_StepInitializeData &dataInit);

    /*!
    *  \brief Gives a description of the step
    *  This method will be used in the GUI management : this description will be shown to the user when the mouse is over the step in the step menu.
    *
    *  \warning Inherited from CT_AbstractStep, this method is not pure virtual (could be not defined here).
    */
    virtual QString getStepDescription() const;

    /**
     * @brief Inherit this method if you want to return your own detailled description.
     *
     *        By default return a empty string
     */
    virtual QString getStepDetailledDescription() const;

    /*!
    *  \brief Creates a new instance of this step
    *
    *  \param dataInit : informations about the step : parent step, plugin manager managing this step and some settings from Qt (for the Q_OBJECT?).
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:
    /*!
    *  \brief Creates the input of this step
    *
    *  This method defines what kind of input the step can accept and get them
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void createInResultModelListProtected();

    /*!
    *  \brief Creates the output of this step
    *
    *  This method defines what kind of results the step produces
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void createOutResultModelListProtected();

    /*!
    *  \brief Create a specific configuration dialog window for the step
    *
    *  This method creates a window for the user to set the different parameters of the step.
    *  The different parameters that can be set by the users and their properties (min, max, level of detail) are defined here :
    *       To add a parameter to this dialog window, just do a "_configDialog->addTYPEOFVALUE(...)"
    *
    *  \warning Inherited from CT_AbstractStep, this method is not pure virtual (could be not defined here).
    */
    virtual void createPostConfigurationDialog();

    /*!
    *  \brief Runs the step process
    *
    *  This method contains the effective calculus of the step (running the step is equivalent to launch this method).
    *  You might consider it as a "main" function for a step.
    *
    *  \warning Inherited from CT_AbstractStep, this method is pure virtual, each step MUST define its own method.
    */
    virtual void compute();

public slots:
    void updateProgress();

private:

    // Declaration of autoRenames Variables (groups or items addes to In models copies)
    CT_AutoRenameModels _hits_ModelName;
    CT_AutoRenameModels _itemOut_grxy_ModelName;
    CT_AutoRenameModels _itemOut_grxz_ModelName;
    CT_AutoRenameModels _itemOut_gryz_ModelName;
    CT_AutoRenameModels _itemOut_prx_ModelName;
    CT_AutoRenameModels _itemOut_pry_ModelName;
    CT_AutoRenameModels _itemOut_prz_ModelName;
    CT_AutoRenameModels _itemOut_prdiag_ModelName;

    QList<CT_MonitoredQThread*>     _threadList;

//********************************************//
//              Attributes of LVox            //
//********************************************//

    double          _res;                   /*!< size of a voxel*/

};

#endif // PB_STEPCOMPUTEHITGRID_H



