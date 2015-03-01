#include "pb_stepcreateplotmanagerfromfile.h"

#include "ct_itemdrawable/ct_plotmanageritem.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_plotmanager/ct_plotmanager_plotslist.h"

// Alias for indexing models
#define DEFout_result "result"
#define DEFout_grp "grp"
#define DEFout_plotmanageritem "plotmanageritem"


// Constructor : initialization of parameters
PB_StepCreatePlotManagerFromFile::PB_StepCreatePlotManagerFromFile(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepCreatePlotManagerFromFile::getStepDescription() const
{
    return tr("Créée un gestionnaire de placettes à partir d'un fichier ASCII");
}

// Step detailled description
QString PB_StepCreatePlotManagerFromFile::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepCreatePlotManagerFromFile::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCreatePlotManagerFromFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCreatePlotManagerFromFile(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCreatePlotManagerFromFile::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepCreatePlotManagerFromFile::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_result = createNewOutResultModel(DEFout_result, tr("Gestionnaire de placette (Plot List)"));
    res_result->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_result->addItemModel(DEFout_grp, DEFout_plotmanageritem, new CT_PlotManagerItem(), tr("Gestionnaire de placettes (Plot List)"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepCreatePlotManagerFromFile::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addFileChoice("Fichier de placettes", CT_FileChoiceButton::OneExistingFile, "Fichier ascii (*.txt, *.asc)", _fileName);
}

void PB_StepCreatePlotManagerFromFile::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_result = outResultList.at(0);

    CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_result);
    res_result->addGroup(grp_grp);
    
    CT_PlotManagerItem* item_plotmanageritem = new CT_PlotManagerItem(DEFout_plotmanageritem, res_result, new CT_PlotManager_plotsList());
    grp_grp->addItemDrawable(item_plotmanageritem);

}
