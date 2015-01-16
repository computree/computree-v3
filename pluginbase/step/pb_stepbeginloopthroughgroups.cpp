#include "pb_stepbeginloopthroughgroups.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

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
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_inResult_g, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", DEF_inGroup);
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

    CT_InAbstractResultModel *resultInModel = getInResultModel(DEF_inResult_g);
    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractGroupModel* groupModel;

    // check if model have choice (can be empty if the step want to create a default out model list)
    if(resultInModel!=NULL && !resultInModel->getPossibilitiesSavedSelected().isEmpty())
    {
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSavedSelected().first()->outModel();
    }

    if(resultInModelOut != NULL)
    {
        groupModel = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_inGroup, resultInModelOut, this);
    }

    if((groupModel != NULL) && !groupModel->getPossibilitiesSavedSelected().isEmpty())
    {
        _outModel = (DEF_CT_AbstractGroupModelOut*)groupModel->getPossibilitiesSavedSelected().first()->outModel()->copy();
        CT_OutResultModelGroup *resultModel = createNewOutResultModel("rrr", tr("toto"));
        resultModel->setRootGroup(_outModel);
    }

}

// Redefine in children steps to complete compute method
void PB_StepBeginLoopThroughGroups::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    // on récupère le résultat copié
    CT_ResultGroup *outResult_g = getOutResultList().at(1);
    CT_ResultGroupIterator it(outResult_g, this, DEF_inGroup);
    int cpt = 1;
    while (it.hasNext() && (!isStopped()))
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*) it.next();
        if (cpt++ == _counter->getCurrentTurn())
        {
            outResult_g->addGroup((CT_AbstractItemGroup*)group->copy(_outModel, outResult_g, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableReference));
        }
    }

    if (_counter->getCurrentTurn() == 1) {
        _counter->setNTurns(cpt);
    }

    setProgress( 100 );
}
