#include "pb_stepcomputepointmetrics.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_abstractstepplugin.h"
#include "ct_metric/abstract/ct_abstractmetric_xyz.h"
#include "ct_view/tools/ct_manageconfigurableelementsdialog.h"

#include "ct_shape2ddata/ct_circle2ddata.h"
#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_areaShape "shapeArea"


// Constructor : initialization of parameters
PB_StepComputePointMetrics::PB_StepComputePointMetrics(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{

    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();

    int s = pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);
        QList<CT_AbstractMetric*> metrics = p->getMetricsAvailable();

        QListIterator<CT_AbstractMetric*> it(metrics);
        while (it.hasNext())
        {
            CT_AbstractMetric* metric = it.next();

            CT_AbstractMetric_XYZ* pointMetric = dynamic_cast<CT_AbstractMetric_XYZ*>(metric);

            if (pointMetric != NULL)
            {
                _availableMetrics.append(pointMetric);
            }
        }
    }
}

PB_StepComputePointMetrics::~PB_StepComputePointMetrics()
{
    qDeleteAll(_selectedMetrics);
    _selectedMetrics.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepComputePointMetrics::getStepDescription() const
{
    return tr("Calcule des métrics sur les points");
}

// Step detailled description
QString PB_StepComputePointMetrics::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepComputePointMetrics::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputePointMetrics::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputePointMetrics(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputePointMetrics::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Item contenant des points"));
    //resIn_res->addItemModel(DEFin_grp, DEFin_areaShape, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise de la placette"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputePointMetrics::createPostConfigurationDialog()
{
    CT_ManageConfigurableElementsDialog dialog(_availableMetrics, tr("Métriques séléctionnées"));
    dialog.exec();

    qDeleteAll(_selectedMetrics);
    _selectedMetrics.append(dialog.getSeletedElements());
}

// Creation and affiliation of OUT models
void PB_StepComputePointMetrics::createOutResultModelListProtected()
{       
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    resCpy_res->addItemModel(DEFin_grp, _outMetrics_ModelName, new CT_AttributesList(), tr("Métriques calculées"));

    QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
    while (it.hasNext())
    {
        CT_AbstractMetric_XYZ* metric = (CT_AbstractMetric_XYZ*) it.next();
        metric->initAttributesModels(resCpy_res, _outMetrics_ModelName);
    }
}

void PB_StepComputePointMetrics::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* outRes = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(outRes, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* points = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_points);
        //const CT_AbstractAreaShape2D* plotArea = (CT_AbstractAreaShape2D*)grp->firstItemByINModelName(this, DEFin_areaShape);

        if (points != NULL)// && plotArea != NULL)
        {
            CT_AttributesList* outAttributes = new CT_AttributesList(_outMetrics_ModelName.completeName(), outRes);
            grp->addItemDrawable(outAttributes);

            QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
            while (it.hasNext())
            {
                CT_AbstractMetric_XYZ* metric = (CT_AbstractMetric_XYZ*) it.next();

                if (metric != NULL)
                {
                    CT_Circle2DData tmp;
                    metric->initResultAndData(outRes, points->getPointCloudIndex(), tmp);//plotArea->getAreaData());
                    metric->computeMetric(outAttributes);
                }
            }
        }
    }    
}
