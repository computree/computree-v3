#include "parameters/widgets/widgetdouble.h"
#include "ui_widgetdouble.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetDouble::WidgetDouble(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetDouble)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetDouble::~WidgetDouble()
{
    delete ui;
}

bool WidgetDouble::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetDouble::getAlias()
{
    return ui->alias->text();
}

QString WidgetDouble::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetDouble::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetDouble::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetDouble::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetDouble::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetDouble::on_alias_textChanged(const QString &arg1)
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
