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

#include "pb_stepcomputehitgrid.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

// Inclusion of actions methods
#include "ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_itemdrawable/ct_grid2dxz.h"
#include "ct_itemdrawable/ct_grid2dyz.h"
#include "ct_itemdrawable/ct_profile.h"
#include "tools/pb_computehitsthread.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include <QFileInfo>
#include <QDebug>
#include <QVector3D>

#define DEF_SearchInResult "r"
#define DEF_SearchInScene   "sc"
#define DEF_SearchInGroup   "gr"

#define DEF_itemOut_grxy "grxy"
#define DEF_itemOut_grxz "grxz"
#define DEF_itemOut_gryz "gryz"

PB_StepComputeHitGrid::PB_StepComputeHitGrid(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _res = 0.5;
}

QString PB_StepComputeHitGrid::getStepDescription() const
{
    // Gives the descrption to print in the GUI
    return tr("Création d'une grille 3D de densité de points");
}

// Step description (tooltip of contextual menu)
QString PB_StepComputeHitGrid::getStepDetailledDescription() const
{
    return tr("Cette étape génère une grille 3D à la <b>résolution</b> spécifiée.<br>"
              "Chaque case reçoit le nombre de points de la scène d'entrée qu'elle contient.<br>"
              "Elle calcule également les profils 2D cumulés en XY, XZ et YZ. ");
}

CT_VirtualAbstractStep* PB_StepComputeHitGrid::createNewInstance(CT_StepInitializeData &dataInit)
{
    // Creates an instance of this step
    return new PB_StepComputeHitGrid(dataInit);
}

void PB_StepComputeHitGrid::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_SearchInResult, tr("Scène(s)"));

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_SearchInGroup);
    resultModel->addItemModel(DEF_SearchInGroup, DEF_SearchInScene, CT_Scene::staticGetType(), tr("Scène"));
}

