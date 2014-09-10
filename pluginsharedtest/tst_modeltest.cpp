#include "tst_modeltest.h"


#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include "ct_itemdrawable/ct_circle.h"
#include "ct_itemdrawable/ct_ellipse.h"
#include "ct_itemdrawable/ct_topfnodegroup.h"

#include <QDebug>

ModelTest::ModelTest()
{
}

void ModelTest::testCaseInSingularItemModelCreation()
{
    CT_InStdSingularItemModel *singularModel = new CT_InStdSingularItemModel("UN", CT_AbstractItemDrawable::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsOptional);

    QCOMPARE(singularModel->uniqueName(),QString("UN"));
    QCOMPARE(singularModel->uniqueNamePlusTurn(),QString("UN"));
    QCOMPARE(singularModel->itemType(),CT_AbstractItemDrawable::staticGetType());
    QCOMPARE(singularModel->displayableName(),QString("DI"));
    QCOMPARE(singularModel->description(),QString("DE"));
    QCOMPARE(singularModel->choiceMode(),CT_InStdSingularItemModel::C_ChooseOneIfMultiple);
    QCOMPARE(singularModel->finderMode(),CT_InStdSingularItemModel::F_IsOptional);
    QVERIFY(singularModel->parentModel() == NULL);
    QCOMPARE(singularModel->rootModel(),singularModel);
    QVERIFY(singularModel->step() == NULL);

    delete singularModel;
}

