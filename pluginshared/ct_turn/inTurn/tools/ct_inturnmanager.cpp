#include "ct_inturnmanager.h"

#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_InTurnManager::CT_InTurnManager(CT_VirtualAbstractStep &step,
                                   CT_InResultModelManager &resultModelManager)
{
    _step = &step;
    _manager = &resultModelManager;
    _maximumTurn = 9999;
    _tIndexManager = NULL;
}

CT_InTurnManager::~CT_InTurnManager()
{
    clearTurnList();
    clearSearchModelList();
}

void CT_InTurnManager::setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager)
{
    _tIndexManager = (CT_TurnIndexManager*)turnIndexManager;
}

CT_TurnIndexManager* CT_InTurnManager::getTurnIndexManager() const
{
    return _tIndexManager;
}

CT_InResultModelManager* CT_InTurnManager::getResultModelManager() const
{
    return _manager;
}

void CT_InTurnManager::setMaximumTurn(int n)
{
    if(n > 0)
    {
        while(_turns.size() > n)
            delete _turns.takeLast();
    }

    _maximumTurn = n;
}

int CT_InTurnManager::maximumTurn() const
{
    return _maximumTurn;
}

CT_InTurn* CT_InTurnManager::createTurn()
{
    CT_InTurn *turn = NULL;

    if((_turns.isEmpty() || _turns.last()->isValid())
            && ((maximumTurn() == 0) || (nTurn() < maximumTurn())))
    {
        turn = new CT_InTurn();

        QListIterator<CT_InAbstractResultModel*> it(_manager->models());

        while(it.hasNext())
            turn->addResultModel((CT_InAbstractResultModel*)it.next()->copy(true));

        _turns.append(turn);
    }

    return turn;
}

int CT_InTurnManager::nTurn() const
{
    return _turns.size();
}

int CT_InTurnManager::nValidTurn() const
{
    if(_turns.isEmpty())
        return 0;

    if(!_turns.last()->isValid())
        return nTurn()-1;

    return nTurn();
}

void CT_InTurnManager::removeTurn(int i)
{
    delete _turns.takeAt(i);
}

const CT_InTurn* CT_InTurnManager::getTurn(int i) const
{
    return _turns.at(i);
}

const QList<CT_InTurn*>& CT_InTurnManager::turns() const
{
    return _turns;
}

void CT_InTurnManager::clearTurnList()
{
    qDeleteAll(_turns.begin(), _turns.end());
    _turns.clear();
}

bool CT_InTurnManager::createSearchModelListForCurrentTurn()
{
    clearSearchModelList();

    if(!isCurrentTurnIndexValid())
        return false;

    // get current turn
    CT_InTurn *turn = _turns.at(_tIndexManager->currentTurnIndex());

    QListIterator<CT_InAbstractResultModel*> it(turn->models());

    // for each models that represent a result in the current turn
    while(it.hasNext())
    {
        CT_InAbstractResultModel *rModel = it.next();

        QList<CT_InStdModelPossibility*> possibilities = rModel->getPossibilitiesSavedSelected();
        QListIterator<CT_InStdModelPossibility*> itP(possibilities);

        // for each possibility of this model (result)
        while(itP.hasNext())
        {
            CT_InStdResultModelPossibility *po = (CT_InStdResultModelPossibility*)itP.next();

            // create a map with key = unique name of the model AND value = the model
            QMap<QString,CT_InAbstractModel*> *map = new QMap<QString,CT_InAbstractModel*>();

            // and insert it in the map that contains all models for current
            // turn with key = a model that represent a result AND value = the map
            _modelsForCurrentTurn.insert((CT_OutAbstractResultModel*)po->outModel(), map);

            // get all models recursively (childrens and childrens of childrens, etc...)
            QList<CT_AbstractModel*> list = po->inModel()->recursiveGetAllModels();
            QListIterator<CT_AbstractModel*> itM(list);

            // for each model
            while(itM.hasNext())
            {
                CT_AbstractModel *model = itM.next();

                if(map->contains(model->uniqueName()))
                {
                    clearSearchModelList();

                    return false;
                }

                // insert it in the map
                map->insert(model->uniqueName(), (CT_InAbstractModel*)model);
            }
        }
    }

    return true;
}

void CT_InTurnManager::clearSearchModelList()
{
    qDeleteAll(_modelsForCurrentTurn.begin(), _modelsForCurrentTurn.end());
    _modelsForCurrentTurn.clear();
}

CT_InAbstractResultModel* CT_InTurnManager::getInResultModel(const QString &uniqueName) const
{
    if(!_step->isCreateDefaultOutModelActive()
            && !isCurrentTurnIndexValid())
        return NULL;

    QListIterator<CT_InAbstractResultModel*> it(_turns.at(_tIndexManager->currentTurnIndex())->models());

    while(it.hasNext())
    {
        CT_InAbstractResultModel *model = it.next();

        if(model->uniqueName() == uniqueName)
            return model;
    }

    return NULL;
}

CT_InAbstractModel* CT_InTurnManager::getInModel(const CT_OutAbstractResultModel &outResultModel, QString modelName) const
{
    QMap<QString,CT_InAbstractModel*> *map = _modelsForCurrentTurn.value((CT_OutAbstractResultModel*)(&outResultModel), NULL);

    if(map == NULL)
        return NULL;

    return map->value(modelName, NULL);
}

CT_InAbstractModel* CT_InTurnManager::getInModel(const QString &outResultUniqueName, const QString &uniqueName) const
{
    QMapIterator<CT_OutAbstractResultModel*, QMap<QString,CT_InAbstractModel*>* > it(_modelsForCurrentTurn);

    while(it.hasNext())
    {
        it.next();

        CT_OutAbstractResultModel *model = it.key();

        if(model->uniqueName() == outResultUniqueName)
            return getInModel(*model, uniqueName);
    }

    return NULL;
}

CT_VirtualAbstractStep* CT_InTurnManager::step() const
{
    return _step;
}

// PRIVATE //

bool CT_InTurnManager::isCurrentTurnIndexValid() const
{
    if(_tIndexManager == NULL)
        return false;

    if(_tIndexManager->currentTurnIndex() >= nValidTurn())
        return false;

    return true;
}
