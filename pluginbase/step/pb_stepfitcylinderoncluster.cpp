#include "pb_stepfitcylinderoncluster.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_beam.h"
#include "ct_itemdrawable/ct_cylinder.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

// Alias for indexing models
#define DEFin_rpoints "rpoints"
#define DEFin_grp "grp"
#define DEFin_points "points"



// Constructor : initialization of parameters
PB_StepFitCylinderOnCluster::PB_StepFitCylinderOnCluster(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepFitCylinderOnCluster::getStepDescription() const
{
    return tr("Ajuste des cylindres sur des clusters");
}

// Step detailled description
QString PB_StepFitCylinderOnCluster::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepFitCylinderOnCluster::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepFitCylinderOnCluster::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepFitCylinderOnCluster(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepFitCylinderOnCluster::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_rpoints = createNewInResultModelForCopy(DEFin_rpoints, tr("Points"));
    resIn_rpoints->setZeroOrMoreRootGroup();
    resIn_rpoints->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rpoints->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Points"));

}

// Creation and affiliation of OUT models
void PB_StepFitCylinderOnCluster::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_rpoints = createNewOutResultModelToCopy(DEFin_rpoints);

    resCpy_rpoints->addItemModel(DEFin_grp, _cyl_ModelName, new CT_Cylinder(), tr("Cylindre"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepFitCylinderOnCluster::createPostConfigurationDialog()
{
    // No parameter dialog for this step
}

void PB_StepFitCylinderOnCluster::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resCpy_rpoints = outResultList.at(0);

    CT_ResultGroupIterator itCpy_grp(resCpy_rpoints, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grpCpy_grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* itemCpy_points = (CT_AbstractItemDrawableWithPointCloud*)grpCpy_grp->firstItemByINModelName(this, DEFin_points);
        if (itemCpy_points != NULL)
        {           
            CT_Cylinder* itemCpy_cyl = CT_Cylinder::staticCreate3DCylinderFromPointCloud(_cyl_ModelName.completeName(), itemCpy_points->id(), resCpy_rpoints, *(itemCpy_points->getPointCloudIndex()), itemCpy_points->getCenterCoordinate());

            if(itemCpy_cyl != NULL)
                grpCpy_grp->addItemDrawable(itemCpy_cyl);
        }

    }    

}
