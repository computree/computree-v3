#include "steptest.h"

#include "ct_abstractstepplugin.h"
#include "ct_step/ct_stepinitializedata.h"
#include "step/steptestmodelinit1.h"
#include "step/steptestmodel1.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_result/ct_resultgroup.h"

StepTest::StepTest()
{
}

void StepTest::testStep()
{
    TestCore core;
    TestPluginManager pManager;
    pManager.setCoreInterface(&core);

    StepTestModelInit1 *stepTInit = new StepTestModelInit1(*(new CT_StepInitializeData(NULL, pManager, NULL)));
    StepTestModel1 *stepT = new StepTestModel1(*(new CT_StepInitializeData(stepTInit, pManager, NULL)));

    QVERIFY(stepTInit->showPreConfigurationDialog());
    QVERIFY(stepTInit->initInResultModelList());
    QVERIFY(stepTInit->showInputResultConfigurationDialog());
    QVERIFY(stepTInit->showPostConfigurationDialog());
    QVERIFY(stepTInit->initAfterConfiguration());

    QVERIFY(stepT->showPreConfigurationDialog());
    QVERIFY(stepT->initInResultModelList());
    QVERIFY(stepT->showInputResultConfigurationDialog());
    QVERIFY(stepT->showPostConfigurationDialog());
    QVERIFY(stepT->initAfterConfiguration());

    stepTInit->execute();

    // search a out model in a out result normal
    QVERIFY(PS_MODELS->searchModel(DEF_OutSectionGroup, stepTInit->getResult(0), stepTInit) != NULL);
    // search a out model in a out result normal
    QVERIFY(PS_MODELS->searchModel(DEF_OutClusterGroup, stepTInit->getResult(0), stepTInit) != NULL);
    // search a out model in a out result normal
    QVERIFY(PS_MODELS->searchModel(DEF_OutClusterItem, stepTInit->getResult(0), stepTInit) != NULL);

    stepT->execute();

    // search a out model in a out result copy
    QVERIFY(PS_MODELS->searchModel(stepT->_autoSearchOutClusterGroup.completeName(), stepT->getResult(0), stepT) != NULL);

    // search a out model in a out result copy
    QVERIFY(PS_MODELS->searchModel(stepT->_autoSearchOutPointCluster.completeName(), stepT->getResult(0), stepT) != NULL);

    // search a in model in a out result copy
    QVERIFY(PS_MODELS->searchModel(DEF_SearchInSectionGroup, stepT->getResult(0), stepT) != NULL);

    // search a in model in a in result copy
    QVERIFY(PS_MODELS->searchModel(DEF_SearchInSectionGroup, stepTInit->getResult(0), stepT) != NULL);

    // search a in model in a in result copy
    QVERIFY(PS_MODELS->searchModel(DEF_SearchInSectionGroup, stepTInit->getResult(0), stepT) != NULL);
}
