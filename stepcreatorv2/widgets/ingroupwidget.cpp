#include "ingroupwidget.h"
#include "ui_ingroupwidget.h"

INGroupWidget::INGroupWidget(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::INGroupWidget)
{
    ui->setupUi(this);
    ui->rb_standard->setChecked(true);

    ui->cb_finderMode->addItem("Obligatory");
    ui->cb_finderMode->addItem("Optional");
    ui->cb_finderMode->setCurrentIndex(0);

    ui->cb_choiceMode->addItem("ChooseOneIfMultiple");
    ui->cb_choiceMode->addItem("ChooseMultipleIfMultiple");
    ui->cb_choiceMode->setCurrentIndex(0);
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
