#include "pb_stepbeginloopthroughgroups.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#define DEF_inResult_g "inResult_g"
#define DEF_inGroup  "inGroup"
#define DEF_inItem   "inItem"

PB_StepBeginLoopThroughGroups::PB_StepBeginLoopThroughGroups(CT_StepInitializeData &dataInit) : CT_StepBeginLoop(dataInit)
{
}

PB_StepBeginLoopThroughGroups::~PB_StepBeginLoopThroughGroups()
{
}

QString PB_StepBeginLoopThroughGroups::getStepDescription() const
{
    return tr("Boucle sur les groupes");
}

QString PB_StepBeginLoopThroughGroups::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* PB_StepBeginLoopThroughGroups::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepBeginLoopThroughGroups(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepBeginLoopThroughGroups::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_inResult_g, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_inGroup);
    resultModel->addItemModel(DEF_inGroup, DEF_inItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
}

// Redefine in children steps to complete ConfigurationDialog
void PB_StepBeginLoopThroughGroups::createPostConfigurationDialog(int &nTurns)
{
    Q_UNUSED(nTurns);
}

// Redefine in children steps to complete out Models
void PB_StepBeginLoopThroughGroups::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);
    createNewOutResultModelToCopy(DEF_inResult_g);
}

// Redefine in children steps to complete compute method
void PB_StepBeginLoopThroughGroups::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    // on récupère le résultat copié
    CT_ResultGroup *outResult_g = getOutResultList().at(1);

    QList<CT_AbstractItemGroup*> groupsToBeRemoved;

    CT_ResultGroupIterator it(outResult_g, this, DEF_inGroup);
    int cpt = 1;
    while (it.hasNext() && (!isStopped()))
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*) it.next();
        if (cpt++ != _counter->getCurrentTurn()) {groupsToBeRemoved.append(group);}
    }

    if (_counter->getCurrentTurn() == 1) {
        _counter->setNTurns(cpt);
    }

    // Suppression effective des groupes
    while (!groupsToBeRemoved.isEmpty())
    {
        CT_AbstractItemGroup *group = groupsToBeRemoved.takeLast();
        recursiveRemoveGroupIfEmpty(group->parentGroup(), group);
    }

    setProgress( 100 );
}

void PB_StepBeginLoopThroughGroups::recursiveRemoveGroupIfEmpty(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const
{
    if(parent != NULL)
    {
        parent->removeGroup(group);

        if(parent->isEmpty()) {recursiveRemoveGroupIfEmpty(parent->parentGroup(), parent);}
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}
