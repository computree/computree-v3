#ifndef CT_INMANAGER_H
#define CT_INMANAGER_H

#include "ct_inresultmodelconfigurationmanager.h"
#include "ct_inresultmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_InManager
{
public:
    CT_InManager(CT_VirtualAbstractStep &step);
    ~CT_InManager();

    CT_VirtualAbstractStep* step() const;

    void setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager);

    CT_InResultModelManager *getResultModelManager() const;
    CT_InTurnManager* getTurnManager() const;
    CT_InResultModelConfigurationManager* getResultModelConfigurationManager() const;
    CT_InResultManager* getResultManager() const;

    void aboutToBeDeleted();

private:

    CT_InResultModelManager                 _inResultModelManager;
    CT_InTurnManager                        *_inTurnManager;
    CT_InResultModelConfigurationManager    *_inResultModelConfigurationManager;
    CT_InResultManager                      *_inResultManager;
};

#endif // CT_INMANAGER_H
