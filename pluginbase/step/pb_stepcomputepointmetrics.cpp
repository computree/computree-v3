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
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_global/ct_context.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_abstractstepplugin.h"
#include "ctlibmetrics/ct_metric/abstract/ct_abstractmetric_xyz.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_areaShape "shapeArea"


// Constructor : initialization of parameters
PB_StepComputePointMetrics::PB_StepComputePointMetrics(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _inSceneDisplayableName = "";

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
                _availableMetrics.append(pointMetric);
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
    return tr("Métriques de points (XYZ)");
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

bool PB_StepComputePointMetrics::setAllSettings(const SettingsNodeGroup *settings)
{
    qDeleteAll(_selectedMetrics);
    _selectedMetrics.clear();

    bool ok = CT_AbstractStep::setAllSettings(settings);

    if(ok) {
        SettingsNodeGroup* group = settings->firstGroupByTagName("PB_StepComputePointMetrics");

        if(group == NULL)
            return false;

        QList<SettingsNodeGroup*> groups = group->groupsByTagName("Metric");

        QListIterator<SettingsNodeGroup*> it(groups);

        while(it.hasNext()) {
            SettingsNodeGroup *groupM = it.next();

            SettingsNodeValue* value = groupM->firstValueByTagName("className");

            if(value == NULL)
                return false;

            QString metricUniqueName = value->value().toString();

            CT_AbstractConfigurableElement *metricFound = NULL;

            QListIterator<CT_AbstractConfigurableElement*> itM(_availableMetrics);

            while(itM.hasNext() && (metricFound == NULL)) {
                CT_AbstractConfigurableElement *metric = itM.next();

                if(metric->getUniqueName() == metricUniqueName)
                    metricFound = metric;
            }

            if(metricFound == NULL)
                return false;

            CT_AbstractConfigurableElement *metricCpy = metricFound->copy();

            SettingsNodeGroup *groupS = NULL;

            if(!groupM->groups().isEmpty())
                groupS = groupM->groups().first();

            if(!metricCpy->setAllSettings(groupS)) {
                delete metricCpy;
                return false;
            }

            _selectedMetrics.append(metricCpy);
        }
    }

    return ok;
}

SettingsNodeGroup *PB_StepComputePointMetrics::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStep::getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepComputePointMetrics");

    QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);

    while(it.hasNext()) {
        SettingsNodeGroup *groupM = new SettingsNodeGroup("Metric");

        CT_AbstractConfigurableElement *metric = it.next();

        groupM->addValue(new SettingsNodeValue("className", metric->getUniqueName()));

        SettingsNodeGroup *childM = metric->getAllSettings();

        if(childM != NULL)
            groupM->addGroup(childM);

        group->addGroup(groupM);
    }

    root->addGroup(group);

    return root;
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
    resIn_res->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));
    resIn_res->addItemModel(DEFin_grp, DEFin_areaShape, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise de la placette"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsOptional);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputePointMetrics::createPostConfigurationDialog()
{
    // Récupération du nom du modèle de la scène d'entrée
    CT_InAbstractResultModel *resultInModel = getInResultModel(DEFin_res);

    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractSingularItemModel* baseModel = NULL;

    if(!resultInModel->getPossibilitiesSavedSelected().isEmpty())
    {
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();
    }

    if(resultInModelOut != NULL)
    {
        baseModel = (CT_InAbstractSingularItemModel*)PS_MODELS->searchModel(DEFin_points, resultInModelOut, this);
    }

    if(baseModel != NULL && !baseModel->getPossibilitiesSavedSelected().isEmpty())
    {
        _inSceneDisplayableName =  baseModel->getPossibilitiesSavedSelected().first()->outModel()->displayableName();
    }
}

bool PB_StepComputePointMetrics::postConfigure()
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
void PB_StepComputePointMetrics::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy != NULL) {
        resCpy->addItemModel(DEFin_grp, _outMetrics_ModelName, new CT_AttributesList(), tr("Métriques") + (_inSceneDisplayableName.isEmpty() ? "" : tr(" (%1)").arg(_inSceneDisplayableName)));

        QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
        while (it.hasNext())
        {
            CT_AbstractMetric_XYZ* metric = dynamic_cast<CT_AbstractMetric_XYZ*>(it.next());
            metric->postConfigure();
            metric->initAttributesModels(resCpy, _outMetrics_ModelName);
        }
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
        const CT_AbstractAreaShape2D* plotArea = (CT_AbstractAreaShape2D*)grp->firstItemByINModelName(this, DEFin_areaShape);

        if (points != NULL)
        {
            const CT_AreaShape2DData *area = NULL;

            if(plotArea != NULL)
                area = &plotArea->getAreaData();

            CT_AttributesList* outAttributes = new CT_AttributesList(_outMetrics_ModelName.completeName(), outRes);
            grp->addItemDrawable(outAttributes);

            QListIterator<CT_AbstractConfigurableElement *> it(_selectedMetrics);
            while (it.hasNext())
            {
                CT_AbstractMetric_XYZ* metric = (CT_AbstractMetric_XYZ*) it.next();

                if (metric != NULL)
                {
                    metric->initDatas(points->getPointCloudIndex(), area);
                    metric->computeMetric(outAttributes);
                }
            }
        }
    }    
}
