#include "pb_steploadpositionsformatching.h"

#include "ct_itemdrawable/ct_referencepoint.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFout_resultRef "resultRef"
#define DEFout_grpRef "grpRef"
#define DEFout_ref "ref"

#define DEFout_resultTrans "resultTrans"
#define DEFout_grpTrans "grpTrans"
#define DEFout_trans "trans"


// Constructor : initialization of parameters
PB_StepLoadPositionsForMatching::PB_StepLoadPositionsForMatching(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _neededFields << "ID" << "X" << "Y" << tr("VALEUR");

    _refFileName = "";
    _transFileName = "";

    _refHeader = true;
    _transHeader = true;

    _refSeparator = "\t";
    _transSeparator = "\t";

    _refDecimal = ".";
    _transDecimal = ".";

    _refSkip = 0;
    _transSkip = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepLoadPositionsForMatching::getStepDescription() const
{
    return tr("Charge deux fichiers de positions pour mise en correspondance");
}

// Step detailled description
QString PB_StepLoadPositionsForMatching::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepLoadPositionsForMatching::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepLoadPositionsForMatching::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadPositionsForMatching(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepLoadPositionsForMatching::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepLoadPositionsForMatching::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_resultRef = createNewOutResultModel(DEFout_resultRef, tr("Positions de référence"));
    res_resultRef->setRootGroup(DEFout_grpRef, new CT_StandardItemGroup(), tr("Groupe"));
    res_resultRef->addItemModel(DEFout_grpRef, DEFout_ref, new CT_ReferencePoint(), tr("Position de référence"));

    CT_OutResultModelGroup *res_resultTrans = createNewOutResultModel(DEFout_resultTrans, tr("Positions à transformer"));
    res_resultTrans->setRootGroup(DEFout_grpTrans, new CT_StandardItemGroup(), tr("Groupe"));
    res_resultTrans->addItemModel(DEFout_grpTrans, DEFout_trans, new CT_ReferencePoint(), tr("Position à transformer"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepLoadPositionsForMatching::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addAsciiFileChoice("Fichier de positions de référence", "Fichier ASCII (*.txt ; *.asc)", true, _neededFields, _refFileName, _refHeader, _refSeparator, _refDecimal, _refSkip, _refColumns);
    configDialog->addAsciiFileChoice("Fichier de positions à transformer", "Fichier ASCII (*.txt ; *.asc)", true, _neededFields, _transFileName, _transHeader, _transSeparator, _transDecimal, _transSkip, _transColumns);
}

void PB_StepLoadPositionsForMatching::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_resultRef = outResultList.at(0);
    CT_ResultGroup* res_resultTrans = outResultList.at(1);

    // OUT results creation
    CT_StandardItemGroup* grp_grpRef= new CT_StandardItemGroup(DEFout_grpRef, res_resultRef);
    res_resultRef->addGroup(grp_grpRef);
    
    CT_ReferencePoint* item_ref = new CT_ReferencePoint(DEFout_ref, res_resultRef, 0, 0, 0, 0);
    grp_grpRef->addItemDrawable(item_ref);

    CT_StandardItemGroup* grp_grpTrans= new CT_StandardItemGroup(DEFout_grpTrans, res_resultTrans);
    res_resultTrans->addGroup(grp_grpTrans);
    
    CT_ReferencePoint* item_trans = new CT_ReferencePoint(DEFout_trans, res_resultTrans, 0, 0, 0, 0);
    grp_grpTrans->addItemDrawable(item_trans);

}
