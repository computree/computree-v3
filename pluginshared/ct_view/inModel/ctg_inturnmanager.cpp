#include "ctg_inturnmanager.h"
#include "ui_ctg_inturnmanager.h"

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"
#include "ctg_inresultmodelturnchoice.h"
#include "ctg_inmodelpossibilitieschoice.h"

#include <QMessageBox>
#include <QVBoxLayout>

CTG_InTurnManager::CTG_InTurnManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTG_InTurnManager)
{
    ui->setupUi(this);

    _manager = NULL;
    m_readOnly = false;
    m_canAddANewTab = false;

    ui->tabWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *removeAction = new QAction("Supprimer", ui->tabWidget);

    ui->tabWidget->addAction(removeAction);

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)), Qt::DirectConnection);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeCurrentTurn()));
}

CTG_InTurnManager::~CTG_InTurnManager()
{
    delete ui;
}

void CTG_InTurnManager::setInTurnManager(const CT_InTurnManager *manager)
{
    _manager = (CT_InTurnManager*)manager;

    constructTabs();
}

void CTG_InTurnManager::reConstruct()
{
    constructTabs();
}

void CTG_InTurnManager::setReadOnly(bool enabled)
{
    m_readOnly = enabled;
}

CT_InAbstractResultModel *CTG_InTurnManager::getModelFromMimeDataForCurrentTurn(const QString &mimeData)
{
    int index = ui->tabWidget->currentIndex();
    QWidget *wid = ui->tabWidget->widget(index);

    int count = wid->layout()->count();

    for(int i=0; i<count; ++i)
    {
        CTG_InResultModelTurnChoice *turnChoice = dynamic_cast<CTG_InResultModelTurnChoice*>(wid->layout()->itemAt(i)->widget());

        if(turnChoice != NULL)
            return turnChoice->getModelFromMimeData(mimeData);
    }

    return NULL;
}

CT_InStdModelPossibility *CTG_InTurnManager::getPossibilityFromMimeDataForCurrentTurn(const QString &mimeData, int *outPossibilityIndex)
{
    int index = ui->tabWidget->currentIndex();
    QWidget *wid = ui->tabWidget->widget(index);

    if(wid != NULL) {
        int count = wid->layout()->count();

        for(int i=0; i<count; ++i)
        {
            CTG_InResultModelTurnChoice *turnChoice = dynamic_cast<CTG_InResultModelTurnChoice*>(wid->layout()->itemAt(i)->widget());

            if(turnChoice != NULL)
                return turnChoice->getPossibilityFromMimeData(mimeData, outPossibilityIndex);
        }
    }

    return NULL;
}

void CTG_InTurnManager::clearTabs()
{
    while(ui->tabWidget->count() > 0)
        delete ui->tabWidget->widget(0);
}

void CTG_InTurnManager::constructTabs()
{
    _constructEnabled = true;

    clearTabs();

    if(_manager != NULL)
    {
        const QList<CT_InTurn*> &turns = _manager->turns();
        QListIterator<CT_InTurn*> it(turns);

        int i = 1;

        while(it.hasNext())
        {
            QWidget *wid = createTabForTurn(it.next(), i);

            if(wid != NULL)
                ui->tabWidget->addTab(wid, wid->windowTitle());

            ++i;
        }

        if(m_canAddANewTab && (ui->tabWidget->count() < _manager->maximumTurn()))
            ui->tabWidget->addTab(new QWidget(), "+");
    }

    _constructEnabled = false;

    _lastIndex = ui->tabWidget->currentIndex();
}

QWidget* CTG_InTurnManager::createTabForTurn(CT_InTurn *turn, int index)
{
    QWidget *wid = new QWidget(NULL);
    //wid->setWindowTitle(QString("Tour %1").arg(index));
    wid->setWindowTitle(tr("Modèles"));

    QVBoxLayout *layout = new QVBoxLayout();

    CTG_InResultModelTurnChoice *turnChoice = new CTG_InResultModelTurnChoice(wid);
    turnChoice->setReadOnly(m_readOnly);
    turnChoice->setTurn(turn);
    layout->addWidget(turnChoice);
    turnChoice->setVisible(false);

    CTG_InModelPossibilitiesChoice *pChoice = new CTG_InModelPossibilitiesChoice(wid);
    pChoice->setReadOnly(m_readOnly);
    layout->addWidget(pChoice);

    wid->setLayout(layout);

    connect(turnChoice, SIGNAL(selectedPossibilityChanged(const CT_InStdResultModelPossibility*)), pChoice, SLOT(setInResultModelPossibility(const CT_InStdResultModelPossibility*)));

    return wid;
}

bool CTG_InTurnManager::isAddTab(int index) const
{
    if(m_canAddANewTab) {
        int count = ui->tabWidget->count();

        if((index > 0)
                && (index == (count-1)))
            return true;
    }

    return false;
}

void CTG_InTurnManager::setResultPossibility(const QString &mimeData, CTG_InTurnManager::ActionType action)
{
    int index = ui->tabWidget->currentIndex();
    QWidget *wid = ui->tabWidget->widget(index);

    int count = wid->layout()->count();

    for(int i=0; i<count; ++i)
    {
        CTG_InResultModelTurnChoice *turnChoice = dynamic_cast<CTG_InResultModelTurnChoice*>(wid->layout()->itemAt(i)->widget());

        if(turnChoice != NULL)
        {
            if(action == Enable)
                turnChoice->enableResultPossibility(mimeData);
            else if(action == Disable)
                turnChoice->disableResultPossibility(mimeData);
            else
                turnChoice->showResultPossibility(mimeData);

            return;
        }
    }
}

void CTG_InTurnManager::currentTabChanged(int index)
{
    if(!_constructEnabled)
    {
        int count = ui->tabWidget->count();

        if(isAddTab(index)
                && !m_readOnly
                && (_manager != NULL)
                && (count < _manager->maximumTurn()))
        {
            CT_InTurn *newTurn = _manager->createTurn();

            if(newTurn != NULL)
            {
                QWidget *wid = createTabForTurn(newTurn, index+1);

                if(wid != NULL)
                {
                    ui->tabWidget->insertTab(index, wid, wid->windowTitle());
                    ++count;

                    if(count == _manager->maximumTurn())
                        delete ui->tabWidget->widget(count-1);

                    ui->tabWidget->setCurrentIndex(index);
                    _lastIndex = index;
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Avertissement"), tr("Impossible de créer un nouveau tour car le dernier "
                                                            "n'est pas valide. Veuillez sélectionner au moins un élément "
                                                            "pour chaque résultat puis ré-essayez."), QMessageBox::Ok);

                ui->tabWidget->setCurrentIndex(_lastIndex);
            }
        }
        else
        {
            _lastIndex = index;
        }
    }
}

void CTG_InTurnManager::removeCurrentTurn()
{
    int index = ui->tabWidget->currentIndex();

    if(!isAddTab(index))
    {
        if(_manager->nTurn() > 1)
        {
            _manager->removeTurn(index);

            reConstruct();
        }
        else
        {
            QMessageBox::warning(this, tr("Avertissement"), tr("Vous ne pouvez pas supprimer le tour car il en faut au moins un."), QMessageBox::Ok);
        }
    }
}

void CTG_InTurnManager::enableResultPossibility(const QString &mimeData)
{
    setResultPossibility(mimeData, Enable);
}

void CTG_InTurnManager::disableResultPossibility(const QString &mimeData)
{
    setResultPossibility(mimeData, Disable);
}

void CTG_InTurnManager::showResultPossibility(const QString &mimeData)
{
    setResultPossibility(mimeData, Show);
}
