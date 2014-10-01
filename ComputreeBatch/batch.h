#ifndef BATCH_H
#define BATCH_H

#include "interfacesbatch.h"

#include "cdm_pluginmanager.h"
#include "cdm_scriptmanagerxml.h"
#include "cdm_stepmanager.h"
#include "cdm_actionsmanager.h"

class Batch : public BatchInterface
{
    Q_OBJECT
    Q_INTERFACES(BatchInterface)

public:
    Batch();
    ~Batch();

    void initWithArgs();

    void asyncLoadScriptFromString(const QString &script);
    void asyncLoadScriptFromFile(QString filepath);
    bool asyncRemoveAllStep();

    bool execute();
    bool isRunning();
    void stop();

    CDM_PluginManager* getPluginManager() const;
    CDM_StepManager* getStepManager() const;
    CDM_ActionsManager *getActionManager() const;

private:
    CDM_PluginManager       *_pluginManager;
    CDM_ScriptManagerXML    *_scriptManager;
    CDM_StepManager         *_stepManager;
    CDM_ActionsManager      *_actionManager;

    QString                 _script;

    void recursiveInformStepInGuiThreadThatWillBeDeleted(CT_VirtualAbstractStep &step);

signals:

    void loadError(QString error);
    void privateLoadScriptText();
    void privateLoadScriptFilePath();
    void privateRemoveAllStep();

private slots:
    void loadScriptText();
    void loadScriptFilePath();

public slots:
    void removeAllStep();
};

#endif // BATCH_H
