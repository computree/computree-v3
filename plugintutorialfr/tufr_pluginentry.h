#ifndef TUFR_PLUGINENTRY_H
#define TUFR_PLUGINENTRY_H

#include "interfaces.h"

class TUFR_StepPluginManager;

class TUFR_PluginEntry : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    TUFR_PluginEntry();
    ~TUFR_PluginEntry();

    QString getVersion() const;
    StepPluginInterface* getStepPluginManager();

private:
    TUFR_StepPluginManager *_stepPluginManager;
};

#endif // TUFR_PLUGINENTRY_H
