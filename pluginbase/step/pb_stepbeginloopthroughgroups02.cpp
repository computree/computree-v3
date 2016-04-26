#include "pb_stepbeginloopthroughgroups02.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#define DEF_inResult_g "inResult"
#define DEF_inGroup  "inGroup"
#define DEF_inItem   "inItem"
#define DEF_inAttName   "inAtt"

PB_StepBeginLoopThroughGroups02::PB_StepBeginLoopThroughGroups02(CT_StepInitializeData &dataInit) : CT_StepBeginLoop(dataInit)
{
}

PB_StepBeginLoopThroughGroups02::~PB_StepBeginLoopThroughGroups02()
{
}

QString PB_StepBeginLoopThroughGroups02::getStepDescription() const
{
    return tr("Boucle sur les groupes (v2)");
}

QString PB_StepBeginLoopThroughGroups02::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* PB_StepBeginLoopThroughGroups02::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new PB_StepBeginLoopThroughGroups02(dataInit);
}

//////////////////// PROTECTED //////////////////

void PB_StepBeginLoopThroughGroups02::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_inResult_g, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_inGroup);
    resultModel->addItemModel(DEF_inGroup, DEF_inItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
    resultModel->addItemAttributeModel(DEF_inItem, DEF_inAttName, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Nom"));
}

// Redefine in children steps to complete ConfigurationDialog
void PB_StepBeginLoopThroughGroups02::createPostConfigurationDialog(int &nTurns)
{
    Q_UNUSED(nTurns);
}

// Redefine in children steps to complete out Models
void PB_StepBeginLoopThroughGroups02::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);    
    createNewOutResultModelToCopy(DEF_inResult_g);
}

// Redefine in children steps to complete compute method
void PB_StepBeginLoopThroughGroups02::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    CT_ResultGroup *outResult = getOutResultList().first();

    if (outResult != NULL)
    {
        int currentTurn = _counter->getCurrentTurn();

        if (currentTurn == 1)
        {
            _ids.clear();
            CT_ResultGroupIterator it(outResult, this, DEF_inGroup);
            while (it.hasNext())
            {
                CT_AbstractItemGroup *group = (CT_AbstractItemGroup*) it.next();

                if (group != NULL)
                {
                    _ids.append(group->id());
                }
            }
            _counter->setNTurns(_ids.size());
            NTurnsSelected();
        }

        QList<CT_AbstractItemGroup*> groupsToBeRemoved;
        CT_ResultGroupIterator it2(outResult, this, DEF_inGroup);
        while (it2.hasNext() && (!isStopped()))
        {
            CT_AbstractItemGroup *group = (CT_AbstractItemGroup*) it2.next();
            if (group->id() == currentTurn)
            {
                QString turnName = QString("Turn%1").arg(currentTurn);
                CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_inItem);
                if (item != NULL)
                {
                    CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(outResult, this, DEF_inAttName);
                    if (att !=  NULL)
                    {
                        turnName = att->toString(item, NULL);
                    }
                }
                _counter->setTurnName(turnName);
            } else {
                groupsToBeRemoved.append(group);
            }
        }

        while (!groupsToBeRemoved.isEmpty())
        {
            CT_AbstractItemGroup *group = groupsToBeRemoved.takeLast();
            recursiveRemoveGroupIfEmpty(group->parentGroup(), group);
        }
    }

    setProgress( 100 );
}

void PB_StepBeginLoopThroughGroups02::recursiveRemoveGroupIfEmpty(CT_AbstractItemGroup *parent, CT_AbstractItemGroup *group) const
{
    if(parent != NULL)
    {
        parent->removeGroup(group);

        if(parent->isEmpty())
            recursiveRemoveGroupIfEmpty(parent->parentGroup(), parent);
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}
