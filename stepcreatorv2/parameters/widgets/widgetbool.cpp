#include "parameters/widgets/widgetbool.h"
#include "ui_widgetbool.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetBool::WidgetBool(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetBool)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetBool::~WidgetBool()
{
    delete ui;
}

bool WidgetBool::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetBool::getAlias()
{
    return ui->alias->text();
}

QString WidgetBool::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetBool::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetBool::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetBool::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetBool::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetBool::on_alias_textChanged(const QString &arg1)
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