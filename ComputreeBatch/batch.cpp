#include "batch.h"

#include "cdm_configfile.h"

#include <QApplication>
#include <QDebug>

Batch::Batch(QuitInterface *qI)
{
    m_quitInterface = qI;
    _pluginManager = new CDM_PluginManager();
    _scriptManager = new CDM_ScriptManagerXML(*_pluginManager);
    _actionManager = new CDM_ActionsManager();
    _stepManager = new CDM_StepManager(_scriptManager, _actionManager);


    connect(this, SIGNAL(privateLoadScriptText()), this, SLOT(loadScriptText()), Qt::QueuedConnection);
    connect(this, SIGNAL(privateLoadScriptFilePath()), this, SLOT(loadScriptFilePath()), Qt::QueuedConnection);
    connect(this, SIGNAL(privateRemoveAllStep()), this, SLOT(removeAllStep()), Qt::QueuedConnection);

    connect(_stepManager, SIGNAL(started()), this, SIGNAL(started()), Qt::QueuedConnection);
    connect(_stepManager, SIGNAL(completed(bool)), this, SIGNAL(finished(bool)), Qt::QueuedConnection);
}

Batch::~Batch()
{
    delete _stepManager;
    delete _scriptManager;
    delete _pluginManager;
    delete _actionManager;

    delete CONFIG_FILE;
}

void Batch::asyncLoadScriptFromString(const QString &script)
{
    if(isRunning())
        return;

    _script = script;

    emit privateLoadScriptText();
}

void Batch::asyncLoadScriptFromFile(QString filepath)
{
    if(isRunning())
        return;

    _script = filepath;

    emit privateLoadScriptFilePath();
}

bool Batch::asyncRemoveAllStep()
{
    if(isRunning())
        return false;

    emit privateRemoveAllStep();

    return true;
}

bool Batch::execute()
{
    if(isRunning())
        return false;

    return _stepManager->executeStep(NULL);
}

bool Batch::isRunning()
{
    return _stepManager->isRunning();
}

void Batch::stop()
{
    _stepManager->stop();
}

CDM_PluginManager* Batch::getPluginManager() const
{
    return _pluginManager;
}

CDM_StepManager* Batch::getStepManager() const
{
    return _stepManager;
}

CDM_ActionsManager *Batch::getActionManager() const
{
    return _actionManager;
}

void Batch::initWithArgs()
{
    if(!_pluginManager->load())
        emit loadError(_pluginManager->getError());

    QStringList args = QCoreApplication::arguments();
    int size = args.size();

    if(size > 1)
    {
        int i = 1;

        while((i<size) && (args.at(i) != "--help"))
            ++i;

        if(i<size) {
            qWarning() << "Help for ComputreeCore :\r\n--help\t\t\tShow this help\r\n-script \"XXX.xsct2\"\tLoad a script\r\n-startSteps\t\tStart execution of steps\r\n--fullbatch\t\tDon't use system tray icon";

            executionFinished();
            return;
        }

        i = 1;

        while((i<size) && (args.at(i) != "-script"))
            ++i;

        if(i < (size-1))
        {
            QString script = args.at(i+1);

            QString error = _scriptManager->loadScript(script, *_stepManager);

            if(!error.isEmpty())
            {
                qWarning((tr("Unable to open script :\r\n") + error).toUtf8());
                executionFinished();
                return;
            }

            i = 1;

            while((i<size) && (args.at(i) != "-startSteps"))
                ++i;

            if(i < size)
            {
                connect(_stepManager, SIGNAL(completed()), this, SLOT(executionFinished()));

                if(!_stepManager->executeStep())
                    qWarning() << tr("Error when execute steps");
            }
        }
    }
}

void Batch::loadScriptText()
{
    QString error = _scriptManager->loadScriptText(_script, *_stepManager);

    emit scriptLoaded(error);
}

void Batch::loadScriptFilePath()
{
    QString error = _scriptManager->loadScript(_script, *_stepManager);

    emit scriptLoaded(error);
}

void Batch::executionFinished()
{
    m_quitInterface->quitApplication();
}

void Batch::removeAllStep()
{
    QList<CT_VirtualAbstractStep*> rootList = _stepManager->getStepRootList();

    bool ok = true;
    while(!rootList.isEmpty()
          && ok)
    {
        CT_VirtualAbstractStep *step = rootList.takeFirst();

        recursiveInformStepInGuiThreadThatWillBeDeleted(*step);

        ok = _stepManager->removeStep(step);
    }

    emit allStepRemoved();
}

void Batch::recursiveInformStepInGuiThreadThatWillBeDeleted(CT_VirtualAbstractStep &step)
{
    step.aboutToBeDeleted();

    QList<CT_VirtualAbstractStep*> stepList = step.getStepChildList();
    QListIterator<CT_VirtualAbstractStep*> it(stepList);

    while(it.hasNext())
    {
        recursiveInformStepInGuiThreadThatWillBeDeleted(*(it.next()));
    }
}


