#include "ct_global/ct_context.h"

#include "ct_stependloop.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#define DEF_inResult "r"
#define DEF_inGroup "g"
#define DEF_inCounter "c"

#define DEF_outResult "ro"

CT_StepEndLoop::CT_StepEndLoop(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _mustRecheckTree = false;

    setDebuggable(true);
}

CT_StepEndLoop::~CT_StepEndLoop()
{
}

QString CT_StepEndLoop::getStepDescription() const
{
    return tr("Termine une boucle dans le script");
}

QString CT_StepEndLoop::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepBeginLoop auparavant pour initier la boucle");
}

CT_VirtualAbstractStep* CT_StepEndLoop::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new CT_StepEndLoop(dataInit);
}

bool CT_StepEndLoop::mustRecheckTree() const
{
    return _mustRecheckTree;
}

//////////////////// PROTECTED //////////////////

void CT_StepEndLoop::createInResultModelListProtected()
{
    CT_InResultModelGroup* res = createNewInResultModel(DEF_inResult, tr("Résultat compteur"), "", true);
    res->setRootGroup(DEF_inGroup);
    res->addItemModel(DEF_inGroup, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));
}


void CT_StepEndLoop::createOutResultModelListProtected()
{
    createNewOutResultModelToCopy(DEF_inResult);
}

void CT_StepEndLoop::compute()
{    
    _mustRecheckTree = false;

    CT_ResultGroup* res = getInputResults().first();

    CT_ResultItemIterator it(res, this, DEF_inCounter);

    if (it.hasNext())
    {
        CT_LoopCounter* counter = (CT_LoopCounter*) it.next();
        PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Fin de boucle, tour %1 sur %2")).arg(counter->getCurrentTurn()).arg(counter->getNTurns()));

        // Use the debug mode at step loop scale (and not at step scale as usual)
        if (counter->getCurrentTurn() % _jump_n_step == 0) {waitForAckIfInDebugMode();}

        if (counter->hasNextTurn())
            _mustRecheckTree = true;

        counter->beginNextTurn();
    }
}