void PB_StepComputeHitGrid::createOutResultModelListProtected()
{    
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEF_SearchInResult);

    res->addItemModel(DEF_SearchInGroup, _itemOut_grxy_ModelName, new CT_Grid2DXY<int>(), tr("DensityXY"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_grxz_ModelName, new CT_Grid2DXZ<int>(), tr("DensityXZ"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_gryz_ModelName, new CT_Grid2DYZ<int>(), tr("DensityYZ"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_prx_ModelName, new CT_Profile<int>(), tr("ProfilX"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_pry_ModelName, new CT_Profile<int>(), tr("ProfilY"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_prz_ModelName, new CT_Profile<int>(), tr("ProfilZ"));
    res->addItemModel(DEF_SearchInGroup, _itemOut_prdiag_ModelName, new CT_Profile<int>(), tr("ProfilDiag"));
    res->addItemModel(DEF_SearchInGroup, _hits_ModelName, new CT_Grid3D<int>(), tr("Hits"));
}

void PB_StepComputeHitGrid::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Résolution de la grille"),tr("meters"),0.0001,10000,2, _res );
}

void PB_StepComputeHitGrid::compute()
{
    // Gets the out result
    CT_ResultGroup* outResult = getOutResultList().first();

    qDeleteAll(_threadList);
    _threadList.clear();

    CT_ResultGroupIterator it(outResult, this, DEF_SearchInGroup);
    // iterate over all groups
    while(!isStopped() && it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next();
        const CT_Scene* scene = (CT_Scene*)group->firstItemByINModelName(this, DEF_SearchInScene);

        if (scene!=NULL)
        {
            // Declaring the output grids
            CT_Grid3D<int>* hitGrid = CT_Grid3D<int>::createGrid3DFromXYZCoords(_hits_ModelName.completeName(), outResult,
                                                                                scene->minX(), scene->minY(), scene->minZ(),
                                                                                scene->maxX(), scene->maxY(), scene->maxZ(),
                                                                                _res, -1, 0);

            CT_Grid2DXY<int>* grxy = CT_Grid2DXY<int>::createGrid2DXYFromXYCoords(_itemOut_grxy_ModelName.completeName(), outResult,
                                                                                  scene->minX(), scene->minY(),
                                                                                  scene->maxX(), scene->maxY(),
                                                                                  _res, scene->minZ(), -1, 0);

            CT_Grid2DXZ<int>* grxz = CT_Grid2DXZ<int>::createGrid2DXZFromXZCoords(_itemOut_grxz_ModelName.completeName(), outResult,
                                                                                  scene->minX(), scene->minZ(),
                                                                                  scene->maxX(), scene->maxZ(),
                                                                                  _res, scene->minY(), -1, 0);

            CT_Grid2DYZ<int>* gryz = CT_Grid2DYZ<int>::createGrid2DYZFromYZCoords(_itemOut_gryz_ModelName.completeName(), outResult,
                                                                                  scene->minY(), scene->minZ(),
                                                                                  scene->maxY(), scene->maxZ(),
                                                                                  _res, scene->minX(), -1, 0);

            CT_Profile<int>* proX = CT_Profile<int>::createProfileFromSegment(_itemOut_prx_ModelName.completeName(), outResult,
                                                                              scene->minX(), scene->minY(), scene->minZ(),
                                                                              scene->maxX(), scene->minY(), scene->minZ(),
                                                                              _res, -1, 0);

            CT_Profile<int>* proY = CT_Profile<int>::createProfileFromSegment(_itemOut_pry_ModelName.completeName(), outResult,
                                                                              scene->minX(), scene->minY(), scene->minZ(),
                                                                              scene->minX(), scene->maxY(), scene->minZ(),
                                                                              _res, -1, 0);

            CT_Profile<int>* proZ = CT_Profile<int>::createProfileFromSegment(_itemOut_prz_ModelName.completeName(), outResult,
                                                                              scene->minX(), scene->minY(), scene->minZ(),
                                                                              scene->minX(), scene->minY(), scene->maxZ(),
                                                                              _res, -1, 0);

            CT_Profile<int>* proDiag = CT_Profile<int>::createProfileFromSegment(_itemOut_prdiag_ModelName.completeName(), outResult,
                                                                              scene->minX(), scene->minY(), scene->minZ(),
                                                                              scene->maxX(), scene->maxY(), scene->maxZ(),
                                                                              _res, -1, 0);

            grxy->initDrawManager("XY Density", true, true);
            grxz->initDrawManager("XZ Density", true, true);
            gryz->initDrawManager("YZ Density", true, true);

            group->addItemDrawable(hitGrid);
            group->addItemDrawable(grxy);
            group->addItemDrawable(grxz);
            group->addItemDrawable(gryz);
            group->addItemDrawable(proX);
            group->addItemDrawable(proY);
            group->addItemDrawable(proZ);
            group->addItemDrawable(proDiag);

            PB_ComputeHitsThread* hitsThread = new PB_ComputeHitsThread(hitGrid, grxy, grxz, gryz, proX, proY, proZ, proDiag, scene);
            connect(hitsThread, SIGNAL(progressChanged()), this, SLOT(updateProgress()));
            hitsThread->start();
            _threadList.append(hitsThread);
        }
    }

    int size = _threadList.size();
    for (int i = 0 ; i < size ; ++i)
    {
        _threadList.at(i)->wait();
        updateProgress();
    }

    _mutex.lock();
    qDeleteAll(_threadList);
    _threadList.clear();
    _mutex.unlock();

    setProgress(99);
}

void PB_StepComputeHitGrid::updateProgress()
{
    float progress = 0;

    _mutex.lock();
    int size = _threadList.size();
    for (int i = 0 ; i < size ; ++i)
    {
        progress += _threadList.at(i)->getProgress();
    }
    _mutex.unlock();

    if(size > 0)
    {
        progress /= (float)size;
        setProgress(progress);
    }
}

