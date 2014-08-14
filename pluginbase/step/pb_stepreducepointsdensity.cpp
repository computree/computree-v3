/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "pb_stepreducepointsdensity.h"

#include "ct_global/ct_context.h"

#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_grid3d.h"

#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_view/ct_buttongroup.h"

#include <math.h>
#include <iostream>
#include <QList>
#include <limits>

#define DEF_SearchInResult "r"
#define DEF_SearchInScene   "sc"

#define DEF_SearchOutGroup  "g"
#define DEF_SearchOutScene  "sc"
#define DEF_SearchOutResult "r"

PB_StepReducePointsDensity::PB_StepReducePointsDensity(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _resolution = 0.005;
}

QString PB_StepReducePointsDensity::getStepDescription() const
{
    return tr("Réduit la densité de scènes, sur la base d'une grille régulière");
}

QString PB_StepReducePointsDensity::getStepDetailledDescription() const
{
    return tr("Créée une grille régulière de la <b>résolution</b> choisie. Ne garde que le point le plus proche du centre dans chaque case. ");
}

CT_VirtualAbstractStep* PB_StepReducePointsDensity::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepReducePointsDensity(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepReducePointsDensity::createInResultModelListProtected()
{
    CT_InOneOrMoreGroupModel *group = new CT_InOneOrMoreGroupModel();

    CT_InStandardItemDrawableModel *item = new CT_InStandardItemDrawableModel(DEF_SearchInScene,
                                                                              CT_Scene::staticGetType(),
                                                                              tr("Scène"));

    group->addItem(item);

    CT_InResultModelGroup *resultModel = new CT_InResultModelGroup(DEF_SearchInResult, group, tr("Scène(s)"));

    addInResultModel(resultModel);
}

// Création et affiliation des modèles OUT
void PB_StepReducePointsDensity::createOutResultModelListProtected()
{
    // Creation du model de groupe racine
    CT_OutStdGroupModel *group = new CT_OutStdGroupModel(DEF_SearchOutGroup, new CT_StandardItemGroup(), tr("Groupe"));

    // Creation du modele d'item pour la scène
    CT_OutStdSingularItemModel *item = new CT_OutStdSingularItemModel(DEF_SearchOutScene,
                                                                                new CT_Scene(),
                                                                                tr("Scène"));

    // ajout du modèle d'item de scene au modele de groupe racine
    group->addItem(item);

    // Creation du modèle de résultat
    CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(DEF_SearchOutResult, group,  tr("Scène à densité réduite"));

    // Affectation du modèle de résultat à l'étape ==> conduira à la création effective de ce résulat avec ce modèle
    addOutResultModel(resultModel);
}

void PB_StepReducePointsDensity::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Résolution de la grille :"), "cm", 0, 10000, 2, _resolution, 100);
}

