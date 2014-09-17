#include "cdm_stepmanager.h"

#include "cdm_scriptmanagerabstract.h"
#include "cdm_actionsmanager.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_step/abstract/ct_abstractstepserializable.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QWaitCondition>
#include <QLocale>

CDM_ScriptStepObjectUserData::CDM_ScriptStepObjectUserData(QString path)
{
    _stepLoadSerializedResultDirPath = path;
}

void CDM_ScriptStepObjectUserData::setSerializedResultDirPath(QString path)
{
    _stepLoadSerializedResultDirPath = path;
}

QString CDM_ScriptStepObjectUserData::getSerializedResultDirPath() const
{
    return _stepLoadSerializedResultDirPath;
}

CDM_StepManager::CDM_StepManager(const CDM_ScriptManagerAbstract *scriptManager,
                                 const CDM_ActionsManager *actionManager) : QThread()
{
    setScriptManager(scriptManager);

    _beginStep = NULL;
    m_currentStep = NULL;
    _debugMode = false;
    _stop = false;
    _action = None;
    m_guiContext = NULL;
    m_actionsManager = (CDM_ActionsManager*)actionManager;
    _force = false;

    _options.load();
    setDefaultQLocale(_options.getQLocale());

    connect(this, SIGNAL(stepFinishExecuted(CT_VirtualAbstractStep*)), this, SLOT(slotRemoveActionsAfterStepExecuted()), Qt::QueuedConnection);
}

CDM_StepManager::~CDM_StepManager()
{
    stop();

    wait();

    qDeleteAll(_stepRootList.begin(), _stepRootList.end());
    //qDeleteAll(_threadCreated.begin(), _threadCreated.end());

    _options.save();
}

void CDM_StepManager::setGuiContext(const GuiContextInterface *guiContext)
{
    m_guiContext = (GuiContextInterface*)guiContext;
}

void CDM_StepManager::setScriptManager(const CDM_ScriptManagerAbstract *scriptManager)
{
    _scripManager = (CDM_ScriptManagerAbstract*)scriptManager;
}

CDM_ScriptManagerAbstract* CDM_StepManager::getScriptManager() const
{
    return _scripManager;
}

bool CDM_StepManager::addStep(CT_VirtualAbstractStep *step, CT_VirtualAbstractStep *parent)
{
    if(parent == NULL)
    {
        _stepRootList.append(step);
        //createThread(*step);

        connectStep(step);

        emit stepAdded(step);

        return true;
    }
    else
    {
        if(parent->appendStep(step))
        {
            //createThread(*step);

            connectStep(step);

            emit stepAdded(step);

            return true;
        }
    }

    return false;
}

bool CDM_StepManager::insertStep(CT_VirtualAbstractStep *step, CT_VirtualAbstractStep &parent)
{
    if(parent.insertStep(-1, step))
    {
        //createThread(*step);

        connectStep(step);

        emit stepInserted(-1, step);

        return true;
    }

    return false;
}

bool CDM_StepManager::removeStep(CT_VirtualAbstractStep *step)
{
    if(isRunning())
    {
        return false;
    }

    if(step->parentStep() != NULL)
    {
        emit stepToBeRemoved(step);

        //_threadCreated.removeOne(step->thread());
        step->parentStep()->removeStep(step);

        return true;
    }
    else
    {
        emit stepToBeRemoved(step);

        //_threadCreated.removeOne(step->thread());
        _stepRootList.removeOne(step);
        delete step;

        return true;
    }

    return false;
}

void CDM_StepManager::setStepDebugModeOn(CT_VirtualAbstractStep *step, bool debugModeOn)
{
    if(m_currentStep == step)
        step->postWaitForAckIfInDebugMode();

    step->setDebugModeOn(debugModeOn);
}

QList<CT_VirtualAbstractStep*> CDM_StepManager::getStepRootList() const
{
    return _stepRootList;
}

