#include "ct_inresultmodelconfigurationmanager.h"

#include "ctg_inresultmodelconfiguration.h"

#include "ct_inmanager.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelselectionhelper.h"

#include "interfaces.h"

CT_InResultModelConfigurationManager::CT_InResultModelConfigurationManager(CT_InManager &inManager)
{
    _inManager = &inManager;
    _configDialog = NULL;
}

CT_InResultModelConfigurationManager::~CT_InResultModelConfigurationManager()
{
    delete _configDialog;
}

CT_InResultModelConfigurationManager::CreateDialogReturn CT_InResultModelConfigurationManager::createInResultModelConfigurationDialog()
{
    CT_InResultModelConfigurationManager::CreateDialogReturn returnVal = CT_InResultModelConfigurationManager::CreateSuccess;

    // si on a pas plusieurs possibilités il n'y a rien a configurer
    if(!checkIfMustCreateOrShowConfigurationDialog())
        returnVal = CT_InResultModelConfigurationManager::CreateNotNecessary;

    if(_configDialog == NULL)
    {
        _configDialog = new CTG_InResultModelConfiguration(NULL);
        _configDialog->setWindowTitle(_configDialog->windowTitle() + QString(" (%1)").arg(_inManager->step()->getStepCustomName() == _inManager->step()->getStepDisplayableName() ? _inManager->step()->getStepExtendedDisplayableName() : _inManager->step()->getStepCustomName()));
        _configDialog->setInManager(_inManager);
        _configDialog->setWindowFlags(_configDialog->windowFlags() | Qt::WindowMaximizeButtonHint);
    }

    return returnVal;
}

CT_InResultModelConfigurationManager::ConfigureReturn CT_InResultModelConfigurationManager::configureInResultModel()
{
    if(checkIfMustCreateOrShowConfigurationDialog())
    {
        // on crée un tour par défaut si il n'y en a pas
        if(_inManager->getTurnManager()->nTurn() == 0)
            _inManager->getTurnManager()->createTurn();

        return (_configDialog->exec() == QDialog::Accepted) ? CT_InResultModelConfigurationManager::ConfigureSuccess : CT_InResultModelConfigurationManager::ConfigureCanceled;
    }

    _inManager->getTurnManager()->clearTurnList();

    // creation d'un tour par défaut avec tous les éléments sélectionné puisqu'il n'y a rien à configurer
    CT_InTurn *turn = _inManager->getTurnManager()->createTurn();

    QListIterator<CT_InAbstractResultModel*> it(turn->models());

    while(it.hasNext())
    {
        CT_InAbstractResultModel *model = it.next();

        CT_ModelSelectionHelper selectionHelper(model);

        bool ok = selectionHelper.selectAllPossibilitiesByDefault();

        Q_ASSERT(ok);

        if(!ok)
            return CT_InResultModelConfigurationManager::ConfigureError;
    }

    return CT_InResultModelConfigurationManager::ConfigureSuccess;
}

void CT_InResultModelConfigurationManager::showReadOnlyInResultModel()
{
    if(_configDialog != NULL)
    {
        _configDialog->setReadOnly(true);
        _configDialog->exec();
    }
}

QList<SettingsNodeGroup*> CT_InResultModelConfigurationManager::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *root = new SettingsNodeGroup("Values");
    root->addValue(new SettingsNodeValue("Version", 1));
    retList.append(root);

    QListIterator<CT_InTurn*> it(_inManager->getTurnManager()->turns());

    while(it.hasNext())
    {
        CT_InTurn *turn = it.next();

        if(turn->isValid())
        {
            SettingsNodeGroup *tGroup = new SettingsNodeGroup("Turn");
            QList<SettingsNodeGroup*> groups = turn->getAllValues();

            while(!groups.isEmpty())
                tGroup->addGroup(groups.takeFirst());

            root->addGroup(tGroup);
        }
    }

    return retList;
}

bool CT_InResultModelConfigurationManager::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(list.isEmpty()
            || (list.first()->name() != "Values"))
        return false;

    SettingsNodeGroup *root = list.first();

    // les infos des tours
    QList<SettingsNodeGroup*> groups = root->groupsByTagName("Turn");

    int nTurn = groups.size();

    if((nTurn == 0)
            || (nTurn > _inManager->getTurnManager()->maximumTurn()))
        return false;

    // on supprime tous ce qu'on a fait avant
    _inManager->getTurnManager()->clearTurnList();
    delete _configDialog;
    _configDialog = NULL;

    // on crée la fenêtre de config si il y en a besoin
    createInResultModelConfigurationDialog();

    for(int i=0; i<nTurn; ++i)
    {
        // on crée un tour
        CT_InTurn *turn = _inManager->getTurnManager()->createTurn();

        if((turn == NULL)
            || !turn->setAllValues(groups.at(i)->groups()))
        {
            _inManager->getTurnManager()->clearTurnList();
            delete _configDialog;
            _configDialog = NULL;

            return false;
        }
    }

    return true;
}

bool CT_InResultModelConfigurationManager::checkIfMustCreateOrShowConfigurationDialog() const
{
    // for each inResultModel
    QListIterator<CT_InAbstractResultModel*> it(_inManager->getResultModelManager()->models());

    while(it.hasNext())
    {
        CT_InAbstractResultModel *model = it.next();

        CT_ModelSelectionHelper selectionHelper(model);

        // we check if it can select all possibilities by default
        if(!selectionHelper.canSelectAllPossibilitiesByDefault())
            return true; // if not we return true to inform that the dialog must be created or displayed
    }

    return false;
}

void CT_InResultModelConfigurationManager::aboutToBeDeleted()
{
    delete _configDialog;
    _configDialog = NULL;
}
