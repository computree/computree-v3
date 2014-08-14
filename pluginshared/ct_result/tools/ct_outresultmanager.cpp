#include "ct_outresultmanager.h"

#include "ct_abstractresult.h"

CT_OutResultManager::CT_OutResultManager(CT_OutResultModelManager &resultModelManager,
                                         CT_OutTurnManager &turnManager)
{
    _resultModelManager = &resultModelManager;
    _turnManager = &turnManager;
}

CT_OutResultManager::~CT_OutResultManager()
{
    clearResults();
}

bool CT_OutResultManager::initResultsForCurrentTurn()
{
    int index = _turnManager->getTurnIndexManager()->currentTurnIndex();

    // si le tour courant est supérieur au nombre de tour définis on ne pourra pas créer de nouveaux résultats (erreur)
    if(index >= _turnManager->nTurn())
        return false;

    // si on a louper un tour ou si on a déjà créé les résultats pour ce tour (erreur)
    if(index != _results.size())
        return false;

    CT_OutTurn *turn = _turnManager->turns().at(index);

    QList<CT_AbstractResult*> *list = new QList<CT_AbstractResult*>(turn->createResults());

    if(list->isEmpty()
            && !turn->models().isEmpty())
    {
        delete list;
        return false;
    }
    else if(!list->isEmpty())
        _results.append(list);

    return true;
}

QList<CT_AbstractResult*> CT_OutResultManager::getResultsForCurrentTurn() const
{
    if(_turnManager->getTurnIndexManager()->currentTurnIndex() < _results.size())
        return (*_results.at(_turnManager->getTurnIndexManager()->currentTurnIndex()));

    return QList<CT_AbstractResult*>();
}

QList< QList<CT_AbstractResult*>* > CT_OutResultManager::getResultsForAllTurn() const
{
    return _results;
}

int CT_OutResultManager::nResults() const
{
    int size = 0;

    QListIterator< QList<CT_AbstractResult*>* > it(_results);

    while(it.hasNext())
    {
        size += it.next()->size();
    }

    return size;
}

CT_AbstractResult* CT_OutResultManager::getResult(int n) const
{
    int size = 0;

    QListIterator< QList<CT_AbstractResult*>* > it(_results);

    while(it.hasNext())
    {
        QList<CT_AbstractResult*> *list2 = it.next();

        if((size + list2->size()) > n)
            return list2->at(n-size);
    }

    return NULL;
}

CT_AbstractResult* CT_OutResultManager::takeResult(int n)
{
    int size = 0;

    QMutableListIterator< QList<CT_AbstractResult*>* > it(_results);

    while(it.hasNext())
    {
        QList<CT_AbstractResult*> *list2 = it.next();

        if((size + list2->size()) > n)
        {
            CT_AbstractResult *ret = list2->takeAt(n-size);

            if(list2->isEmpty())
            {
                it.remove();
                delete list2;
            }

            return ret;
        }
    }

    return NULL;
}

void CT_OutResultManager::clearResults()
{
    while(!_results.isEmpty())
    {
        QList<CT_AbstractResult*> *list = _results.takeFirst();

        qDeleteAll(list->begin(), list->end());
        delete list;
    }
}

void CT_OutResultManager::completesResultsOfCurrentTurn()
{
    if(_turnManager->getTurnIndexManager()->currentTurnIndex() >= _results.size())
        return;

    QListIterator<CT_AbstractResult*> it(*_results.at(_turnManager->getTurnIndexManager()->currentTurnIndex()));

    while(it.hasNext())
    {
        CT_AbstractResult *res = it.next();

        if(res->isClearedFromMemory())
        {
            res->setComplete();

            emit resultAdded(res);
        }
    }
}
