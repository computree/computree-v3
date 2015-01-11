#ifndef CT_MONITOREDQTHREAD_H
#define CT_MONITOREDQTHREAD_H

#include <QThread>
#include "pluginShared_global.h"
#include "qdebug.h"

class PLUGINSHAREDSHARED_EXPORT CT_MonitoredQThread : public QThread
{
    Q_OBJECT
public:
    CT_MonitoredQThread();

    inline size_t getProgress() {return _progress;}

signals:
    void progressChanged();

protected:
    size_t     _progress;
};

#endif // CT_MONITOREDQTHREAD_H
