#include "ct_inresultmanager.h"

#include "ct_abstractresult.h"

CT_InResultManager::CT_InResultManager(CT_InResultModelManager &resultModelManager,
                                       CT_InTurnManager &turnManager)
{
    _inResultModelManager = &resultModelManager;
    _turnManager = &turnManager;
}

bool CT_InResultManager::initResultsFromStep(CT_VirtualAbstractStep *step)
{
    clearResults();

    if(!_inResultModelManager->needInputResults())
        return true;

    if(_turnManager->nValidTurn() == 0)
        return false;

    QListIterator<CT_InTurn*> it(_turnManager->turns());

    while(it.hasNext())
    {
        CT_InTurn *turn = it.next();

        if(turn->isValid())
        {
            QList<CT_InResultsByModel*> list = turn->getResultsFromStep(step);

            if(list.isEmpty())
            {
                clearResults();
                return false;
            }

            _results.append(list);
        }
    }

    return true;
}

bool CT_InResultManager::existResultsForCurrentTurn() const
{
    if(_turnManager->getTurnIndexManager() == NULL)
        return false;

    return _turnManager->getTurnIndexManager()->currentTurnIndex() < _results.size();
}

QList<CT_AbstractResult*> CT_InResultManager::getResultsForCurrentTurn() const
{
    QList<CT_AbstractResult*> retList;

    if(existResultsForCurrentTurn())
    {
        QListIterator<CT_InResultsByModel*> it(_results.at(_turnManager->getTurnIndexManager()->currentTurnIndex()));

        while(it.hasNext())
        {
            retList.append(it.next()->getResults());
        }
    }

    return retList;
}

QList<CT_InResultsByModel*> CT_InResultManager::getResultsSortedByModelsForCurrentTurn() const
{
    return _results.at(_turnManager->getTurnIndexManager()->currentTurnIndex());
}

void CT_InResultManager::clearResults()
{
    while(!_results.isEmpty())
    {
        QList<CT_InResultsByModel*> list = _results.takeLast();
        qDeleteAll(list.begin(), list.end());
    }
}

void CT_InResultManager::setResultsOfCurrentTurnBusy(bool busy)
{
    if(existResultsForCurrentTurn())
    {
        QListIterator<CT_InResultsByModel*> it(_results.at(_turnManager->getTurnIndexManager()->currentTurnIndex()));

        while(it.hasNext())
        {
            QListIterator<CT_AbstractResult*> itR(it.next()->getResults());

            while(itR.hasNext())
            {
                itR.next()->setBusy(busy);
            }
        }
    }
}
