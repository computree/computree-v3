#include "pb_stepapplypointfilters.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_stdlaspointsattributescontainer.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "filter/pb_filterbyreturntype.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_lasAtt "lasAtt"



// Constructor : initialization of parameters
PB_StepApplyPointFilters::PB_StepApplyPointFilters(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _aaa = false;
}

// Step description (tooltip of contextual menu)
QString PB_StepApplyPointFilters::getStepDescription() const
{
    return tr("Applique des filtres sur les points");
}

// Step detailled description
QString PB_StepApplyPointFilters::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepApplyPointFilters::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepApplyPointFilters::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepApplyPointFilters(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepApplyPointFilters::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Item contenant des points"));
    resIn_res->addItemModel(DEFin_grp, DEFin_lasAtt, CT_StdLASPointsAttributesContainer::staticGetType(), tr("Attributs LAS"));
}

// Creation and affiliation of OUT models
void PB_StepApplyPointFilters::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    resCpy_res->addItemModel(DEFin_grp, _outPoints_ModelName, new CT_Scene(), tr("PointsFiltrés"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepApplyPointFilters::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addBool("", "", "", _aaa);
}

void PB_StepApplyPointFilters::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* _res = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(_res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* points = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_points);
        const CT_StdLASPointsAttributesContainer* lasAtt = (CT_StdLASPointsAttributesContainer*)grp->firstItemByINModelName(this, DEFin_lasAtt);
        if (points != NULL && lasAtt != NULL)
        {
            PB_FilterByReturnType filter(PB_FilterByReturnType::Last);
            filter.setPointCloudIndex(points->getPointCloudIndex());
            filter.setLASAttributesContainer(lasAtt);

            CT_PointCloudIndexVector* outCloud = filter.filterPointCloudIndex();



            if (outCloud->size() > 0)
            {
                CT_Scene* outScene = new CT_Scene(_outPoints_ModelName.completeName(), _res);
                outScene->updateBoundingBox();
                grp->addItemDrawable(outScene);

                // The PCI must be registered to the point repository :
                outScene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(outCloud));
            } else {delete outCloud;}
        }
    }    
}