void PB_StepReducePointsDensity::compute()
{

    // Result IN
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup *inResult = inResultList.first();
    CT_InStandardItemDrawableModel *inSceneModel = (CT_InStandardItemDrawableModel*) getInModelForResearch(inResult, DEF_SearchInScene);

    // Result OUT
    const QList<CT_ResultGroup*> &outResList = getOutResultList();
    CT_ResultGroup *outResult = outResList.first();
    CT_OutStdGroupModel *groupModel = (CT_OutStdGroupModel*) getOutModelForCreation(outResult, DEF_SearchOutGroup);
    CT_OutStdSingularItemModel *outSceneModel = (CT_OutStdSingularItemModel*) getOutModelForCreation(outResult, DEF_SearchOutScene);


    for ( CT_Scene *in_scene = (CT_Scene*) inResult->beginItem(inSceneModel)
          ; in_scene != NULL  && !isStopped()
          ; in_scene = (CT_Scene*) inResult->nextItem())
    {
        const CT_AbstractPointCloudIndex *pointCloudIndex = in_scene->getPointCloudIndex();
        size_t n_points = pointCloudIndex->indexSize();

        _minx = in_scene->minX();
        _miny = in_scene->minY();
        _minz = in_scene->minZ();

        _dimx = ceil((in_scene->maxX() - _minx)/_resolution);
        _dimy = ceil((in_scene->maxY() - _miny)/_resolution);
        _dimz = ceil((in_scene->maxZ() - _minz)/_resolution);

        _halfResolution = _resolution / 2;

        PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("La scène d'entrée comporte %1 points.")).arg(n_points));

        QMap<size_t, size_t> indexMap;

        // Extraction des points de la placette
        size_t i = 0;

        while((i<n_points) && (!isStopped()))
        {
            size_t pointIndex;
            const CT_Point &point = pointCloudIndex->constTAt(i, pointIndex);

            size_t col, row, levz;
            size_t grdIndex = gridIndex(point.getX(), point.getY(), point.getZ(), col, row, levz);

            size_t previousPointIndex = indexMap.value(grdIndex, std::numeric_limits<size_t>::max());

            if (previousPointIndex == std::numeric_limits<size_t>::max())
            {
                indexMap.insert(grdIndex, pointIndex);
            } else {

                float gridx, gridy, gridz;
                cellCoordinates(col, row, levz, gridx, gridy, gridz);

                float distance = pow(point.getX() - gridx, 2) + pow(point.getY() - gridy, 2) + pow(point.getZ() - gridz, 2);

                const CT_Point &previousPoint = pointCloudIndex->constTAtGlobalIndex(previousPointIndex);

                float previousDistance = pow(previousPoint.getX() - gridx, 2) + pow(previousPoint.getY() - gridy, 2) + pow(previousPoint.getZ() - gridz, 2);

                if (distance < previousDistance)
                {
                    indexMap.insert(grdIndex, pointIndex);
                }
            }
            ++i;

            if (i % 10000 == 0) {setProgress(50 * i / n_points);}
        }


        QMapIterator<size_t, size_t> it(indexMap);

        size_t npts = indexMap.size();
        i = 0;

        CT_PointCloudIndexVector *resPointCloudIndex = new CT_PointCloudIndexVector(npts);

        while (it.hasNext() && (!isStopped()))
        {
            it.next();

            resPointCloudIndex->replaceIndex(i, it.value(), false);

            if (i % 10000 == 0) {setProgress(50 + 49 * i / npts);}
            ++i;
        }

        if (resPointCloudIndex->indexSize() > 0)
        {
            CT_StandardItemGroup *outGroup = new CT_StandardItemGroup(groupModel, outResult);
            CT_Scene *outScene = new CT_Scene(outSceneModel, outResult);

            outScene->setBoundingBox(in_scene->minX(),in_scene->minY(),in_scene->minZ(), in_scene->maxX(),in_scene->maxY(),in_scene->maxZ());
            outScene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(resPointCloudIndex));
            outGroup->addItemDrawable(outScene);
            outResult->addGroup(outGroup);

            PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("La scène de densité réduite comporte %1 points.")).arg(outScene->getPointCloudIndex()->indexSize()));
        } else {

            PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Aucun point conservé pour cette scène"));
        }

        setProgress(99);
    }

}

size_t PB_StepReducePointsDensity::gridIndex(const float &x, const float &y, const float &z, size_t &colx, size_t &liny, size_t &levz) const
{
    colx = (size_t) floor((x - _minx) / _resolution);
    liny = (size_t) floor((y - _miny) / _resolution);
    levz = (size_t) floor((z - _minz) / _resolution);

    return levz*_dimx*_dimy + liny*_dimx + colx;;
}

void PB_StepReducePointsDensity::cellCoordinates(const size_t &colx, const size_t &liny, const size_t &levz, float &x, float &y, float &z) const
{
    x = _minx + colx*_resolution + _halfResolution;
    y = _miny + liny*_resolution + _halfResolution;
    z = _minz + levz*_resolution + _halfResolution;
}