void ModelTest::testCaseInGroupModelCreation()
{
    CT_InStdGroupModel *groupModel = new CT_InStdGroupModel("UNG", CT_TOPFNodeGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory);

    QVERIFY2(groupModel->uniqueName() == "UNG", "uniqueName not valid");
    QVERIFY2(groupModel->uniqueNamePlusTurn() == "UNG", "uniqueNamePlusTurn not valid");
    QVERIFY2(groupModel->groupType() == CT_TOPFNodeGroup::staticGetType(), qPrintable("groupType not valid : " + groupModel->groupType()));
    QVERIFY2(groupModel->displayableName() == "DIG", "displayableName not valid");
    QVERIFY2(groupModel->description() == "DEG", "description not valid");
    QVERIFY2(groupModel->choiceMode() == CT_InAbstractGroupModel::C_ChooseOneIfMultiple, "choiceMode not valid");
    QVERIFY2(groupModel->finderMode() == CT_InAbstractGroupModel::F_IsObligatory, "finderMode not valid");
    QVERIFY2(groupModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(groupModel->rootModel() == groupModel, "rootModel not valid");
    QVERIFY2(groupModel->step() == NULL, "step not valid");

    delete groupModel;
}

void ModelTest::testCaseInZeroOrMoreGroupModelCreation()
{
    CT_InZeroOrMoreGroupModel *groupModel = new CT_InZeroOrMoreGroupModel();

    QVERIFY2(!groupModel->uniqueName().isEmpty(), "uniqueName empty");
    QVERIFY2(!groupModel->uniqueNamePlusTurn().isEmpty(), "uniqueNamePlusTurn empty");
    QVERIFY2(groupModel->groupType() == CT_AbstractItemGroup::staticGetType(), qPrintable("groupType not valid : " + groupModel->groupType()));
    QVERIFY2(!groupModel->displayableName().isEmpty(), "displayableName empty");
    QVERIFY2(!groupModel->description().isEmpty(), "description empty");
    QVERIFY2(groupModel->choiceMode() == CT_InAbstractModel::C_DontChoose, "choiceMode not C_DontChoose");
    QVERIFY2(groupModel->finderMode() == CT_InAbstractModel::F_IsObligatory, "finderMode not F_IsObligatory");
    QVERIFY2(groupModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(groupModel->rootModel() == groupModel, "rootModel not valid");
    QVERIFY2(groupModel->step() == NULL, "step not valid");

    delete groupModel;
}

void ModelTest::testCaseInResultGroupModelCreation()
{
    CT_InResultModelGroup *rModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    rModel->setTurnIndex(1);

    QVERIFY2(rModel->uniqueName() == "UNR", "uniqueName not valid");
    QVERIFY2(rModel->uniqueNamePlusTurn() != rModel->uniqueName(), "uniqueNamePlusTurn not valid");
    QVERIFY2(rModel->turnIndex() == 1, "turnIndex not valid");
    QVERIFY2(rModel->displayableName() == "DIR", "displayableName not valid");
    QVERIFY2(rModel->description() == "DER", "description not valid");
    QVERIFY2(rModel->isRecursive(), "isRecursive not valid");
    QVERIFY2(rModel->rootGroup() == NULL, "rootGroup not valid");
    QVERIFY2(rModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(rModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(rModel->step() == NULL, "step not valid");

    delete rModel;
}

void ModelTest::testCaseInTreeCreation()
{
    CT_InResultModelGroup *rModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(rModel->setZeroOrMoreRootGroup());
    QVERIFY(rModel->addGroupModel("", "UNG", CT_TOPFNodeGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(rModel->addItemModel("UNG", "UNI", CT_AbstractItemDrawable::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsOptional));

    CT_InAbstractGroupModel *gModel = (CT_InAbstractGroupModel*)rModel->findModelInTree("UNG");
    QVERIFY(gModel != NULL);

    QVERIFY2(gModel->uniqueName() == "UNG", "uniqueName not valid");
    QVERIFY2(gModel->uniqueNamePlusTurn() == "UNG", "uniqueNamePlusTurn not valid");
    QVERIFY2(gModel->groupType() == CT_TOPFNodeGroup::staticGetType(), qPrintable("groupType not valid : " + gModel->groupType()));
    QVERIFY2(gModel->displayableName() == "DIG", "displayableName not valid");
    QVERIFY2(gModel->description() == "DEG", "description not valid");
    QVERIFY2(gModel->choiceMode() == CT_InAbstractGroupModel::C_ChooseOneIfMultiple, "choiceMode not valid");
    QVERIFY2(gModel->finderMode() == CT_InAbstractGroupModel::F_IsObligatory, "finderMode not valid");
    QVERIFY2(gModel->parentModel() == rModel->rootGroup(), "parentModel not valid");
    QVERIFY2(gModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(gModel->step() == NULL, "step not valid");

    CT_InAbstractSingularItemModel *siModel = (CT_InAbstractSingularItemModel*)rModel->findModelInTree("UNI");
    QVERIFY(siModel != NULL);

    QVERIFY2(siModel->uniqueName() == "UNI", "uniqueName not valid");
    QVERIFY2(siModel->uniqueNamePlusTurn() == "UNI", "uniqueNamePlusTurn not valid");
    QVERIFY2(siModel->itemType() == CT_AbstractItemDrawable::staticGetType(), "itemType not valid");
    QVERIFY2(siModel->displayableName() == "DI", "displayableName not valid");
    QVERIFY2(siModel->description() == "DE", "description not valid");
    QVERIFY2(siModel->choiceMode() == CT_InStdSingularItemModel::C_ChooseOneIfMultiple, "choiceMode not valid");
    QVERIFY2(siModel->finderMode() == CT_InStdSingularItemModel::F_IsOptional, "finderMode not valid");
    QVERIFY2(siModel->parentModel() == gModel, "parentModel not valid");
    QVERIFY2(siModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(siModel->step() == NULL, "step not valid");

    QVERIFY2(rModel->rootGroup()->parentModel() == rModel, "rootGroup->parentModel() not valid");

    QVERIFY(rModel->addItemModel("", "UNI", CT_AbstractItemDrawable::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsOptional) == false);

    delete rModel;
}

void ModelTest::testCaseOutSingularItemModelCreation()
{
    CT_OutStdSingularItemModel *singularModel = new CT_OutStdSingularItemModel("UN", new CT_Circle(), "DI", "DE");

    QVERIFY2(singularModel->uniqueName() == "UN", "uniqueName not valid");
    QVERIFY2(singularModel->uniqueNamePlusTurn() == "UN", "uniqueNamePlusTurn not valid");
    QVERIFY2(singularModel->itemDrawable() != NULL, "item == NULL");
    QVERIFY2(singularModel->displayableName() == "DI", "displayableName not valid");
    QVERIFY2(singularModel->description() == "DE", "description not valid");
    QVERIFY2(singularModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(singularModel->rootModel() == singularModel, "rootModel not valid");
    QVERIFY2(singularModel->step() == NULL, "step not valid");

    delete singularModel;
}

void ModelTest::testCaseOutGroupModelCreation()
{
    CT_OutStdGroupModel *groupModel = new CT_OutStdGroupModel("UNG", new CT_TOPFNodeGroup(), "DIG", "DEG");

    QVERIFY2(groupModel->uniqueName() == "UNG", "uniqueName not valid");
    QVERIFY2(groupModel->uniqueNamePlusTurn() == "UNG", "uniqueNamePlusTurn not valid");
    QVERIFY2(groupModel->group() != NULL, "groupType == NULL ");
    QVERIFY2(groupModel->displayableName() == "DIG", "displayableName not valid");
    QVERIFY2(groupModel->description() == "DEG", "description not valid");
    QVERIFY2(groupModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(groupModel->rootModel() == groupModel, "rootModel not valid");
    QVERIFY2(groupModel->step() == NULL, "step not valid");

    delete groupModel;
}

void ModelTest::testCaseOutOPFNodeGroupModelCreation()
{
    CT_OutOPFNodeGroupModel *groupModel = new CT_OutOPFNodeGroupModel("UNG", new CT_TOPFNodeGroup(), "DIG", "DEG");

    QVERIFY2(groupModel->uniqueName() == "UNG", "uniqueName not valid");
    QVERIFY2(groupModel->uniqueNamePlusTurn() == "UNG", "uniqueNamePlusTurn not valid");
    QVERIFY2(groupModel->group() != NULL, "groupType == NULL ");
    QVERIFY2(groupModel->displayableName() == "DIG", "displayableName not valid");
    QVERIFY2(groupModel->description() == "DEG", "description not valid");
    QVERIFY2(groupModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(groupModel->rootModel() == groupModel, "rootModel not valid");
    QVERIFY2(groupModel->step() == NULL, "step not valid");

    delete groupModel;
}

void ModelTest::testCaseOutResultGroupModelCreation()
{
    CT_OutResultModelGroup *rModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    rModel->setTurnIndex(1);

    QVERIFY2(rModel->uniqueName() == "UNR", "uniqueName not valid");
    QVERIFY2(rModel->uniqueNamePlusTurn() != rModel->uniqueName(), "uniqueNamePlusTurn not valid");
    QVERIFY2(rModel->turnIndex() == 1, "turnIndex not valid");
    QVERIFY2(rModel->resultName() == "RN", "resultName not valid");
    QVERIFY2(rModel->displayableName() == "DIR", qPrintable("displayableName not valid : " + rModel->displayableName()));
    QVERIFY2(rModel->description() == "DER", "description not valid");
    QVERIFY2(rModel->rootGroup() == NULL, "rootGroup not valid");
    QVERIFY2(rModel->parentModel() == NULL, "parentModel not valid");
    QVERIFY2(rModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(rModel->step() == NULL, "step not valid");

    rModel->recursiveSetComplete();
    delete rModel;
}

void ModelTest::testCaseOutTreeCreation()
{
    CT_OutResultModelGroup *rModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        rModel->setRootGroup("UNG", new CT_TOPFNodeGroup(), "DIG", "DEG");
            QVERIFY(rModel->addItemModel("UNG", "UNI", new CT_Circle(), "DI", "DE"));

    CT_OutAbstractGroupModel *gModel = (CT_OutAbstractGroupModel*)rModel->findModelInTree("UNG");
    QVERIFY(gModel != NULL);

    QVERIFY2(gModel->uniqueName() == "UNG", "uniqueName not valid");
    QVERIFY2(gModel->uniqueNamePlusTurn() == "UNG", "uniqueNamePlusTurn not valid");
    QVERIFY2(gModel->itemDrawable()->getType() == CT_TOPFNodeGroup::staticGetType(), qPrintable("groupType not valid : " + gModel->itemDrawable()->getType()));
    QVERIFY2(gModel->displayableName() == "DIG", "displayableName not valid");
    QVERIFY2(gModel->description() == "DEG", "description not valid");
    QVERIFY2(gModel->parentModel() == rModel, "parentModel not valid");
    QVERIFY2(gModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(gModel->step() == NULL, "step not valid");

    CT_OutAbstractSingularItemModel *siModel = (CT_OutAbstractSingularItemModel*)rModel->findModelInTree("UNI");
    QVERIFY(siModel != NULL);

    QVERIFY2(siModel->uniqueName() == "UNI", "uniqueName not valid");
    QVERIFY2(siModel->uniqueNamePlusTurn() == "UNI", "uniqueNamePlusTurn not valid");
    QVERIFY2(siModel->itemDrawable()->getType() == CT_Circle::staticGetType(), "itemType not valid");
    QVERIFY2(siModel->displayableName() == "DI", "displayableName not valid");
    QVERIFY2(siModel->description() == "DE", "description not valid");
    QVERIFY2(siModel->parentModel() == gModel, "parentModel not valid");
    QVERIFY2(siModel->rootModel() == rModel, "rootModel not valid");
    QVERIFY2(siModel->step() == NULL, "step not valid");

    QVERIFY(rModel->addItemModel("UNG", "UNI", new CT_Circle(), "DI", "DE") == false);

    rModel->recursiveSetComplete();
    delete rModel;
}

void ModelTest::testCaseTreeSearchSimple1()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
            QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG2", "UNI", new CT_Circle(), "DI", "DE"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, false));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 0);
    QCOMPARE(iRModel->rootGroup()->nPossibilitiesSaved(), 0);

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 2);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 1);
    iRModel->getPossibilitiesSaved().first()->setSelected(true);
    ((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->getPossibilitiesSaved().first()->setSelected(true);
    ((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->getPossibilitiesSaved().first()->setSelected(true);
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple2()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
            QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG2", "UNI", new CT_Circle(), "DI", "DE"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addItemModel("", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, false));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 0);
    QCOMPARE(iRModel->rootGroup()->nPossibilitiesSaved(), 0);

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 1);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple3()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
                QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 0);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());


    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple4()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("OUNR", NULL);
    oRModel->setRootGroup("OUNG");
        QVERIFY(oRModel->addGroupModel("OUNG", "OUNG2"));
            QVERIFY(oRModel->addItemModel("OUNG2", "UNI1", new CT_Circle()));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("IUNR", NULL);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "IUNG"));
                QVERIFY(iRModel->addGroupModel("IUNG", "IUNG2"));
                    QVERIFY(iRModel->addItemModel("IUNG2", "IUNI", CT_Circle::staticGetType()));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "IUNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "IUNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "IUNG2"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 0);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());


    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple1Copy()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroupToCopy *iRModel = new CT_InResultModelGroupToCopy("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 0);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple1ItemAttribute()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
            QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));
                QVERIFY(iRModel->addItemAttributeModel("UNI", "UNIA", QList<QString>() << CT_AbstractCategory::DATA_RADIUS, CT_AbstractCategory::NUMBER));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNIA"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 0);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple2ItemAttribute()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
            QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));
                QVERIFY(iRModel->addItemAttributeModel("UNI", "UNIA", QList<QString>() << CT_AbstractCategory::DATA_ANGLE, CT_AbstractCategory::NUMBER));

    QCOMPARE(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true), false);

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple3ItemAttribute()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
            QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));
                QVERIFY(iRModel->addItemAttributeModel("UNI", "UNIA", QList<QString>() << CT_AbstractCategory::DATA_RADIUS, CT_AbstractCategory::STRING));

    QCOMPARE(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true), false);

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple4ItemAttribute()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addItemModel("UNG", "UNI1", new CT_Circle(), "DI1", "DE1"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
        QVERIFY(iRModel->setZeroOrMoreRootGroup());
            QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
            QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));
                QVERIFY(iRModel->addItemAttributeModel("UNI", "UNIA", QList<QString>() << CT_AbstractCategory::DATA_RATIO << CT_AbstractCategory::DATA_RADIUS, CT_AbstractCategory::NUMBER));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNIA"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 0);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchComplex()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
            QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG2", "UNI", new CT_Circle(), "DI", "DE"));
                QVERIFY(oRModel->addGroupModel("UNG2", "UNG3", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                    QVERIFY(oRModel->addItemModel("UNG3", "UNI2", new CT_Circle(), "DI", "DE"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, false));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 0);
    QCOMPARE(iRModel->rootGroup()->nPossibilitiesSaved(), 0);

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 2);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 2);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 2);
    QCOMPARE(iRModel->recursiveSelectAllPossibilitiesByDefault(), false);

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchOne()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
            QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG2", "UNI", new CT_Circle(), "DI", "DE"));
                QVERIFY(oRModel->addGroupModel("UNG", "UNG3", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                    QVERIFY(oRModel->addItemModel("UNG3", "UNI2", new CT_Circle(), "DI", "DE"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindOnePossibilityInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNI"))->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 1);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchVeryComplex()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
            QVERIFY(oRModel->addItemModel("UNG2", "UNI1", new CT_Circle(), "DI1", "DE1"));
            QVERIFY(oRModel->addGroupModel("UNG2", "UNG3", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG3", "UNI2", new CT_Ellipse(), "DI2", "DE2"));
        QVERIFY(oRModel->addItemModel("UNG", "UNI3", new CT_Circle(), "DI3", "DE3"));
        QVERIFY(oRModel->addGroupModel("UNG", "UNG4", new CT_StandardItemGroup(), "DIG4", "DEG4"));
            QVERIFY(oRModel->addItemModel("UNG4", "UNI4", new CT_Circle(), "DI4", "DE4"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, "UNG")->nPossibilitiesSaved(), 3);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, "UNI")->nPossibilitiesSaved(), 3);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName())->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->recursiveSelectAllPossibilitiesByDefault(), false);

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchSimple1Optional()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
        oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
            QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG2", "UNI", new CT_Circle(), "DI", "DE"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsOptional));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, "UNG"))->nPossibilitiesSaved(), 2);
    QCOMPARE(((CT_InAbstractModel*)iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName()))->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->recursiveSelectAllPossibilitiesByDefault(), false);

    delete oRModel;
    delete iRModel;
}

