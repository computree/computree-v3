#include "ct_stepbeginloop.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include <limits>

#define DEF_outResult "r"
#define DEF_outGroup "g"
#define DEF_outCounter "c"

CT_StepBeginLoop::CT_StepBeginLoop(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _nTurns = 1;
    _counter = QSharedPointer<CT_Counter>();
}

CT_StepBeginLoop::~CT_StepBeginLoop()
{
    _counter.clear();
}

QString CT_StepBeginLoop::getStepDescription() const
{
    return tr("Démarre une boucle dans le script");
}

QString CT_StepBeginLoop::getStepDetailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* CT_StepBeginLoop::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    return new CT_StepBeginLoop(dataInit);
}

bool CT_StepBeginLoop::isSettingsModified() const
{
    if (_counter.isNull())
    {
        return CT_AbstractStepCanBeAddedFirst::isSettingsModified();
    } else {
        return (!_counter->isFinished());
    }
}

//////////////////// PROTECTED //////////////////

void CT_StepBeginLoop::createInResultModelListProtected()
{
    setNotNeedInputResult();
}


// Redefine in children steps to remplace ConfigurationDialog
void CT_StepBeginLoop::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addInt(tr("Nombre d'itérations"), "", 1, std::numeric_limits<int>::max(), _nTurns);

    createPostConfigurationDialog(configDialog);

    _counter.clear();
}

// Redefine in children steps to complete ConfigurationDialog
void CT_StepBeginLoop::createPostConfigurationDialog(CT_StepConfigurableDialog *configDialog)
{
    Q_UNUSED(configDialog);
}


void CT_StepBeginLoop::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res = createNewOutResultModel(DEF_outResult, "Compteur");
    res->setRootGroup(DEF_outGroup);
    res->addItemModel(DEF_outGroup, DEF_outCounter, new CT_LoopCounter(), tr("Compteur"));

    createOutResultModelListProtected(res);
}

// Redefine in children steps to complete out Models
void CT_StepBeginLoop::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);
}


void CT_StepBeginLoop::compute()
{
    // on récupère le résultat copié
    CT_ResultGroup *outRes = getOutResultList().first();
    CT_StandardItemGroup* group = new CT_StandardItemGroup(DEF_outGroup, outRes);
    outRes->addGroup(group);

    if (_counter.isNull())
    {
        _counter = QSharedPointer<CT_Counter>(new CT_Counter(_nTurns));

    }

    PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Début de boucle, tour %1 sur %2")).arg(_counter->getCurrentTurn()).arg(_counter->getNTurns()));


    CT_LoopCounter* loopCounter = new CT_LoopCounter(DEF_outCounter, outRes, _counter);
    group->addItemDrawable(loopCounter);

    // For use in children steps
    compute(outRes, group);
}


// Redefine in children steps to complete compute method
void CT_StepBeginLoop::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);
}