bool CDM_StepManager::executeStep(CT_VirtualAbstractStep *beginStep)
{
    if(isRunning())
    {
        if(_debugMode)
        {
            if(ackDebugMode(-1))
            {
                emit stepWaitForAckInDebugMode(false);

                return true;
            }
        }

        return false;
    }

    return internalExecuteStep(beginStep, true);
}

bool CDM_StepManager::executeModifyStep(CT_VirtualAbstractStep *beginStep)
{
    if(isRunning() || (beginStep == NULL) || !beginStep->isModifiable())
        return false;

    return internalExecuteModifyStep(beginStep, true);
}

bool CDM_StepManager::executeOrForwardStepInDebugMode(CT_VirtualAbstractStep *beginStep)
{
    if(!isRunning())
    {
        return internalExecuteStep(beginStep, true);
    }

    if(ackDebugMode(1))
    {
        emit stepWaitForAckInDebugMode(false);

        return true;
    }

    return false;
}

bool CDM_StepManager::executeOrForwardStepFastInDebugMode(CT_VirtualAbstractStep *beginStep)
{
    if(!isRunning())
    {
        return internalExecuteStep(beginStep, true);
    }

    if(ackDebugMode(_options.getFastJumpValueInDebugMode()))
    {
        emit stepWaitForAckInDebugMode(false);

        return true;
    }

    return false;
}

bool CDM_StepManager::quitManualMode()
{
    QMutexLocker locker(&_mutex);

    if(m_currentStep == NULL)
        return false;

    m_currentStep->quitManualMode();

    return true;
}

bool CDM_StepManager::loadSerializedResultFromStep(CT_AbstractStepSerializable &childBeginStep)
{
    if(isRunning())
    {
        return false;
    }

    CDM_ScriptStepObjectUserData *userData = dynamic_cast<CDM_ScriptStepObjectUserData*>(childBeginStep.userData(0));

    if((userData == NULL)
            || (!childBeginStep.canBeDeserialized(userData->getSerializedResultDirPath())))
    {
        return false;
    }

    _action = LoadSerializeResult;
    _stop = false;

    emit started(true);

    _beginStep = &childBeginStep;

    start();

    return true;
}

void CDM_StepManager::stop()
{
    QMutexLocker locker(&_mutex);

    _stop = true;

    if(m_currentStep != NULL)
        m_currentStep->stop();
}

bool CDM_StepManager::setFastForwardJumpInDebugMode(int value)
{
    return _options.changeFastJumpInDebugMode(value);
}

bool CDM_StepManager::setOptions(CDM_StepManagerOptions options)
{
    if(!isRunning())
    {
        _options = options;
        setDefaultQLocale(_options.getQLocale());

        return true;
    }

    return false;
}

CDM_StepManagerOptions CDM_StepManager::getOptions() const
{
    return _options;
}

CDM_StepManager::ActionType CDM_StepManager::getAction() const
{
    return _action;
}

bool CDM_StepManager::checkAutoSaveDisabledIfExecuteFromStep(CT_VirtualAbstractStep &step) const
{
    return (step.parentStep() != NULL);
}

bool CDM_StepManager::checkOneStepIsInDebugModeFromStep(CT_VirtualAbstractStep *step) const
{
    QList<CT_VirtualAbstractStep*> stepChildList;

    if(step != NULL)
    {
        if(step->isDebugModeOn())
            return true;

        stepChildList = step->getStepChildList();
    }
    else
    {
        stepChildList = getStepRootList();
    }

    QListIterator<CT_VirtualAbstractStep*> it(stepChildList);

    while(it.hasNext())
    {
        if(checkOneStepIsInDebugModeFromStep(it.next()))
            return true;
    }

    return false;
}

bool CDM_StepManager::isInManualMode() const
{
    QMutexLocker locker((QMutex*)&_mutex);

    if(m_currentStep == NULL)
        return false;

    return m_currentStep->isInManualMode();
}

