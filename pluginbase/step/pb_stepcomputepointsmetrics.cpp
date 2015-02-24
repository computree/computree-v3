#include "pb_stepcomputepointsmetrics.h"

#include "ct_itemdrawable/ct_datasourcegeo.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_iterator/ct_pointiterator.h"

#include "ct_view/ct_stepconfigurabledialog.h"


// Alias for indexing models
#define DEFin_resultScene "resultScene"
#define DEFin_grpScene "grpScene"
#define DEFin_scene "scene"

#define DEFin_resultDataSource "resultDataSource"
#define DEFin_grpDataSource "grpDataSource"
#define DEFin_dataSource "dataSource"

#define DEFout_resultMetrics "resultMetrics"
#define DEFout_grpMetrics "grpMetrics"
#define DEFout_metrics "metrics"


// Constructor : initialization of parameters
PB_StepComputePointsMetrics::PB_StepComputePointsMetrics(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepComputePointsMetrics::getStepDescription() const
{
    return tr("Calcul de métriques à partir de nuages de points");
}

// Step detailled description
QString PB_StepComputePointsMetrics::getStepDetailledDescription() const
{
    return tr("Cette étape gère plusieurs sortes d'éléments :<br>"
              "- Une(des) scène(s) d'entrée<br>"
              "- Un gestionnaire de placettes (enprises dans un fichier, génération auto...)<br>"
              "- Un gestionnaire de Filtres de points (CT_AbstractXYZFilter)<br>"
              "- Un gestionnaire de Métriques de points (CT_AbstractXYZMetric)<br>"
              "<br>"
              "Elle est conçue pour fonctionner au sein d'une boucle de script, et donc garder en mémoire les placettes incomplètes à un tour de boucle. ");
}

// Step URL
QString PB_StepComputePointsMetrics::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputePointsMetrics::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputePointsMetrics(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputePointsMetrics::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_resultDataSource = createNewInResultModel(DEFin_resultDataSource, tr("Source de Donnée géographique"), "", true);
    resIn_resultDataSource->setZeroOrMoreRootGroup();
    resIn_resultDataSource->addGroupModel("", DEFin_grpDataSource, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_resultDataSource->addItemModel(DEFin_grpDataSource, DEFin_dataSource, CT_DataSourceGeo::staticGetType(), tr("Source de données géographique"));

    CT_InResultModelGroup *resIn_resultScene = createNewInResultModel(DEFin_resultScene, tr("Scènes"));
    resIn_resultScene->setZeroOrMoreRootGroup();
    resIn_resultScene->addGroupModel("", DEFin_grpScene, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_resultScene->addItemModel(DEFin_grpScene, DEFin_scene, CT_Scene::staticGetType(), tr("Scène"));


}

// Creation and affiliation of OUT models
void PB_StepComputePointsMetrics::createOutResultModelListProtected()
{
//    CT_OutResultModelGroup *res_resultMetrics = createNewOutResultModel(DEFout_resultMetrics, tr("Métriques"));
//    res_resultMetrics->setRootGroup(DEFout_grpMetrics, new CT_StandardItemGroup(), tr("Groupe"));
//    res_resultMetrics->addItemModel(DEFout_grpMetrics, DEFout_metrics, new CT_Beam(), tr("Métriques"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputePointsMetrics::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addTitle(tr("<b>1. Choix des placettes de calcul</b>"));
    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>2. Choix des filtres</b>"));
    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>3. Choix des métriques</b>"));
    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>4. Choix des combinaisons filtres / métriques</b>"));
    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>Gestionnaire de configurations</b>"));

}

void PB_StepComputePointsMetrics::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_resultDataSource = inResultList.at(0);
    CT_ResultGroup* resIn_resultScene = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_resultMetrics = outResultList.at(0);

    CT_ResultItemIterator itIn_dataSource(resIn_resultDataSource, this, DEFin_dataSource);
    if (itIn_dataSource.hasNext())
    {
        const CT_DataSourceGeo* itemIn_dataSource = (CT_DataSourceGeo*)itIn_dataSource.next();
        if (itemIn_dataSource != NULL)
        {

            // TODO

            CT_ResultItemIterator itIn_scene(resIn_resultScene, this, DEFin_scene);
            while (itIn_scene.hasNext() && !isStopped())
            {
                const CT_Scene* itemIn_scene = (CT_Scene*)itIn_scene.next();
                if (itemIn_scene != NULL)
                {
                    CT_PointIterator itP(itemIn_scene->getPointCloudIndex());
                    while (itP.hasNext())
                    {
                        const CT_Point &point = itP.next().currentPoint();
                        // TODO

                    }
                }
            }
        }
    }


    // OUT results creation (move it to the appropried place in the code)
//    CT_StandardItemGroup* grp_grpMetrics= new CT_StandardItemGroup(DEFout_grpMetrics, res_resultMetrics);
//    res_resultMetrics->addGroup(grp_grpMetrics);
    
//    CT_Beam* item_metrics = new CT_Beam(DEFout_metrics, res_resultMetrics);
//    grp_grpMetrics->addItemDrawable(item_metrics);



}
