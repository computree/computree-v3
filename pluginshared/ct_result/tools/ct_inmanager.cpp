#include "ct_inmanager.h"

CT_InManager::CT_InManager(CT_VirtualAbstractStep &step)
{
    _inTurnManager = new CT_InTurnManager(step, _inResultModelManager);
    _inResultModelConfigurationManager = new CT_InResultModelConfigurationManager(*this);
    _inResultManager = new CT_InResultManager(_inResultModelManager, *_inTurnManager);
}

CT_InManager::~CT_InManager()
{
    delete _inResultManager;
    delete _inResultModelConfigurationManager;
    delete _inTurnManager;
}

CT_VirtualAbstractStep* CT_InManager::step() const
{
    return _inTurnManager->step();
}

void CT_InManager::setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager)
{
    _inTurnManager->setTurnIndexManager(turnIndexManager);
}

CT_InResultModelManager* CT_InManager::getResultModelManager() const
{
    return (CT_InResultModelManager*)&_inResultModelManager;
}

CT_InTurnManager* CT_InManager::getTurnManager() const
{
    return _inTurnManager;
}

CT_InResultModelConfigurationManager* CT_InManager::getResultModelConfigurationManager() const
{
    return _inResultModelConfigurationManager;
}

CT_InResultManager* CT_InManager::getResultManager() const
{
    return _inResultManager;
}

void CT_InManager::aboutToBeDeleted()
{
    _inResultModelConfigurationManager->aboutToBeDeleted();
}
