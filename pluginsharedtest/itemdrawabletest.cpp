#include "itemdrawabletest.h"

#include "ct_itemdrawable/ct_ttreegroup.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"

ItemDrawableTest::ItemDrawableTest(QObject *parent) :
    QObject(parent)
{
}

void ItemDrawableTest::testTNodeGroup()
{
    CT_OutResultModelGroup *rModel = new CT_OutResultModelGroup("r", NULL);
    CT_OutStdGroupModel *gModel = new CT_OutStdGroupModel("tree", new CT_TTreeGroup());
    CT_OutOPFNodeGroupModel *nModel = new CT_OutOPFNodeGroupModel("root");
    CT_OutOPFNodeGroupModel *sModel = new CT_OutOPFNodeGroupModel("c");

    rModel->setRootGroup(gModel);
    gModel->addGroup(nModel);
    gModel->addGroup(sModel);

    CT_ResultGroup *result = new CT_ResultGroup();
    CT_TTreeGroup *tree = new CT_TTreeGroup(gModel, result);
    CT_TNodeGroup *root = new CT_TNodeGroup(nModel, result);
    CT_TNodeGroup *root2 = new CT_TNodeGroup(nModel, result);
    CT_TNodeGroup *c = new CT_TNodeGroup(sModel, result);
    CT_TNodeGroup *c2 = new CT_TNodeGroup(sModel, result);
    CT_TNodeGroup *c3 = new CT_TNodeGroup(sModel, result);

    QCOMPARE(tree->setRootNode(root), true);
    QCOMPARE(tree->setRootNode(root2), true);
    QCOMPARE(tree->nGroups(), 1);
    QCOMPARE(root2->addComponent(c), true);
    QCOMPARE(root2->addComponent(c2), true);
    QCOMPARE(root2->addComponent(c3), true);
    QCOMPARE(root2->nComponent(), (size_t)3);
    QCOMPARE(root2->removeComponent(c2), true);
    QCOMPARE(root2->nComponent(), (size_t)1);
    QCOMPARE(root2->removeComponent(c), true);
    QCOMPARE(root2->nComponent(), (size_t)0);

    delete tree;
    delete result;
    delete rModel;
}
