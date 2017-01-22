#include "gcitationdialog.h"
#include "ui_gcitationdialog.h"

GCitationDialog::GCitationDialog(QString pluginsAndStepsText, QString bibTexText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GCitationDialog)
{
    ui->setupUi(this);

    ui->te_pluginsAndSteps->setText(pluginsAndStepsText);
    ui->te_bibTex->setText(bibTexText);
}

GCitationDialog::~GCitationDialog()
{
    delete ui;
}
