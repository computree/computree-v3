#include "steptestmodel1.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_itemdrawable/ct_pointcluster.h"

#include <QDebug>

StepTestModel1::StepTestModel1(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

CT_VirtualAbstractStep *StepTestModel1::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new StepTestModel1(dataInit);
}

void StepTestModel1::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy * resultModel = createNewInResultModelForCopy(DEF_SearchInResult, tr("Sections / Clusters"));
    resultModel->setZeroOrMoreRootGroup();
    qDebug() << resultModel->addGroupModel("", DEF_SearchInSectionGroup);
    qDebug() << resultModel->addGroupModel(DEF_SearchInSectionGroup, DEF_SearchInClusterGroup);
    qDebug() << resultModel->addItemModel(DEF_SearchInClusterGroup, DEF_SearchInPointCluster, CT_PointCluster::staticGetType());
}

void StepTestModel1::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities* resultCopy = createNewOutResultModelToCopy(DEF_SearchInResult);
    qDebug() << resultCopy->addGroupModel(DEF_SearchInSectionGroup, _autoSearchOutClusterGroup, new CT_StandardItemGroup(), tr("Cluster"));
    qDebug() << resultCopy->addItemModel(_autoSearchOutClusterGroup, _autoSearchOutPointCluster, new CT_PointCluster(), tr("Groupe de points"));
    qDebug() << resultCopy->removeGroupModel(DEF_SearchInClusterGroup);
}

void StepTestModel1::compute()
{

}
