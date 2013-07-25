#include "outgroupwidget.h"
#include "ui_outgroupwidget.h"
#include "models/abstractoutmodel.h"

OUTGroupWidget::OUTGroupWidget(AbstractOutModel* model, QWidget *parent) :
    AbstractOutWidget(model, parent),
    ui(new Ui::OUTGroupWidget)
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

OUTGroupWidget::~OUTGroupWidget()
{
    delete ui;
}


bool OUTGroupWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString OUTGroupWidget::getAlias()
{
    return ui->alias->text();
}

QString OUTGroupWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString OUTGroupWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

OUTGroupWidget::GroupType  OUTGroupWidget::getResultType()
{
    if (ui->rb_zeroormore->isChecked())
    {
        return OUTGroupWidget::G_ZeroOrMore;
    } else if (ui->rb_oneormore->isChecked())
    {
        return OUTGroupWidget::G_OneOrMore;
    } else
    {
        return OUTGroupWidget::G_Sandard;
    }
}
OUTGroupWidget::FinderMode OUTGroupWidget::getFinderMode()
{
    if (ui->cb_finderMode->currentText()=="Obligatory")
    {
        return OUTGroupWidget::F_Obligatory;
    } else
    {
        return OUTGroupWidget::F_Optional;
    }
}

OUTGroupWidget::ChoiceMode OUTGroupWidget::getChoiceMode()
{
    if (ui->cb_choiceMode->currentText()=="ChooseOneIfMultiple")
    {
        return OUTGroupWidget::C_OneIfMultiple;
    } else if (ui->cb_choiceMode->currentText()=="ChooseMultipleIfMultiple")
    {
        return OUTGroupWidget::C_MultipleIfMultiple;
    } else
    {
        return OUTGroupWidget::C_DontChoose;
    }
}

void OUTGroupWidget::on_rb_standard_toggled(bool checked)
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

void OUTGroupWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
