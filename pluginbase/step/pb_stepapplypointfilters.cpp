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

#include "ct_abstractstepplugin.h"
#include "ct_filter/abstract/ct_abstractfilter_xyz.h"
#include "ct_filter/abstract/ct_abstractfilter_las.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_lasAtt "lasAtt"


// Constructor : initialization of parameters
PB_StepApplyPointFilters::PB_StepApplyPointFilters(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{

    PluginManagerInterface *pm = PS_CONTEXT->pluginManager();

    int s = pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *p = pm->getPlugin(i);
        QList<CT_AbstractFilter*> filters = p->getFiltersAvailable();

        QListIterator<CT_AbstractFilter*> it(filters);
        while (it.hasNext())
        {
            CT_AbstractFilter* filter = it.next();

            CT_AbstractFilter_XYZ* pointFilter = dynamic_cast<CT_AbstractFilter_XYZ*>(filter);

            if (pointFilter != NULL)
            {
                _availableFilters.append(pointFilter);
            }
        }
    }
}

PB_StepApplyPointFilters::~PB_StepApplyPointFilters()
{
    qDeleteAll(_modelNames.values());
    _modelNames.clear();

    qDeleteAll(_selectedFilters);
    _selectedFilters.clear();
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

// Semi-automatic creation of step parameters DialogBox
void PB_StepApplyPointFilters::createPostConfigurationDialog()
{
    _configDialog = new CT_ManageConfigurableElementsDialog(tr("Filtres séléctionnés"), _availableFilters, &_selectedFilters);
}

bool PB_StepApplyPointFilters::postConfigure()
{
    if(_configDialog != NULL)
    {
        if(_configDialog->exec() == 1)
        {
            setSettingsModified(true);
            return true;
        }
        return false;
    }
    return true;
}

SettingsNodeGroup* PB_StepApplyPointFilters::getAllSettings() const
{
    SettingsNodeGroup *root = CT_VirtualAbstractStep::getAllSettings();
    SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepApplyPointFilters");
    group->addValue(new SettingsNodeValue("Version", "1"));

    if(_configDialog != NULL)
    {
        SettingsNodeGroup *postC = new SettingsNodeGroup("ConfigDialog");
        postC->addValue(new SettingsNodeValue("Config", _configDialog->getConfig()));
        group->addGroup(postC);
    }

    root->addGroup(group);
    return root;
}

bool PB_StepApplyPointFilters::setAllSettings(const SettingsNodeGroup *settings)
{
    bool ok = CT_VirtualAbstractStep::setAllSettings(settings);

    if(ok && _configDialog != NULL)
    {
        QString config = "";

        QList<SettingsNodeGroup*> groups = settings->groupsByTagName("PB_StepApplyPointFilters");
        if(groups.isEmpty()) {return false;}

        SettingsNodeGroup *rootStep = groups.first();

        QList<SettingsNodeGroup*> groups2 = rootStep->groupsByTagName("ConfigDialog");
        if(groups2.isEmpty()) {return false;}

        SettingsNodeGroup *rootConfig = groups2.first();

        QList<SettingsNodeValue*> values = rootConfig->valuesByTagName("Config");

        if(!values.isEmpty())
        {
            config = values.first()->value().toString();
        }

        QString errors = _configDialog->setConfig(config);

        if (!errors.isEmpty())
        {
            PS_LOG->addMessage(LogInterface::error, LogInterface::action, errors);
        }
    }

    return ok;
}


// Creation and affiliation of OUT models
void PB_StepApplyPointFilters::createOutResultModelListProtected()
{       
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    QListIterator<CT_AbstractConfigurableElement *> it(_selectedFilters);
    while (it.hasNext())
    {
        CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) it.next();
        CT_AutoRenameModels* modelName = new CT_AutoRenameModels();
        _modelNames.insert(filter, modelName);

        resCpy_res->addItemModel(DEFin_grp, *modelName, new CT_Scene(), filter->getCompleteName());
    }
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
            QListIterator<CT_AbstractConfigurableElement *> it(_selectedFilters);
            while (it.hasNext())
            {
                CT_AbstractConfigurableElement* element = it.next();
                CT_AutoRenameModels* modelName = _modelNames.value(element);
                CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) element->copy();
                CT_AbstractFilter_LAS* filterLAS = dynamic_cast<CT_AbstractFilter_LAS*>(filter);

                if (filter != NULL && modelName != NULL && points->getPointCloudIndex() != NULL)
                {
                    filter->setPointCloudIndex(points);

                    if (filterLAS != NULL)
                    {
                        ((CT_AbstractFilter_LAS*) filter)->setLASAttributesContainer(lasAtt);
                    }

                    CT_PointCloudIndexVector* outCloud = filter->filterPointCloudIndex();

                    if (outCloud->size() > 0)
                    {
                        CT_Scene* outScene = new CT_Scene(modelName->completeName(), _res);
                        outScene->updateBoundingBox();
                        grp->addItemDrawable(outScene);

                        // The PCI must be registered to the point repository :
                        outScene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(outCloud));

                    } else {delete outCloud;}

                    delete filter;
                }
            }
        }
    }    
}
