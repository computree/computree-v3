#include "ct_outturnmanager.h"

#include "ct_outabstractresultmodel.h"

CT_OutTurnManager::CT_OutTurnManager(CT_OutResultModelManager &resultModelManager)
{
    _manager = &resultModelManager;
}

CT_OutTurnManager::~CT_OutTurnManager()
{
    clearBackup();
    clearTurnList();
    clearSearchModelList();
}

void CT_OutTurnManager::setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager)
{
    _tIndexManager = (CT_TurnIndexManager*)turnIndexManager;
}

CT_TurnIndexManager* CT_OutTurnManager::getTurnIndexManager() const
{
    return _tIndexManager;
}

CT_OutResultModelManager* CT_OutTurnManager::getResultModelManager() const
{
    return _manager;
}

CT_OutTurn* CT_OutTurnManager::createTurn(bool addTurnNameToDisplayableName)
{
    if(_tIndexManager == NULL)
        return NULL;

    CT_OutTurn *turn = NULL;
    int size = turns().size();

    if(size < _tIndexManager->maximumTurn())
    {
        turn = new CT_OutTurn();

        QListIterator<CT_OutAbstractResultModel*> it(_manager->takeModels());

        while(it.hasNext())
        {
            CT_OutAbstractResultModel *outModel = it.next();

            outModel->recursiveSetComplete();

            if(addTurnNameToDisplayableName)
                outModel->setDisplayableName(outModel->displayableName() + QString(" [T%1]").arg(size+1));

            // ATTENTION NE JAMAIS SUPPRIMER CETTE LIGNE !
            // Sinon la recherche des résultats à utiliser en entrée dans les étapes ne fonctionnera plus
            if(_tIndexManager->nTurn() > 1)
                outModel->setTurnIndex(size+1);

            turn->addResultModel(outModel);
        }

        _turns.append(turn);
    }

    return turn;
}

int CT_OutTurnManager::nTurn() const
{
    return _turns.size();
}

const QList<CT_OutTurn*>& CT_OutTurnManager::turns() const
{
    return _turns;
}

void CT_OutTurnManager::clearTurnList()
{
    qDeleteAll(_turns.begin(), _turns.end());
    _turns.clear();
}

bool CT_OutTurnManager::createSearchModelListForCurrentTurn()
{
    clearSearchModelList();

    if(_tIndexManager == NULL)
        return false;

    if(_tIndexManager->currentTurnIndex() >= nTurn())
        return false;

    CT_OutTurn *turn = _turns.at(_tIndexManager->currentTurnIndex());

    QListIterator<CT_OutAbstractResultModel*> it(turn->models());

    while(it.hasNext())
    {
        CT_OutAbstractResultModel *model = it.next();

        if(existInSearchResultModelList(model))
        {
            clearSearchModelList();

            return false;
        }

        QMap<QString,CT_OutAbstractModel*> *map = new QMap<QString,CT_OutAbstractModel*>();
        _modelsForCurrentTurn.insert(model, map);

        QList<CT_AbstractModel*> list = model->recursiveGetAllModels();
        QListIterator<CT_AbstractModel*> itM(list);

        while(itM.hasNext())
        {
            CT_AbstractModel *outModel = itM.next();

            if(map->contains(outModel->uniqueName()))
            {
                clearSearchModelList();

                return false;
            }

            map->insert(outModel->uniqueName(), (CT_OutAbstractModel*)outModel);
        }

        _resultModelsForCurrentTurn.append(model);
    }

    return true;
}

void CT_OutTurnManager::clearSearchModelList()
{
    _resultModelsForCurrentTurn.clear();

    qDeleteAll(_modelsForCurrentTurn.begin(), _modelsForCurrentTurn.end());
    _modelsForCurrentTurn.clear();
}

CT_OutAbstractResultModel* CT_OutTurnManager::getOutResultModel(QString modelName) const
{
    QListIterator<CT_OutAbstractResultModel*> it(_resultModelsForCurrentTurn);

    while(it.hasNext())
    {
        CT_OutAbstractResultModel *r = it.next();

        if(r->uniqueName() == modelName)
            return r;
    }

    return NULL;
}

CT_OutAbstractModel *CT_OutTurnManager::getOutModel(const CT_OutAbstractResultModel &resultModel, QString modelName) const
{
    QMap<QString,CT_OutAbstractModel*> *map = _modelsForCurrentTurn.value((CT_OutAbstractResultModel*)&resultModel, NULL);

    if(map == NULL)
        return NULL;

    return map->value(modelName, NULL);
}

void CT_OutTurnManager::backupTurns()
{
    clearBackup();

    m_backup = _turns;

    _turns.clear();
}

bool CT_OutTurnManager::hasBackup() const
{
    return !m_backup.isEmpty();
}

void CT_OutTurnManager::clearBackup()
{
    qDeleteAll(m_backup.begin(), m_backup.end());
    m_backup.clear();
}

// PRIVATE //

bool CT_OutTurnManager::existInSearchResultModelList(CT_OutAbstractResultModel *model) const
{
    QListIterator<CT_OutAbstractResultModel*> it(_resultModelsForCurrentTurn);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == model->uniqueName())
            return true;
    }

    return false;
}
