#include "ingroupwidget.h"
#include "ui_ingroupwidget.h"

INGroupWidget::INGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::INGroupWidget)
{
    ui->setupUi(this);
    ui->rb_standard->setChecked(true);
}

INGroupWidget::~INGroupWidget()
{
    delete ui;
}



void INGroupWidget::on_rb_standard_toggled(bool checked)
{
    if (checked)
    {
        ui->alias->clear();
        ui->alias->setEnabled(true);
        ui->modelName->clear();
        ui->modelName->setEnabled(true);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(true);
    } else {
        ui->alias->clear();
        ui->alias->setEnabled(false);
        ui->modelName->clear();
        ui->modelName->setEnabled(false);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(false);
    }
}
