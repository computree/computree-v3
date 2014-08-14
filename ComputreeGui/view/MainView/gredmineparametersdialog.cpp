#include "gredmineparametersdialog.h"
#include "ui_gredmineparametersdialog.h"

#include <QFileDialog>

GRedmineParametersDialog::GRedmineParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GRedmineParametersDialog)
{
    ui->setupUi(this);
}

GRedmineParametersDialog::~GRedmineParametersDialog()
{
    delete ui;
}

QString GRedmineParametersDialog::getDirectory()
{
    return ui->lb_directory->text();
}

QString GRedmineParametersDialog::getId()
{
    return ui->id->text();
}

QString GRedmineParametersDialog::getPassword()
{
    return ui->password->text();
}

bool GRedmineParametersDialog::isProxyActivated()
{
    return ui->cb_proxy->isChecked();
}

QString GRedmineParametersDialog::getProxyAdress()
{
    return ui->proxyAdress->text();
}

QString GRedmineParametersDialog::getProxyId()
{
    return ui->proxyId->text();
}

QString GRedmineParametersDialog::getProxyPassword()
{
    return ui->proxyPassword->text();
}

GRedmineParametersDialog::SystemType GRedmineParametersDialog::getSystemType()
{
    if (ui->rb_windows->isChecked()) {return GRedmineParametersDialog::Windows;}
    return GRedmineParametersDialog::Linux;
}

void GRedmineParametersDialog::on_pb_directory_clicked()
{
    QString s = QFileDialog::getExistingDirectory(0, tr("Choisir un répertoire d'export"), "");
    if (QDir(s).exists())
    {
        ui->lb_directory->setText(s);
    } else {
        ui->lb_directory->setText("");
    }
}
