#include "parameters/widgets/widgetstringchoice.h"
#include "ui_widgetstringchoice.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetStringChoice::WidgetStringChoice(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetStringChoice)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetStringChoice::~WidgetStringChoice()
{
    delete ui;
}

bool WidgetStringChoice::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetStringChoice::getAlias()
{
    return ui->alias->text();
}

QString WidgetStringChoice::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetStringChoice::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetStringChoice::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetStringChoice::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetStringChoice::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetStringChoice::on_alias_textChanged(const QString &arg1)
{
    if (arg1.size()>0)
    {
        if (arg1.at(0)!='_')
        {
            ui->alias->setText("_" + arg1);
        }
    }
    _model->onAliasChange();
}