void ModelTest::testCaseTreeSearchComplexOptional()
{
    CT_OutResultModelGroup *oRModel = new CT_OutResultModelGroup("UNR", NULL, "RN", "DIR", "DER");
    oRModel->setRootGroup("UNG", new CT_StandardItemGroup(), "DIG", "DEG");
        QVERIFY(oRModel->addGroupModel("UNG", "UNG2", new CT_StandardItemGroup(), "DIG2", "DEG2"));
            QVERIFY(oRModel->addItemModel("UNG2", "UNI1", new CT_Circle(), "DI1", "DE1"));
            QVERIFY(oRModel->addGroupModel("UNG2", "UNG3", new CT_StandardItemGroup(), "DIG2", "DEG2"));
                QVERIFY(oRModel->addItemModel("UNG3", "UNI2", new CT_Ellipse(), "DI2", "DE2"));
                QVERIFY(oRModel->addItemModel("UNG3", "UNI3", new CT_Circle(), "DI3", "DE3"));
        QVERIFY(oRModel->addGroupModel("UNG", "UNG4", new CT_StandardItemGroup(), "DIG4", "DEG4"));
            QVERIFY(oRModel->addItemModel("UNG4", "UNI4", new CT_Circle(), "DI4", "DE4"));
    oRModel->recursiveSetComplete();

    CT_InResultModelGroup *iRModel = new CT_InResultModelGroup("UNR", NULL, "DIR", "DER", true);
    QVERIFY(iRModel->setZeroOrMoreRootGroup());
    QVERIFY(iRModel->addGroupModel("", "UNG", CT_StandardItemGroup::staticGetType(), "DIG", "DEG", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory));
    QVERIFY(iRModel->addItemModel("UNG", "UNI", CT_Circle::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsOptional));
    QVERIFY(iRModel->addItemModel("UNG", "UNI2", CT_Ellipse::staticGetType(), "DI", "DE", CT_InStdSingularItemModel::C_ChooseOneIfMultiple, CT_InStdSingularItemModel::F_IsObligatory));

    QVERIFY(iRModel->recursiveFindAllPossibilitiesInModel(*oRModel, true));
    QCOMPARE(iRModel->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, "UNG")->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, "UNI")->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, "UNI2")->nPossibilitiesSaved(), 1);
    QCOMPARE(iRModel->findModelInTreeOfModelInPossibility(0, iRModel->rootGroup()->uniqueName())->nPossibilitiesSaved(), 1);
    QVERIFY(iRModel->recursiveSelectAllPossibilitiesByDefault());
    QVERIFY(iRModel->recursiveIsAtLeastOnePossibilitySelectedIfItDoes());

    delete oRModel;
    delete iRModel;
}
