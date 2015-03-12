#include "pb_stepmergeclustersfrompositions.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_math/ct_mathpoint.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_global/ct_context.h"

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

#define DEFout_rsc "rsc"
#define DEFout_grp "grp"
#define DEFout_scene "scene"


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

    CT_InResultModelGroup *resIn_rPos = createNewInResultModel(DEFin_rPos, tr("Positions 2D"), tr(""), true);
    resIn_rPos->setZeroOrMoreRootGroup();
    resIn_rPos->addGroupModel("", DEFin_grpPos, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rPos->addItemModel(DEFin_grpPos, DEFin_position, CT_Point2D::staticGetType(), tr("Position 2D"));

}

// Creation and affiliation of OUT models
void PB_StepMergeClustersFromPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_rsc = createNewOutResultModel(DEFout_rsc, tr("Scènes segmentées"));
    res_rsc->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_rsc->addItemModel(DEFout_grp, DEFout_scene, new CT_Scene(), tr("Scène segmentée"));

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
    CT_ResultGroup* resIn_rPos = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_rsc = outResultList.at(0);


    QMap<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > positionsData;
    // Création de la liste des positions 2D
    CT_ResultGroupIterator itIn_grpPos(resIn_rPos, this, DEFin_grpPos);
    while (itIn_grpPos.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grpPos = (CT_AbstractItemGroup*) itIn_grpPos.next();

        const CT_Point2D* position = (CT_Point2D*)grpIn_grpPos->firstItemByINModelName(this, DEFin_position);
        if (position != NULL)
        {
            positionsData.insert(position, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* >(new CT_PointCloudIndexVector(), new QList<const CT_PointCluster*>()));
        }
    }

    setProgress(10);


    // Création des correspondance clusters / positions
    CT_ResultGroupIterator itIn_grpClusters(resIn_rclusters, this, DEFin_grpClusters);
    while (itIn_grpClusters.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grpClusters = (CT_AbstractItemGroup*) itIn_grpClusters.next();
        
        const CT_PointCluster* cluster = (CT_PointCluster*)grpIn_grpClusters->firstItemByINModelName(this, DEFin_cluster);
        if (cluster != NULL)
        {
            const Eigen::Vector3d &center = cluster->getCenterCoordinate();

            CT_Point2D* bestPosition = NULL;
            double minDist = std::numeric_limits<double>::max();

            QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > itPos(positionsData);

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
                QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = (QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > ) positionsData.value(bestPosition);
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
    QList<QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > cloudIndices = positionsData.values();

    QFuture<void> futur = QtConcurrent::map(cloudIndices, PB_StepMergeClustersFromPositions::addPointsToScenes);
    int progressMin = futur.progressMinimum();
    int progressTotal = futur.progressMaximum() - futur.progressMinimum();
    while (!futur.isFinished())
    {
        setProgress(60.0 + 30.0*(futur.progressValue() - progressMin)/progressTotal);
    }


    setProgress(90);
    // Création des scènes
    for (int i = 0 ; i < cloudIndices.size() ; i++)
    {
        const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair  = cloudIndices.at(i);
        CT_PointCloudIndexVector* cloudIndexVector = pair.first;

        pair.second->clear();
        delete pair.second;

        if (cloudIndexVector->size() > 0)
        {
            CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, res_rsc);
            res_rsc->addGroup(grp);

            CT_Scene* scene = new CT_Scene(DEFout_scene, res_rsc, PS_REPOSITORY->registerPointCloudIndex(cloudIndexVector));
            scene->updateBoundingBox();
            grp->addItemDrawable(scene);

        } else {
            delete cloudIndexVector;
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


    // TODO add async with GuiManagerInterface
    QHashIterator<CT_AbstractItemDrawable*, CT_AbstractItemGroup*> it(m_itemDrawableToAdd);

    while(it.hasNext())
        m_doc->addItemDrawable(*it.next().key());

    // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
    m_doc->setCurrentAction(new PB_ActionModifyClustersGroups());



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

