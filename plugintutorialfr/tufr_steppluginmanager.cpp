#include "tufr_steppluginmanager.h"

#include "ct_stepseparator.h"
#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"

//#include "step/TUFR_stepcalculateplotindicator.h"

TUFR_StepPluginManager::TUFR_StepPluginManager() : CT_AbstractStepPlugin()
{
}

TUFR_StepPluginManager::~TUFR_StepPluginManager()
{
}

bool TUFR_StepPluginManager::loadGenericsStep()
{
    CT_StepSeparator *sep = addNewSeparator(new CT_StepSeparator());
//    sep->addStep(new TUFR_StepCalculatePlotIndicator(*createNewStepInitializeData(NULL)));

    return true;
}

bool TUFR_StepPluginManager::loadOpenFileStep()
{
    return true;
}

bool TUFR_StepPluginManager::loadCanBeAddedFirstStep()
{
//    CT_StepCanBeAddedFirstSeparator *sep = addNewSeparator(new CT_StepCanBeAddedFirstSeparator());
//    sep->addStep(new TUFR_StepLoadFieldInventory(*createNewStepInitializeData(NULL)));

    return true;
}
