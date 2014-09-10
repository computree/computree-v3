#include "steptestmodelinit1.h"

#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_itemdrawable/ct_pointcluster.h"

#include <QDebug>

StepTestModelInit1::StepTestModelInit1(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
}

CT_VirtualAbstractStep* StepTestModelInit1::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new StepTestModelInit1(dataInit);
}

void StepTestModelInit1::createInResultModelListProtected()
{
    setNotNeedInputResult();
}

void StepTestModelInit1::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *rModel = createNewOutResultModel(DEF_OutResult);
    rModel->setRootGroup(DEF_OutSectionGroup);
    qDebug() << rModel->addGroupModel(DEF_OutSectionGroup, DEF_OutClusterGroup);
    qDebug() << rModel->addItemModel(DEF_OutClusterGroup, DEF_OutClusterItem, new CT_PointCluster());
}

void StepTestModelInit1::compute()
{

}
