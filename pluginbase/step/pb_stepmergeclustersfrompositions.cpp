#include "pb_stepmergeclustersfrompositions.h"

#include "ct_itemdrawable/ct_scene.h"
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
//#include "ct_turn/inTurn/tools/ct_inturnmanager.h"
//#include "ct_tools/model/ct_generateoutmodelname.h"
//#include "ct_global/ct_context.h"
//#include "ct_model/tools/ct_modelsearchhelper.h"


//Inclusion of actions
#include "actions/pb_actionmodifyclustersgroups.h"

#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <limits>

// Alias for indexing models
#define DEFin_rclusters "rclusters"
#define DEFin_grpClusters "grpClusters"
#define DEFin_cluster "cluster"

#define DEFin_rPos "rPos"
#define DEFin_grpPos "grpPos"
#define DEFin_position "position"

// Constructor : initialization of parameters
PB_StepMergeClustersFromPositions::PB_StepMergeClustersFromPositions(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _interactiveMode = true;
    m_doc = NULL;

    setManual(true);
}

// Step description (tooltip of contextual menu)
QString PB_StepMergeClustersFromPositions::getStepDescription() const
{
    return tr("Regroupe des clusters à partir de positions 2D");
}

// Step detailled description
QString PB_StepMergeClustersFromPositions::getStepDetailledDescription() const
{
    return tr("Cette étape permet de générer une scène pour chaque positions 2D fournie.<br>"
              "Chaque cluster d'entrée est affilié à la position la plus proche (en 2D XY).<br>"
              "<br>"
              "Ensuite une action interactive permet de corriger cette attribution automatique.");
}

// Step URL
QString PB_StepMergeClustersFromPositions::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepMergeClustersFromPositions::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepMergeClustersFromPositions(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepMergeClustersFromPositions::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_rclusters = createNewInResultModel(DEFin_rclusters, tr(""), tr(""), true);
    resIn_rclusters->setZeroOrMoreRootGroup();
    resIn_rclusters->addGroupModel("", DEFin_grpClusters, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rclusters->addItemModel(DEFin_grpClusters, DEFin_cluster, CT_PointCluster::staticGetType(), tr("Cluster"));

    CT_InResultModelGroupToCopy *resIn_rPos = createNewInResultModelForCopy(DEFin_rPos, tr("Positions 2D"), tr(""), true);
    resIn_rPos->setZeroOrMoreRootGroup();
    resIn_rPos->addGroupModel("", DEFin_grpPos, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rPos->addItemModel(DEFin_grpPos, DEFin_position, CT_Point2D::staticGetType(), tr("Position 2D"));

    resIn_rclusters->setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(1);
    resIn_rPos->setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(1);
}

//void PB_StepMergeClustersFromPositions::preProcessCreateOutResultModelListProtected()
//{
//    CT_GenerateOutModelName generator;
//    QList<QString> generatedNames;
//    CT_InTurnManager *tm = getInTurnManager();

//    _outclustergroupname = generator.getNewModelNameThatDontExistIn(*tm, generatedNames, "gcl");
//    generatedNames.append(_outclustergroupname);
//}


// Creation and affiliation of OUT models
void PB_StepMergeClustersFromPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_rPos);
    res->addItemModel(DEFin_grpPos, _outSceneModelName, new CT_Scene(), tr("Scène segmentée"));


//    // récupération du modèle in du résultat
//    CT_InAbstractResultModel *resultmodel = getInResultModel(DEFin_rclusters);

//    if(!isCreateDefaultOutModelActive()
//            && resultmodel->getPossibilitiesSavedSelected().isEmpty())
//    {
//        qFatal("Error while create out model in PB_StepMergeClustersFromPositions");
//    }

//    if(!isCreateDefaultOutModelActive())
//    {
//        // récupération du modèle out de LA possibilité liée au modèle de résultat (il n'y en a qu'une ici) comme spécifié implicitement dans addInResultModel
//        CT_OutAbstractResultModel *resultpossibilitymodel = (CT_OutAbstractResultModel*)resultmodel->getPossibilitiesSavedSelected().first()->outModel();
//        // récupération du modèle in de PointCluster
//        CT_InAbstractModel *pointclustermodel = (CT_InAbstractModel*)PS_MODELS->searchModel(DEFin_cluster, resultpossibilitymodel, this);
//        // récupération du modèle out de LA possilibité liée au modèle de Pointcluster (il n'y en a qu'une ici) CAR on a utilisé C_ChooseOneIfMultiple
//        CT_OutAbstractModel *pointclusterpossibilitymodel = pointclustermodel->getPossibilitiesSavedSelected().first()->outModel();
//        // récupération du modèle du groupe contenant le PointCluster
//        DEF_CT_AbstractGroupModelOut *pointclustergroupmodel = dynamic_cast<DEF_CT_AbstractGroupModelOut*> (pointclusterpossibilitymodel->parentModel()); //groupe qui le pointcluster

//        // On copie le modèle du groupe
//        DEF_CT_AbstractGroupModelOut *pointclustergroupoutmodel = pointclustergroupmodel->copyGroup();
//        pointclustergroupoutmodel->setUniqueName(_outclustergroupname);

//        CT_OutAbstractGroupModel* model = (CT_OutAbstractGroupModel*) PS_MODELS->searchModel(DEFin_grpPos, res->getModels().first(), this);
//        model->addGroup(pointclustergroupoutmodel);
//    }

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMergeClustersFromPositions::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addBool(tr("Correction interactive ?"), "", "", _interactiveMode);
}

