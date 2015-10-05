#include "pb_stepmergeclustersfrompositions02.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_grid2dxy.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_math/ct_mathpoint.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_global/ct_context.h"

//Inclusion of actions
#include "actions/pb_actionmodifyclustersgroups02.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentMap>
#endif

#include <QMessageBox>
#include <limits>

// Alias for indexing models
#define DEFin_rclusters "rclusters"
#define DEFin_grpClusters "grpClusters"
#define DEFin_cluster "cluster"

#define DEFin_rPos "rPos"
#define DEFin_grpPos "grpPos"
#define DEFin_position "position"

#define DEF_SearchInMNTResult   "mntres"
#define DEF_SearchInMNTGroup    "mntgrp"
#define DEF_SearchInMNT         "mntitem"


// Constructor : initialization of parameters
PB_StepMergeClustersFromPositions02::PB_StepMergeClustersFromPositions02(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _interactiveMode = true;
    _hRef = 1.3;
    _dMax = 2.0;
    m_doc = NULL;

    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepMergeClustersFromPositions02::getStepDescription() const
{
    return tr("Regroupe des clusters à partir de positions 2D (V2)");
}

// Step detailled description
QString PB_StepMergeClustersFromPositions02::getStepDetailledDescription() const
{
    return tr("Cette étape permet de générer une scène pour chaque positions 2D fournie.<br>"
              "Chaque cluster d'entrée est affilié à la position la plus proche (en 2D XY).<br>"
              "<br>"
              "Ensuite une action interactive permet de corriger cette attribution automatique.");
}

// Step URL
QString PB_StepMergeClustersFromPositions02::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepMergeClustersFromPositions02::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepMergeClustersFromPositions02(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepMergeClustersFromPositions02::createInResultModelListProtected()
{  
    CT_InResultModelGroup *resIn_rclusters = createNewInResultModel(DEFin_rclusters, tr(""), tr(""), true);
    resIn_rclusters->setZeroOrMoreRootGroup();
    resIn_rclusters->addGroupModel("", DEFin_grpClusters, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rclusters->addItemModel(DEFin_grpClusters, DEFin_cluster, CT_PointCluster::staticGetType(), tr("Cluster"));

    CT_InResultModelGroupToCopy *resIn_rPos = createNewInResultModelForCopy(DEFin_rPos, tr("Positions 2D"), tr(""), true);
    resIn_rPos->setZeroOrMoreRootGroup();
    resIn_rPos->addGroupModel("", DEFin_grpPos, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rPos->addItemModel(DEFin_grpPos, DEFin_position, CT_Point2D::staticGetType(), tr("Position 2D"));

    CT_InResultModelGroup *resultMNTModel = createNewInResultModel(DEF_SearchInMNTResult, tr("MNT (Raster)"), "", true);
    resultMNTModel->setZeroOrMoreRootGroup();
    resultMNTModel->addGroupModel("", DEF_SearchInMNTGroup);
    resultMNTModel->addItemModel(DEF_SearchInMNTGroup, DEF_SearchInMNT, CT_Grid2DXY<double>::staticGetType(), tr("Modèle Numérique de Terrain"));
    resultMNTModel->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);

    resIn_rclusters->setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(1);
    resIn_rPos->setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(1);
}


// Creation and affiliation of OUT models
void PB_StepMergeClustersFromPositions02::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_rPos);
    res->addItemModel(DEFin_grpPos, _outSceneModelName, new CT_Scene(), tr("Scène segmentée"));
    res->addItemAttributeModel(_outSceneModelName, _outSceneZRefModelName, new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_Z), tr("Z MNT"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMergeClustersFromPositions02::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Hauteur de référence"), "", -99999, 99999, 2, _hRef);
    configDialog->addDouble(tr("Distance maximum entre clusters d'un même groupe"), "", 0, 99999, 2, _dMax);
    configDialog->addBool(tr("Correction interactive ?"), "", "", _interactiveMode);
}

void PB_StepMergeClustersFromPositions02::compute()
{
    setManual(_interactiveMode);

    CT_ResultGroup* resIn_rclusters = getInputResultsForModel(DEFin_rclusters).first();
    CT_ResultGroup* inMNTResult = getInputResultsForModel(DEF_SearchInMNTResult).first();

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_rsc = outResultList.at(0);

    // Récupération du MNT
    CT_Grid2DXY<double>* mnt = NULL;
    CT_ResultItemIterator it(inMNTResult, this, DEF_SearchInMNT);
    if(it.hasNext()) {mnt = (CT_Grid2DXY<double>*) it.next();}

    QMap<const CT_Point2D*, double> positionsZRef;

    // Création de la liste des positions 2D
    CT_ResultGroupIterator grpPosIt(res_rsc, this, DEFin_grpPos);
    while (grpPosIt.hasNext() && !isStopped())
    {
        CT_AbstractItemGroup* grpPos = (CT_AbstractItemGroup*) grpPosIt.next();

        const CT_Point2D* position = (CT_Point2D*)grpPos->firstItemByINModelName(this, DEFin_position);
        if (position != NULL)
        {
            CT_PointCloudIndexVector* cloudIndexVector = new CT_PointCloudIndexVector();
            cloudIndexVector->setSortType(CT_AbstractCloudIndex::NotSorted);
            _positionsData.insert(position, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* >(cloudIndexVector, new QList<const CT_PointCluster*>()));

            double mntVal = 0;
            if (mnt != NULL) {mntVal = mnt->valueAtXY(position->getCenterX(), position->getCenterY());}
            if (mntVal == mnt->NA()) {mntVal = 0;}

            positionsZRef.insert(position, mntVal + _hRef);
        }
    }

    setProgress(1);

    // Création de la correspondance clusters / groupes
    CT_ResultGroupIterator itIn_grpClusters(resIn_rclusters, this, DEFin_grpClusters);
    while (itIn_grpClusters.hasNext() && !isStopped())
    {
        CT_AbstractItemGroup* grpIn_grpClusters = (CT_AbstractItemGroup*) itIn_grpClusters.next();
        CT_PointCluster* cluster = (CT_PointCluster*)grpIn_grpClusters->firstItemByINModelName(this, DEFin_cluster);
        if (cluster != NULL)
        {
            _clustersGroups.insert(cluster, grpIn_grpClusters);
        }
    }

    setProgress(3);


    // Création des correspondance clusters / positions
    // Phase 1 : recherche du cluster le plus proche de chaque position
    QList<CT_PointCluster*> clustersPhase1;

    QMutableMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > itPos(_positionsData);
    while (itPos.hasNext())
    {
        itPos.next();
        Eigen::Vector3d posCenter = itPos.key()->getCenterCoordinate();
        posCenter(2) = positionsZRef.value(itPos.key());

        CT_PointCluster* bestCluster = NULL;
        double minDist = std::numeric_limits<double>::max();

        CT_ResultGroupIterator itIn_grpClusters2(resIn_rclusters, this, DEFin_grpClusters);
        while (itIn_grpClusters2.hasNext() && !isStopped())
        {
            CT_AbstractItemGroup* grpIn_grpClusters = (CT_AbstractItemGroup*) itIn_grpClusters2.next();

            CT_PointCluster* cluster = (CT_PointCluster*)grpIn_grpClusters->firstItemByINModelName(this, DEFin_cluster);
            if (cluster != NULL && !clustersPhase1.contains(cluster))
            {
                const Eigen::Vector3d &center = cluster->getCenterCoordinate();
                double distance = squareDist(posCenter, center);
                if (distance <= _dMax && distance < minDist)
                {
                    minDist = distance;
                    bestCluster = cluster;
                }
            }
        }

        if (bestCluster != NULL)
        {
            clustersPhase1.append(bestCluster);
            QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = itPos.value();
            pair.second->append(bestCluster);
        }
    }

    setProgress(5);

    // Création des correspondance clusters / positions
    // Phase 2 : création de la map des distances
    QList<ClusterData> clusterDataList;

    CT_ResultGroupIterator itIn_grpClusters3(resIn_rclusters, this, DEFin_grpClusters);
    while (itIn_grpClusters3.hasNext() && !isStopped())
    {
        CT_AbstractItemGroup* grpIn_grpClusters = (CT_AbstractItemGroup*) itIn_grpClusters3.next();

        CT_PointCluster* cluster = (CT_PointCluster*)grpIn_grpClusters->firstItemByINModelName(this, DEFin_cluster);
        if (cluster != NULL && !clustersPhase1.contains(cluster))
        {
            const Eigen::Vector3d &center = cluster->getCenterCoordinate();
            CT_PointCluster* bestCluster = NULL;
            CT_Point2D* bestPosition = NULL;
            double minDist = std::numeric_limits<double>::max();

            QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > itPos(_positionsData);
            while (itPos.hasNext())
            {
                itPos.next();
                const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = itPos.value();

                if (pair.second->size() > 0)
                {
                    const CT_PointCluster* clusterForPos = pair.second->first();
                    const Eigen::Vector3d &clusterForPosCenter = clusterForPos->getCenterCoordinate();

                    double distance = squareDist(clusterForPosCenter, center);
                    if (distance < minDist)
                    {
                        minDist = distance;
                        bestPosition = (CT_Point2D*) itPos.key();
                        bestCluster = (CT_PointCluster*) clusterForPos;
                    }
                }
            }

            if (bestPosition != NULL)
            {
                clusterDataList.append(ClusterData(cluster, minDist, bestPosition, bestCluster));
            }
        }
    }

    setProgress(10);


    // Création des correspondance clusters / positions
    // Phase 3 : aggrégation des clusters par proximité relative
    int cpt = -1;
    int nbClust = clusterDataList.size();
    double distance = 0;
    bool changed = true;

    while (!clusterDataList.isEmpty() && !isStopped())
    {
        if (changed)
        {
            std::sort(clusterDataList.begin(), clusterDataList.end());
            changed = false;
        }

        ClusterData clusterData = clusterDataList.takeFirst();

        if (clusterData._distance > _dMax)
        {
            _trash.append(clusterData._cluster);
        } else {
            // Affectation du cluster à la position la plus proche
            QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = (QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &) _positionsData.value(clusterData._position);
            pair.second->append(clusterData._cluster);

            _clusterToCluster.insert(clusterData._positionCluster, clusterData._cluster);

            // Mise à jour de la map des distances à l'aide de ce cluster
            for (int i = 0 ; i < clusterDataList.size() ; i++)
            {
                ClusterData& clusterD = (ClusterData&) clusterDataList.at(i);

                distance = squareDist(clusterData.center(), clusterD.center());
                if (distance < _dMax && distance < clusterD._distance)
                {
                    clusterD._position = clusterData._position;
                    clusterD._distance = distance;
                    clusterD._positionCluster = clusterData._cluster;
                    changed = true;
                }
            }
        }

        if (++cpt % 500 == 0) {setProgress(10.0 + ((float)cpt / (float)nbClust)*70.0);}
    }

    setProgress(80);

    // Début de la partie interactive
    if (_interactiveMode)
    {
        m_doc = NULL;

        m_status = 0;
        requestManualMode();

        m_status = 1;
        requestManualMode();
    }
    // Fin de la partie interactive


    // Ajout des points aux nuages d'indices
    QList<QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > cloudIndices = _positionsData.values();

    QFuture<void> futur = QtConcurrent::map(cloudIndices, PB_StepMergeClustersFromPositions02::addPointsToScenes);
    int progressMin = futur.progressMinimum();
    int progressTotal = futur.progressMaximum() - futur.progressMinimum();
    while (!futur.isFinished())
    {
        setProgress(80.0 + 19.0*(futur.progressValue() - progressMin)/progressTotal);
    }


    setProgress(99);

    // Création des scènes
    CT_ResultGroupIterator grpPosIt2(res_rsc, this, DEFin_grpPos);
    while (grpPosIt2.hasNext())
    {
        CT_StandardItemGroup* grpPos = (CT_StandardItemGroup*) grpPosIt2.next();
        const CT_Point2D* position = (CT_Point2D*)grpPos->firstItemByINModelName(this, DEFin_position);

        if (position != NULL)
        {
            double zRef = positionsZRef.value(position);

            QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = (QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &) _positionsData.value(position);
            CT_PointCloudIndexVector* cloudIndexVector = pair.first;

            if (cloudIndexVector->size() > 0)
            {
                cloudIndexVector->setSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);

                CT_Scene* scene = new CT_Scene(_outSceneModelName.completeName(), res_rsc, PS_REPOSITORY->registerPointCloudIndex(cloudIndexVector));
                scene->updateBoundingBox();
                grpPos->addItemDrawable(scene);

                scene->addItemAttribute(new CT_StdItemAttributeT<double>(_outSceneZRefModelName.completeName(), CT_AbstractCategory::DATA_Z, res_rsc, (zRef - _hRef)));
            } else {
                delete cloudIndexVector;
            }

            pair.second->clear();
            delete pair.second;
        }
    }

    setProgress(100);
}

void PB_StepMergeClustersFromPositions02::addPointsToScenes(QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair)
{
    CT_PointCloudIndexVector* cloudIndexVector = pair.first;
    QList<const CT_PointCluster*> *list = pair.second;

    for (int i = 0 ; i < list->size() ; i++)
    {
        const CT_AbstractCloudIndex* clIndex = list->at(i)->getPointCloudIndex();
        for (size_t p = 0 ; p < clIndex->size() ; p++)
        {
            cloudIndexVector->addIndex(clIndex->constIndexAt(p));
        }
    }
}

void PB_StepMergeClustersFromPositions02::initManualMode()
{
    // create a new 3D document
    if(m_doc == NULL)
        m_doc = getGuiContext()->documentManager()->new3DDocument();

    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
    m_doc->setCurrentAction(new PB_ActionModifyClustersGroups02(&_positionsData, &_clusterToCluster, &_trash));


    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette "
                                                         "étape de filtrage."), QMessageBox::Ok);
}

void PB_StepMergeClustersFromPositions02::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            m_itemDrawableSelected = m_doc->getSelectedItemDrawable();
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            m_doc = NULL;

            quitManualMode();
        }
    }
}
