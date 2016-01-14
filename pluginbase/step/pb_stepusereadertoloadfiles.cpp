#include "pb_stepusereadertoloadfiles.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

#include "ct_itemdrawable/ct_readeritem.h"

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_group "group"
#define DEFin_reader "reader"


// Constructor : initialization of parameters
PB_StepUseReaderToLoadFiles::PB_StepUseReaderToLoadFiles(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{

}

PB_StepUseReaderToLoadFiles::~PB_StepUseReaderToLoadFiles()
{
}

// Step description (tooltip of contextual menu)
QString PB_StepUseReaderToLoadFiles::getStepDescription() const
{
    return tr("Charger des fichiers à l'aide de readers");
}

// Step detailled description
QString PB_StepUseReaderToLoadFiles::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepUseReaderToLoadFiles::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepUseReaderToLoadFiles::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepUseReaderToLoadFiles(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepUseReaderToLoadFiles::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *res = createNewInResultModelForCopy(DEFin_res, tr("Fichiers"));
    res->setZeroOrMoreRootGroup();
    res->addGroupModel("", DEFin_group);
    res->addItemModel(DEFin_group, DEFin_reader, CT_ReaderItem::staticGetType(), tr("Reader"));
}


// Creation and affiliation of OUT models
void PB_StepUseReaderToLoadFiles::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_res);


}


void PB_StepUseReaderToLoadFiles::compute()
{


}
