#include "pb_stepsegmentcrownsfromstemclusters.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_scene "scene"
#define DEFin_grpPos "grpPos"
#define DEFin_position "pos"

// Constructor : initialization of parameters
PB_StepSegmentCrownsFromStemClusters::PB_StepSegmentCrownsFromStemClusters(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _distMax = 1.0;
}

// Step description (tooltip of contextual menu)
QString PB_StepSegmentCrownsFromStemClusters::getStepDescription() const
{
    return tr("Segmente des houppiers à partir de clusters tiges");
}

// Step detailled description
QString PB_StepSegmentCrownsFromStemClusters::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepSegmentCrownsFromStemClusters::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSegmentCrownsFromStemClusters::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSegmentCrownsFromStemClusters(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSegmentCrownsFromStemClusters::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Tiges détéctées"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_scene, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène complète"));
    resIn_res->addGroupModel(DEFin_grp, DEFin_grpPos, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grpPos, DEFin_position, CT_PointCluster::staticGetType(), tr("Cluster Position"));
}

// Creation and affiliation of OUT models
void PB_StepSegmentCrownsFromStemClusters::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);
    resCpy_res->addItemModel(DEFin_grpPos, _outScene_ModelName, new CT_Scene(), tr("Scène segmentée"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSegmentCrownsFromStemClusters::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addDouble(tr("Distance Max"), "m", 0.01, 9999, 2, _distMax);
}

void PB_StepSegmentCrownsFromStemClusters::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res = outResultList.at(0);

    CT_ResultGroupIterator itCpy_grp(res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        CT_AbstractItemDrawableWithPointCloud* in_scene = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_scene);

        if (in_scene != NULL)
        {
            const CT_AbstractPointCloudIndex *pointCloudIndex = in_scene->getPointCloudIndex();
            size_t n_points = pointCloudIndex->size();


            // List of positions
            QMap<CT_PointCluster*, CT_StandardItemGroup*> positionsGroups;
            QMap<CT_PointCluster*, CT_PointCloudIndexVector*> positionsClouds;
            CT_GroupIterator itCpy_grpPos(grp, this, DEFin_grpPos);
            while (itCpy_grpPos.hasNext() && !isStopped())
            {
                CT_StandardItemGroup* grpPos = (CT_StandardItemGroup*) itCpy_grpPos.next();
                CT_PointCluster* position = (CT_PointCluster*) grpPos->firstItemByINModelName(this, DEFin_position);
                if (position != NULL)
                {
                    positionsGroups.insert(position, grpPos);
                    positionsClouds.insert(position, new CT_PointCloudIndexVector());
                }
            }
            setProgress(25);


            CT_PointCloudIndexVector *resPointCloudIndex = new CT_PointCloudIndexVector();
            if (resPointCloudIndex->size() > 0)
            {
                // creation et ajout de la scene
                CT_Scene *outScene = new CT_Scene(_outScene_ModelName.completeName(), res, PS_REPOSITORY->registerPointCloudIndex(resPointCloudIndex));
                outScene->updateBoundingBox();
                //grpPos->addItemDrawable(outScene);
            } else {
                delete resPointCloudIndex;
            }

            positionsGroups.clear();
            setProgress(99);
        }
    }
    setProgress(100);
}


