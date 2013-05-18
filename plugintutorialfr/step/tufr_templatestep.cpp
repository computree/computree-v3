#include "tufr_templatestep.h"

// Inclusion of in and out results models
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

// Inclusion of in and out groups models
#include "ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"

// Inclusion of in and out items models
#include "ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"

// Inclusion of standard result class
#include "ct_result/ct_resultgroup.h"

// Alias for indexing in and out models
#define DEF_SearchInResult  "r"
#define DEF_SearchOutResult "r"

// Constructor : initialization of parameters
TUFR_TemplateStep::TUFR_TemplateStep(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _parametre = 0;
}

// Step description (tooltip of contextual menu)
QString TUFR_TemplateStep::getStepDescription() const
{
    return "Description";
}

// Step copy method
CT_VirtualAbstractStep* TUFR_TemplateStep::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new TUFR_TemplateStep(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void TUFR_TemplateStep::createInResultModelListProtected()
{
}

// Creation and affiliation of OUT models
void TUFR_TemplateStep::createOutResultModelListProtected()
{
}

// Semi-automatic creation of step parameters DialogBox
void TUFR_TemplateStep::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Parameter value", "unit", 0, 1000, 2, _parametre);
}

void TUFR_TemplateStep::compute()
{
    // DONT'T FORGET TO ADD THIS STEP TO THE STEPPLUGINMANAGER !!!!!
}
