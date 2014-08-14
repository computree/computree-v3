#include "ct_outmanager.h"

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"
#include "ct_virtualabstractstep.h"

CT_OutManager::CT_OutManager(CT_VirtualAbstractStep &step,
                             CT_InTurnManager &inTurnManager)
{
    _step = &step;
    _outTurnManager = new CT_OutTurnManager(_outResultModelManager);
    _outResultManager = new CT_OutResultManager(_outResultModelManager, *_outTurnManager);

    _inTurnManager = &inTurnManager;
}

CT_OutManager::~CT_OutManager()
{
    delete _outTurnManager;
    delete _outResultManager;
}

void CT_OutManager::setTurnIndexManager(const CT_TurnIndexManager *turnIndexManager)
{
    _outTurnManager->setTurnIndexManager(turnIndexManager);
}

CT_OutResultModelManager* CT_OutManager::getResultModelManager() const
{
    return (CT_OutResultModelManager*)&_outResultModelManager;
}

CT_OutTurnManager* CT_OutManager::getTurnManager() const
{
    return _outTurnManager;
}

CT_OutResultManager* CT_OutManager::getResultManager() const
{
    return _outResultManager;
}

bool CT_OutManager::createAllTurn()
{
    // si on a des résultats de sorties
    // et que le gestionnaire de tours n'a
    // pas encore de backup
    if((_outResultManager->nResults() > 0)
            && !_outTurnManager->hasBackup())
        _outTurnManager->backupTurns(); // on fait un backup (pour éviter d'avoir des problèmes si le résultat est toujours affiché dans une vue)

    _outTurnManager->clearTurnList();
    _outTurnManager->clearSearchModelList();

    _outResultModelManager.clearAllResultModels();

    int n = _inTurnManager->nValidTurn();

    if(n <= 0)
        return false;

    _inTurnManager->getTurnIndexManager()->resetTurnIndex();

    _step->preProcessCreateOutResultModelListProtected();

    for(int i=0; i<n; ++i)
    {
        _inTurnManager->createSearchModelListForCurrentTurn();

        // on demande à l'étape de créer ses modèles de sortie pour ce tour
        // il vont être ajouter au CT_ResultModelManager
        _step->createOutResultModelListProtected();

        // on demande de créer un nouveau tour
        // le CT_TurnManager va enlever les modèles ajoutés au CT_ResultModelManager
        // pour les insérer au nouveau tour

        // si il n'a pas pu créer de nouveau tour alors il y a une erreur
        if(_outTurnManager->createTurn(n > 1) == NULL)
        {
            // on supprime tout
            _outTurnManager->clearTurnList();
            _inTurnManager->clearSearchModelList();
            _inTurnManager->getTurnIndexManager()->resetTurnIndex();
            return false;
        }

        _inTurnManager->getTurnIndexManager()->incrementTurnIndex();
    }

    _inTurnManager->clearSearchModelList();
    _inTurnManager->getTurnIndexManager()->resetTurnIndex();

    return true;
}

void CT_OutManager::resultCleared()
{
    _outTurnManager->clearBackup();
}
