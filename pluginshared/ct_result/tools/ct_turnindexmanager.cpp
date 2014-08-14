#include "ct_turnindexmanager.h"

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"

CT_TurnIndexManager::CT_TurnIndexManager(CT_InTurnManager &inTurnManager)
{
    _inTurnManager = &inTurnManager;
    _currentTurnIndex = 0;
}

uint CT_TurnIndexManager::maximumTurn() const
{
    return _inTurnManager->maximumTurn();
}

int CT_TurnIndexManager::nTurn() const
{
    return _inTurnManager->nValidTurn();
}

uint CT_TurnIndexManager::currentTurnIndex() const
{
    return _currentTurnIndex;
}

bool CT_TurnIndexManager::incrementTurnIndex()
{
    if((_currentTurnIndex < _inTurnManager->nValidTurn())
            && (_currentTurnIndex < maximumTurn()))
    {
        ++_currentTurnIndex;
        return true;
    }

    return false;
}

void CT_TurnIndexManager::resetTurnIndex()
{
    _currentTurnIndex = 0;
}