void PB_StepMergeClustersFromPositions::compute()
{
    setManual(_interactiveMode);

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_rclusters = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_rsc = outResultList.at(0);


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
        }
    }

    setProgress(10);

    // Création des correspondance clusters / positions
    CT_ResultGroupIterator itIn_grpClusters(resIn_rclusters, this, DEFin_grpClusters);
    while (itIn_grpClusters.hasNext() && !isStopped())
    {
        CT_AbstractItemGroup* grpIn_grpClusters = (CT_AbstractItemGroup*) itIn_grpClusters.next();
        
        CT_PointCluster* cluster = (CT_PointCluster*)grpIn_grpClusters->firstItemByINModelName(this, DEFin_cluster);
        if (cluster != NULL)
        {
            _clustersGroups.insert(cluster, grpIn_grpClusters);

            const Eigen::Vector3d &center = cluster->getCenterCoordinate();

            CT_Point2D* bestPosition = NULL;
            double minDist = std::numeric_limits<double>::max();

            QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > itPos(_positionsData);

            while (itPos.hasNext())
            {
                itPos.next();

                double distance = CT_MathPoint::distance2D(itPos.key()->getCenterCoordinate(), center);
                if (distance < minDist)
                {
                    minDist = distance;
                    bestPosition = (CT_Point2D*) itPos.key();
                }
            }

            if (bestPosition != NULL)
            {
                QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = _positionsData[bestPosition];
                pair.second->append(cluster);
            }
        }

    }

    setProgress(50);

    

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


    setProgress(60);



    // Ajout des points aux nuages d'indices
    QList<QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > cloudIndices = _positionsData.values();

    QFuture<void> futur = QtConcurrent::map(cloudIndices, PB_StepMergeClustersFromPositions::addPointsToScenes);
    int progressMin = futur.progressMinimum();
    int progressTotal = futur.progressMaximum() - futur.progressMinimum();
    while (!futur.isFinished())
    {
        setProgress(60.0 + 30.0*(futur.progressValue() - progressMin)/progressTotal);
    }


    setProgress(90);
    // Création des scènes
//    const CT_OutAbstractGroupModel* outClusterGroupModel = (CT_OutAbstractGroupModel*)PS_MODELS->searchModelForCreation(_outclustergroupname, res_rsc);


    CT_ResultGroupIterator grpPosIt2(res_rsc, this, DEFin_grpPos);
    while (grpPosIt2.hasNext())
    {
        CT_StandardItemGroup* grpPos = (CT_StandardItemGroup*) grpPosIt2.next();
        const CT_Point2D* position = (CT_Point2D*)grpPos->firstItemByINModelName(this, DEFin_position);

        if (position != NULL)
        {
            QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = (QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &) _positionsData.value(position);
            CT_PointCloudIndexVector* cloudIndexVector = pair.first;

            if (cloudIndexVector->size() > 0)
            {
                cloudIndexVector->setSortType(CT_AbstractCloudIndex::SortedInAscendingOrder);

                CT_Scene* scene = new CT_Scene(_outSceneModelName.completeName(), res_rsc, PS_REPOSITORY->registerPointCloudIndex(cloudIndexVector));
                scene->updateBoundingBox();
                grpPos->addItemDrawable(scene);

//                QListIterator<const CT_PointCluster*> itClust(*pair.second);
//                while (itClust.hasNext())
//                {
//                    CT_PointCluster* cluster = (CT_PointCluster*) itClust.next();
//                    CT_AbstractItemGroup *grpClust = (CT_AbstractItemGroup*) _clustersGroups.value(cluster)->copy(outClusterGroupModel, res_rsc, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableReference);
//                    grpPos->addGroup(grpClust);
//                }

            } else {
                delete cloudIndexVector;
            }

            pair.second->clear();
            delete pair.second;
        }
    }

    setProgress(100);
}

void PB_StepMergeClustersFromPositions::addPointsToScenes(QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair)
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

void PB_StepMergeClustersFromPositions::initManualMode()
{
    if(m_doc == NULL)
    {

        QMap<QString, QVariant> param;
        param.insert("Transparency", QVariant(true));

        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument(param);

    }
    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
    m_doc->setCurrentAction(new PB_ActionModifyClustersGroups(&_positionsData));


    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette "
                                                         "étape de filtrage."), QMessageBox::Ok);
}

void PB_StepMergeClustersFromPositions::useManualMode(bool quit)
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

