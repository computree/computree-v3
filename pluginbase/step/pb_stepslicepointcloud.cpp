#include "pb_stepslicepointcloud.h"

#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "actions/pb_actionslicepointcloud.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include <QMessageBox>

// Alias for indexing models
#define DEFin_resScene "resScene"
#define DEFin_grp "grp"
#define DEFin_scene "scene"

#define DEFout_resScene "resScene"
#define DEFout_slice "slice"
#define DEFout_cluster "cluster"


// Constructor : initialization of parameters
PB_StepSlicePointCloud::PB_StepSlicePointCloud(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _zMin = 0;
    _zMax = 0;
    _thickness = 0.02;
    _spacing = 0.20;
    _manual = true;

    setManual(_manual);

    _m_status = 0;
    _m_doc = NULL;

    _sceneList = new QList<CT_Scene*>();
}

PB_StepSlicePointCloud::~PB_StepSlicePointCloud()
{
    _sceneList->clear();
    delete _sceneList;
}

// Step description (tooltip of contextual menu)
QString PB_StepSlicePointCloud::getStepDescription() const
{
    return tr("Découpe une scène en tranche");
}

// Step detailled description
QString PB_StepSlicePointCloud::getStepDetailledDescription() const
{
    return tr("Action manuelle permettant de découper une scène d'entrée en tranches horizontales.<br>"
              "Il est possible d'en régler intéractivement :<br>"
              "- L'épaisseur (<b>_thickness</b>)<br>"
              "- L'espacement entre deux tranches (<b>_spacing</b>)<br>"
              "<br>"
              "N.B. : Cette étape peut également fonctionner en mode non interactif, avec les paramètres choisis dans la boite de configuration. ");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepSlicePointCloud::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepSlicePointCloud(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepSlicePointCloud::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_resScene = createNewInResultModel(DEFin_resScene, tr("Scène à découper"));
    resIn_resScene->setZeroOrMoreRootGroup();
    resIn_resScene->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_resScene->addItemModel(DEFin_grp, DEFin_scene, CT_Scene::staticGetType(), tr("Scène à découper"));
}

// Creation and affiliation of OUT models
void PB_StepSlicePointCloud::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_resScene = createNewOutResultModel(DEFout_resScene, tr("Scène découpée"));
    res_resScene->setRootGroup(DEFout_slice, new CT_StandardItemGroup(), tr("Tranche"));
    res_resScene->addItemModel(DEFout_slice, DEFout_cluster, new CT_PointCluster(), tr("Points de la tranche"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepSlicePointCloud::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *dialog = newStandardPostConfigurationDialog();

    dialog->addDouble(tr("Epaisseur des tranches :"), tr("cm"), 0.1, 100000, 2, _thickness, 100);
    dialog->addDouble(tr("Espacement des tranches :"), tr("cm"), 0.1, 100000, 2, _spacing, 100);
    dialog->addBool("","",tr("Choix interactif des paramètres"), _manual);
}

void PB_StepSlicePointCloud::compute()
{
    _m_status = 0;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_resScene = inResultList.at(0);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_resScene = outResultList.at(0);

    CT_ResultItemIterator itIn_scene(resIn_resScene, this, DEFin_scene);
    while (itIn_scene.hasNext())
    {
        CT_Scene* itemIn_scene = (CT_Scene*) itIn_scene.next();
        _sceneList->append(itemIn_scene);
    }

    requestManualMode();
    _m_status = 1;

    // Do the slices
    for (int sc = 0 ; sc < _sceneList->size() ; sc++)
    {
        CT_Scene* itemIn_scene = (CT_Scene*) _sceneList->at(sc);

        const CT_AbstractPointCloudIndex *PCI_itemIn_scene = itemIn_scene->getPointCloudIndex();
        size_t nPts_itemIn_scene = PCI_itemIn_scene->size();

        for (size_t i = 0 ; i < nPts_itemIn_scene ; i++)
        {
            size_t index;
            const CT_Point &point = PCI_itemIn_scene->constTAt(i, index);
        }

    }


    // OUT results creation (move it to the appropried place in the code)
//    CT_StandardItemGroup* grp_slice= new CT_StandardItemGroup(DEFout_slice, res_resScene);
//    res_resScene->addGroup(grp_slice);
    
//    CT_PointCluster* item_cluster = new CT_PointCluster(DEFout_cluster, res_resScene);
//    grp_slice->addItemDrawable(item_cluster);

    requestManualMode();
}

void PB_StepSlicePointCloud::initManualMode()
{
    if(_m_doc == NULL)
    {
        // create a new 3D document
        _m_doc = getGuiContext()->documentManager()->new3DDocument();

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        _m_doc->setCurrentAction(new PB_ActionSlicePointCloud(_sceneList), false);
    }

    _m_doc->removeAllItemDrawable();

    QMessageBox::information(NULL,
                             tr("Mode manuel"),
                             tr("Bienvenue dans le mode manuel de cette étape.\n"
                             "Veuillez sélectionner les paramètres pour réaliser les tranches."),
                             QMessageBox::Ok);
}

void PB_StepSlicePointCloud::useManualMode(bool quit)
{
    if(_m_status == 0)
    {
        if(quit)
        {
        }
    }
    else if(_m_status == 1)
    {
        if(!quit)
        {
            _m_doc = NULL;
            quitManualMode();
        }
    }

}