////////////// PROTECTED //////////////

bool CDM_StepManager::internalExecuteStep(CT_VirtualAbstractStep *beginStep, bool debugMode)
{
    _action = ExecuteStep;
    _debugMode = debugMode;
    _stop = false;
    _force = (beginStep != NULL);

    emit started(true);

    _beginStep = beginStep;

    start();

    return true;
}

bool CDM_StepManager::internalExecuteModifyStep(CT_VirtualAbstractStep *beginStep, bool debugMode)
{
    _action = ExecuteModifyStep;
    _debugMode = debugMode;
    _stop = false;
    _force = (beginStep != NULL);

    emit started(true);

    _beginStep = beginStep;

    start();

    return true;
}

void CDM_StepManager::connectStep(CT_VirtualAbstractStep *step)
{
    connect(step, SIGNAL(resultAdded(const CT_AbstractResult*)), this, SIGNAL(resultAdded(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(step, SIGNAL(resultToBeClearedFromMemory(const CT_AbstractResult*)), this, SIGNAL(resultToBeClearedFromMemory(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(step, SIGNAL(resultToBeRemoved(const CT_AbstractResult*)), this, SIGNAL(resultToBeRemoved(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(step, SIGNAL(resultToBeSerialized(const CT_AbstractResult*)), this, SIGNAL(resultToBeSerialized(const CT_AbstractResult*)), Qt::DirectConnection);
}

void CDM_StepManager::connectStepBeforeRunning(CT_VirtualAbstractStep *step)
{
    connect(step, SIGNAL(waitForAckInDebugMode()), this, SLOT(slotStepWaitForAckInDebugMode()), Qt::QueuedConnection);
    connect(step, SIGNAL(showMessage(QString)), this, SIGNAL(stepNeedShowMessage(QString)), Qt::QueuedConnection);
    connect(step, SIGNAL(manualModeRequired()), this, SLOT(slotStepRequiredManualMode()), Qt::QueuedConnection);
    connect(step, SIGNAL(manualModeCompleted()), this, SLOT(slotStepManualModeCompleted()), Qt::QueuedConnection);
}

void CDM_StepManager::disconnectStepAfterRunning(CT_VirtualAbstractStep *step)
{
    disconnect(step, SIGNAL(waitForAckInDebugMode()), this, SLOT(slotStepWaitForAckInDebugMode()));
    disconnect(step, SIGNAL(showMessage(QString)), this, SIGNAL(stepNeedShowMessage(QString)));
    disconnect(step, SIGNAL(manualModeRequired()), this, SLOT(slotStepRequiredManualMode()));
    disconnect(step, SIGNAL(manualModeCompleted()), this, SLOT(slotStepManualModeCompleted()));
}

void CDM_StepManager::run()
{
    if(_action == ExecuteStep)
    {
        QString scriptFileAndSerializedDirName = QString("serialization_%1").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss"));

        bool restart;

        do
        {
            restart = false;

            if(_beginStep == NULL)
            {
                bool continueLoop = true;

                QListIterator<CT_VirtualAbstractStep*> it(_stepRootList);

                while(continueLoop
                      && !restart
                      && it.hasNext())
                {
                    continueLoop = recursiveExecuteStep(scriptFileAndSerializedDirName, *it.next(), restart, false);
                }

                if(restart)
                    _beginStep = NULL;
            }
            else
            {
                if(checkAutoSaveDisabledIfExecuteFromStep(*_beginStep))
                    _options.disableAutoSave();

                recursiveExecuteStep(scriptFileAndSerializedDirName, *_beginStep, restart, _force);
            }

        }while(restart);
    }
    else if(_action == LoadSerializeResult)
    {
        CT_AbstractStepSerializable *beginCT_AbstractStepSerializable = dynamic_cast<CT_AbstractStepSerializable*>(_beginStep);

        if(beginCT_AbstractStepSerializable != NULL)
        {
            CDM_ScriptStepObjectUserData *userData = dynamic_cast<CDM_ScriptStepObjectUserData*>(beginCT_AbstractStepSerializable->userData(0));

            if((userData != NULL)
                    && (beginCT_AbstractStepSerializable->canBeDeserialized(userData->getSerializedResultDirPath())))
            {
                // on va tout d'abord supprimer tous les rsultats
                // des tapes filles de l'tape d'o on doit commencer
                recursiveClearResult(*_beginStep);

                beginCT_AbstractStepSerializable->deserialize(userData->getSerializedResultDirPath());
            }
        }
    }

    emit completed(true);
}

////////////// PRIVATE //////////////

bool CDM_StepManager::recursiveExecuteStep(QString &scriptFileAndSerializedDirName, CT_VirtualAbstractStep &step, bool &restart, bool force)
{
    bool continueLoop = true;
    bool forceAfterExecute = force;

    // si l'tape a subit de changement
    // ou si elle n'a aucun rsultat de sortie
    // ou si elle en a un mais qu'il a t supprim de la mmoire
    if(forceAfterExecute
        || step.isSettingsModified()
        || (step.nResult() == 0)
        || ((step.nResult() > 0)
            && (step.getResult(0)->isClearedFromMemory())))
    {
        // on lance l'tape seulement si elle a besoin d'un rsultat
        // et qu'il y en a un
        // OU
        // que l'tape n'a pas besoin de rsultat
        if((step.needInputResults()
            && (step.parentStep() != NULL)
            && !step.parentStep()->isStopped()
            && (step.parentStep()->getErrorCode() == 0))
            || !step.needInputResults())
        {
            _mutex.lock();
            m_currentStep = &step;
            connectStepBeforeRunning(&step);

            if(step.isDebugModeOn())
                step.setGuiContext(m_guiContext);

            _mutex.unlock();

            emit stepBeginExecuted(&step);

            // lance l'tape
            if((_action == ExecuteModifyStep) && (_beginStep != NULL) && (&step == _beginStep) && step.isModifiable())
                step.executeModify();
            else
                step.execute();

            emit stepFinishExecuted(&step);

            _mutex.lock();
            disconnectStepAfterRunning(&step);

            step.setGuiContext(NULL);

            m_currentStep = NULL;
            _mutex.unlock();

            forceAfterExecute = true;

            if(step.getErrorCode() != 0)
                continueLoop = false;
        }
        else
        {
            continueLoop = false;
        }
    }

    if(continueLoop)
        continueLoop = !step.isStopped();

    if(continueLoop)
    {
        if(step.mustRecheckTree())
        {
            restart = true;
            return continueLoop;
        }
    }

    // si on a pas stopp l'etape ou si il n'y a pas eu d'erreur
    if(continueLoop)
    {
        // on continu avec les tapes filles
        QList<CT_VirtualAbstractStep*> children = step.getStepChildList();
        QListIterator<CT_VirtualAbstractStep*> it(children);

        while(continueLoop
              && !restart
              && it.hasNext())
        {
            CT_VirtualAbstractStep *childStep = it.next();

            continueLoop = recursiveExecuteStep(scriptFileAndSerializedDirName, *childStep, restart, forceAfterExecute);
        }

        if(!restart)
        {
            // si on doit supprimer de la mmoire les rsultats des tapes et
            // que la sauvegarde automatique n'est pas active
            if(_options.isAutoClearResultFromMemoryEnable()
                && !_options.isAutoSaveEnable())
            {
                // on peut commencer  dtruire les rsultats
                step.clearOutResultFromMemory();
            }
            // sinon si on doit sauvegarder les rsultats dans un fichier
            // et qu'on est revenu  la premire tape
            else if(_options.isAutoSaveEnable()
                    && (step.parentStep() == NULL))
            {
                // normalement la premire tape est du type CT_AbstractStepSerializable
                CT_AbstractStepSerializable *stepSerializable = dynamic_cast<CT_AbstractStepSerializable*>(&step);

                if(stepSerializable != NULL)
                {
                    QString savePath = QString("%1/%2").arg(_options.autoSaveDirPath()).arg(scriptFileAndSerializedDirName);

                    // donc on peut la srialiser
                    if(stepSerializable->serialize(savePath, _options.isAutoClearResultFromMemoryEnable()))
                    {
                        // on enregistre l'endroit ou sont enregistr les rsultats srialiss
                        stepSerializable->setUserData(0, new CDM_ScriptStepObjectUserData(savePath));

                        if(getScriptManager() != NULL)
                        {
                            getScriptManager()->writeScript(QString("%1/%2.%3").arg(_options.autoSaveDirPath()).arg(scriptFileAndSerializedDirName).arg(getScriptManager()->getFileExtensionAccepted().first()),
                                                       true,
                                                       *this);
                        }
                    }
                }
            }
        }
    }

    return continueLoop;
}

void CDM_StepManager::recursiveClearResult(CT_VirtualAbstractStep &step)
{
    QList<CT_VirtualAbstractStep *> stepChildList = step.getStepChildList();
    QListIterator<CT_VirtualAbstractStep*> it(stepChildList);

    while((!_stop)
            && it.hasNext())
    {
        recursiveClearResult(*(it.next()));
    }

    if(!_stop)
    {
        step.clearOutResult();
    }
}

/*QThread* CDM_StepManager::createThread(CT_VirtualAbstractStep &step)
{
    QThread *thread = step.thread();

    if(!_threadCreated.contains(thread))
    {
        thread = new QThread();
        step.moveToThread(thread);

        connect(&step, SIGNAL(isCompleted()), thread, SLOT(quit()), Qt::DirectConnection);
        connect(thread, SIGNAL(started()), &step, SLOT(execute()), Qt::QueuedConnection);

        _threadCreated.append(thread);
    }

    return thread;
}*/

bool CDM_StepManager::ackDebugMode(int jumpNStep)
{
    QMutexLocker locker(&_mutex);

    if(m_currentStep != NULL)
    {
        m_currentStep->postWaitForAckIfInDebugMode();

        if(jumpNStep < 0)
            m_currentStep->setDebugModeOn(false);
        else
            m_currentStep->ackDebugMode(jumpNStep);

        return true;
    }

    return false;
}

void CDM_StepManager::slotStepWaitForAckInDebugMode()
{
    m_currentStep->preWaitForAckIfInDebugMode();

    emit stepWaitForAckInDebugMode(true);
}

void CDM_StepManager::slotStepRequiredManualMode()
{
    CT_VirtualAbstractStep *step = (CT_VirtualAbstractStep*)sender();

    if(step != NULL)
    {
        // si on ne connait pas le contexte donc on
        // tourne en mode batch
        if(m_guiContext == NULL)
        {
            step->quitManualMode(); // on quitte le mode manuel car il n'est pas possible
        }
        else
        {
            emit stepRequiredManualMode(step);
            emit stepInManualMode(true);

            // sinon si on n'a pas déjà donné le contexte à l'étape
            if(step->getGuiContext() != m_guiContext)
                step->setGuiContext(m_guiContext); // on lui donne

            // on informe l'étape qu'on a bien activé le mode manuel
            step->ackManualMode();
        }
    }
}

void CDM_StepManager::slotStepManualModeCompleted()
{
    CT_VirtualAbstractStep *step = (CT_VirtualAbstractStep*)sender();

    if(step != NULL)
    {
        emit stepInManualMode(false);
        emit stepQuitManualMode(step);
    }
}

void CDM_StepManager::setDefaultQLocale(QString name)
{
    QLocale locale(name);
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    QLocale::setDefault(locale);
}

void CDM_StepManager::slotRemoveActionsAfterStepExecuted()
{
    if(m_actionsManager != NULL)
        m_actionsManager->clearActions();
}

