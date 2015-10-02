#include "ctg_inresultmodelconfiguration.h"
#include "ui_ctg_inresultmodelconfiguration.h"

#include "ct_inmanager.h"

#include <QMessageBox>

CTG_InResultModelConfiguration::CTG_InResultModelConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTG_InResultModelConfiguration)
{
    ui->setupUi(this);

    _inManager = NULL;

    connect(ui->inResultModelPossibilities, SIGNAL(enableResultPossibility(QString)), ui->inTurnManager, SLOT(enableResultPossibility(QString)), Qt::DirectConnection);
    connect(ui->inResultModelPossibilities, SIGNAL(disableResultPossibility(QString)), ui->inTurnManager, SLOT(disableResultPossibility(QString)), Qt::DirectConnection);
    connect(ui->inResultModelPossibilities, SIGNAL(showResultPossibility(QString)), ui->inTurnManager, SLOT(showResultPossibility(QString)), Qt::DirectConnection);
}

CTG_InResultModelConfiguration::~CTG_InResultModelConfiguration()
{
    delete ui;
}

void CTG_InResultModelConfiguration::setInManager(const CT_InManager *manager)
{
    _inManager = (CT_InManager*)manager;

    ui->inResultModelPossibilities->setInTurnManager(ui->inTurnManager);
    ui->inResultModelPossibilities->setInResultModelManager(manager->getResultModelManager());
    ui->inTurnManager->setInTurnManager(manager->getTurnManager());
}

void CTG_InResultModelConfiguration::setReadOnly(bool enabled)
{
    ui->inResultModelPossibilities->setReadOnly(enabled);
    ui->inTurnManager->setReadOnly(enabled);
}

int CTG_InResultModelConfiguration::exec()
{
    ui->inTurnManager->reConstruct();

    return QDialog::exec();
}

void CTG_InResultModelConfiguration::accept()
{
    if(_inManager == NULL)
        return;

    QListIterator<CT_InTurn*> it(_inManager->getTurnManager()->turns());

    while(it.hasNext())
    {
        CT_InTurn *turn = it.next();
        turn->clearError();

        if(!turn->isValid())
        {
            QMessageBox::warning(this, tr("Avertissement"), tr("Vous n'avez pas correctement configuré "
                                                               "un tour, veuillez sélectionner des données "
                                                               "d'entrée.\r\n\r\n%1").arg(turn->errors()), QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}
