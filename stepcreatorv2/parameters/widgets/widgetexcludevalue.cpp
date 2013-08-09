#include "parameters/widgets/widgetexcludevalue.h"
#include "ui_widgetexcludevalue.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetExcludeValue::WidgetExcludeValue(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetExcludeValue)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetExcludeValue::~WidgetExcludeValue()
{
    delete ui;
}

bool WidgetExcludeValue::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetExcludeValue::getAlias()
{
    return ui->alias->text();
}

QString WidgetExcludeValue::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetExcludeValue::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetExcludeValue::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetExcludeValue::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetExcludeValue::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetExcludeValue::on_alias_textChanged(const QString &arg1)
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
