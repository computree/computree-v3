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
    ui->cb_choiceMode->addItem("DontChoose");
    ui->cb_choiceMode->setCurrentIndex(0);
}

INGroupWidget::~INGroupWidget()
{
    delete ui;
}


bool INGroupWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}


QString INGroupWidget::getAlias()
{
    return ui->alias->text();
}

QString INGroupWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString INGroupWidget::getDescription()
{
    return ui->modelDescription->toPlainText();
}

INGroupWidget::GroupType  INGroupWidget::getResultType()
{
    if (ui->rb_zeroormore->isChecked())
    {
        return INGroupWidget::G_ZeroOrMore;
    } else if (ui->rb_oneormore->isChecked())
    {
        return INGroupWidget::G_OneOrMore;
    } else
    {
        return INGroupWidget::G_Sandard;
    }
}
INGroupWidget::FinderMode INGroupWidget::getFinderMode()
{
    if (ui->cb_finderMode->currentText()=="Obligatory")
    {
        return INGroupWidget::F_Obligatory;
    } else
    {
        return INGroupWidget::F_Optional;
    }
}

INGroupWidget::ChoiceMode INGroupWidget::getChoiceMode()
{
    if (ui->cb_choiceMode->currentText()=="ChooseOneIfMultiple")
    {
        return INGroupWidget::C_OneIfMultiple;
    } else if (ui->cb_choiceMode->currentText()=="ChooseMultipleIfMultiple")
    {
        return INGroupWidget::C_MultipleIfMultiple;
    } else
    {
        return INGroupWidget::C_DontChoose;
    }
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
