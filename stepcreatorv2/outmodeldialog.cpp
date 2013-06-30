#include "outmodeldialog.h"
#include "ui_outmodeldialog.h"

OUTModelDialog::OUTModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OUTModelDialog)
{
    ui->setupUi(this);
}

OUTModelDialog::~OUTModelDialog()
{
    delete ui;
}
