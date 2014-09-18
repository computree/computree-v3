#ifndef INTERFACESBATCH_H
#define INTERFACESBATCH_H

#include <QtPlugin>
#include <QObject>

class BatchInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~BatchInterface() {}

    virtual void asyncLoadScriptFromString(const QString &script) = 0;
    virtual void asyncLoadScriptFromFile(QString filepath) = 0;
    virtual bool asyncRemoveAllStep() = 0;

    virtual bool execute() = 0;
    virtual bool isRunning() = 0;
    virtual void stop() = 0;

signals:

    void scriptLoaded(QString error);
    void allStepRemoved();

    void started();
    void finished(bool error);
};

class BatchPluginInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~BatchPluginInterface() {}

    virtual void setBatchInterface(BatchInterface *inter) = 0;

    virtual void init() = 0;
    virtual bool isInitialized() const = 0;
    virtual bool isRunning() const = 0;

public slots:
    virtual void run() = 0;
    virtual void stop() = 0;

signals:
    void started();
    void finished();
};


#define BatchInterface_iid "com.krebs.michael.ONF.Batch.BatchInterface"
#define BatchPluginInterface_iid "com.krebs.michael.ONF.Batch.BatchPluginInterface"

Q_DECLARE_INTERFACE(BatchPluginInterface, BatchPluginInterface_iid)
Q_DECLARE_INTERFACE(BatchInterface, BatchInterface_iid)

#endif // INTERFACESBATCH_H
