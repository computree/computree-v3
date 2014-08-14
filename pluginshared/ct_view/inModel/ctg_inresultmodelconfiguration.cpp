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

    connect(ui->inResultModelPossibilities, SIGNAL(resultChoosed(QString)), ui->inTurnManager, SLOT(setResultChoosed(QString)));
}

CTG_InResultModelConfiguration::~CTG_InResultModelConfiguration()
{
    delete ui;
}

void CTG_InResultModelConfiguration::setInManager(const CT_InManager *manager)
{
    _inManager = (CT_InManager*)manager;

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
        if(!it.next()->isValid())
        {
            QMessageBox::warning(this, "Avertissement", "Vous n'avez pas correctement configuré "
                                                        "un tour, veuillez sélectionner des données "
                                                        "d'entrée.", QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}
