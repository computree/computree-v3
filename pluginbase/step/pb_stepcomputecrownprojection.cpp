#include "pb_stepcomputecrownprojection.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_beam.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFin_rscene "rscene"
#define DEFin_grpsc "grpsc"
#define DEFin_scene "scene"



// Constructor : initialization of parameters
PB_StepComputeCrownProjection::PB_StepComputeCrownProjection(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _computeSlices = true;
    _spacing = 0.5;
    _thickness = 2;
}

// Step description (tooltip of contextual menu)
QString PB_StepComputeCrownProjection::getStepDescription() const
{
    return tr("Projection de houppiers");
}

// Step detailled description
QString PB_StepComputeCrownProjection::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepComputeCrownProjection::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputeCrownProjection::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputeCrownProjection(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputeCrownProjection::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_rscene = createNewInResultModelForCopy(DEFin_rscene, tr("Scène(s)"));
    resIn_rscene->setZeroOrMoreRootGroup();
    resIn_rscene->addGroupModel("", DEFin_grpsc, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_rscene->addItemModel(DEFin_grpsc, DEFin_scene, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));

}

// Creation and affiliation of OUT models
void PB_StepComputeCrownProjection::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_rscene = createNewOutResultModelToCopy(DEFin_rscene);

    resCpy_rscene->addItemModel(DEFin_grpsc, _convexHull_ModelName, new CT_Polygon2D(), tr("Projection au sol"));
    resCpy_rscene->addGroupModel(DEFin_grpsc, _grpSlice_ModelName, new CT_StandardItemGroup(), tr("Groupe"));

    resCpy_rscene->addItemModel(_grpSlice_ModelName, _sclice_ModelName, new CT_Polygon2D(), tr("Projection d'une tranche"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputeCrownProjection::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addBool("Calculer les projections par tranches", "", "", _computeSlices);
    configDialog->addDouble("Espacement des tranches", "m", 0, 1e+09, 2, _spacing, 1);
    configDialog->addDouble("Epaisseur des tranches", "m", 0, 1e+09, 2, _thickness, 1);
}

void PB_StepComputeCrownProjection::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    _rscene = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grpsc(_rscene, this, DEFin_grpsc);
    while (itCpy_grpsc.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grpCpy_grpsc = (CT_StandardItemGroup*) itCpy_grpsc.next();
        
        computeConvexHullForOneSceneGroup(grpCpy_grpsc);
    }
}

void PB_StepComputeCrownProjection::computeConvexHullForOneSceneGroup(CT_StandardItemGroup* group)
{
    const CT_AbstractItemDrawableWithPointCloud* itemCpy_scene = (CT_AbstractItemDrawableWithPointCloud*)group->firstItemByINModelName(this, DEFin_scene);
    if (itemCpy_scene != NULL)
    {


        CT_Polygon2DData *data = new CT_Polygon2DData();
        CT_Polygon2D* itemCpy_convexHull = new CT_Polygon2D(_convexHull_ModelName.completeName(), _rscene, data);
        group->addItemDrawable(itemCpy_convexHull);

        CT_StandardItemGroup* grpSlice= new CT_StandardItemGroup(_grpSlice_ModelName.completeName(), _rscene);
        group->addGroup(grpSlice);

        CT_Polygon2DData *dataSlice = new CT_Polygon2DData();
        CT_Polygon2D* itemCpy_sclice = new CT_Polygon2D(_sclice_ModelName.completeName(), _rscene, dataSlice);
        grpSlice->addItemDrawable(itemCpy_sclice);

    }
}
