#include "inresultwidget.h"
#include "ui_inresultwidget.h"

INResultWidget::INResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::INResultWidget)
{
    ui->setupUi(this);
    ui->rb_notneededresult->setChecked(true);
}

INResultWidget::~INResultWidget()
{
    delete ui;
}

void INResultWidget::on_rb_notneededresult_toggled(bool checked)
{
    if (checked)
    {
        ui->alias->clear();
        ui->alias->setEnabled(false);
        ui->resultName->clear();
        ui->resultName->setEnabled(false);
        ui->modelName->clear();
        ui->modelName->setEnabled(false);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(false);
    } else {
        ui->resultName->clear();
        ui->resultName->setEnabled(true);
        ui->alias->clear();
        ui->alias->setEnabled(true);
        ui->modelName->clear();
        ui->modelName->setEnabled(true);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(true);
    }
}
