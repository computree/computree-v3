#include "ingroupwidget.h"
#include "ui_ingroupwidget.h"
#include "models/abstractinmodel.h"

INGroupWidget::INGroupWidget(AbstractInModel* model, QWidget *parent) :
    AbstractInWidget(model, parent),
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
    ui->cb_choiceMode->setCurrentIndex(2);

    setFocusProxy(ui->alias);
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
    return ui->modelDescription->toPlainText().replace("\n","\\n");
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
        ui->modelName->clear();
        ui->modelName->setEnabled(true);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(true);
        ui->cb_finderMode->setEnabled(true);
        ui->cb_choiceMode->setEnabled(true);
    } else {
        ui->modelName->clear();
        ui->modelName->setEnabled(false);
        ui->modelDescription->clear();
        ui->modelDescription->setEnabled(false);
        ui->cb_finderMode->setEnabled(false);
        ui->cb_choiceMode->setEnabled(false);
    }
}

void INGroupWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
