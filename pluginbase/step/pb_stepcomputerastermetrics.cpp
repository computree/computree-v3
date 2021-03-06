#include "pb_stepcomputerastermetrics.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractimage2d.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_abstractstepplugin.h"

#include "ctlibmetrics/ct_metric/abstract/ct_abstractmetric_raster.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_raster "raster"
#define DEFin_areaShape "shapeArea"


// Constructor : initialization of parameters
PB_StepComputeRasterMetrics::PB_StepComputeRasterMetrics(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

PB_StepComputeRasterMetrics::~PB_StepComputeRasterMetrics()
{
    qDeleteAll(_selectedMetrics);
    _selectedMetrics.clear();
}

void PB_StepComputeRasterMetrics::init()
{
    CT_AbstractStep::init();

    _availableMetrics.clear();

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

            CT_AbstractMetric_Raster* pointMetric = dynamic_cast<CT_AbstractMetric_Raster*>(metric);

            if (pointMetric != NULL)
                _availableMetrics.append(pointMetric);
        }
    }
}

// Step description (tooltip of contextual menu)
QString PB_StepComputeRasterMetrics::getStepDescription() const
{
    return tr("Métriques de rasters");
}

// Step detailled description
QString PB_StepComputeRasterMetrics::getStepDetailledDescription() const
{
    QString ret;

    QListIterator<CT_AbstractConfigurableElement*> it(_availableMetrics);

    while(it.hasNext()) {
        CT_AbstractConfigurableElement *ce = it.next();

        ret += tr("<b>%1</b><br/><br/><i>%2</i>").arg(ce->getShortDisplayableName()).arg(ce->getShortDescription());

        if(it.hasNext())
            ret += "<br/><br/>";
    }

    if(!ret.isEmpty())
        return ret;

    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepComputeRasterMetrics::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputeRasterMetrics::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputeRasterMetrics(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputeRasterMetrics::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_raster, CT_AbstractImage2D::staticGetType(), tr("Raster"));
    resIn_res->addItemModel(DEFin_grp, DEFin_areaShape, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise de la placette"));
}

bool PB_StepComputeRasterMetrics::postConfigure()
{    
    CTG_ConfigurableElementsSelector cd(NULL, !getStepChildList().isEmpty());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(_availableMetrics);
    cd.setElementsSelected(&_selectedMetrics);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}


// Creation and affiliation of OUT models
void PB_StepComputeRasterMetrics::createOutResultModelListProtected()
{       
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy_res != NULL) {
        resCpy_res->addItemModel(DEFin_grp, _outMetrics_ModelName, new CT_AttributesList(), tr("Métriques calculées"));

        QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
        while (it.hasNext())
        {
            CT_AbstractMetric_Raster* metric = (CT_AbstractMetric_Raster*) it.next();
            metric->initAttributesModels(resCpy_res, _outMetrics_ModelName);
        }
    }
}

void PB_StepComputeRasterMetrics::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* outRes = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(outRes, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractImage2D* raster = (CT_AbstractImage2D*)grp->firstItemByINModelName(this, DEFin_raster);
        const CT_AbstractAreaShape2D* plotArea = (CT_AbstractAreaShape2D*)grp->firstItemByINModelName(this, DEFin_areaShape);

        if (raster != NULL && plotArea != NULL)
        {
            CT_AttributesList* outAttributes = new CT_AttributesList(_outMetrics_ModelName.completeName(), outRes);
            grp->addItemDrawable(outAttributes);

            QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
            while (it.hasNext())
            {
                CT_AbstractMetric_Raster* metric = (CT_AbstractMetric_Raster*) it.next();

                if (metric != NULL)
                {
                    metric->initDatas(raster, plotArea->getAreaData());
                    metric->computeMetric(outAttributes);
                }
            }
        }
    }    
}
