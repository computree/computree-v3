#include "parameters/widgets/widgetstring.h"
#include "ui_widgetstring.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetString::WidgetString(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetString)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetString::~WidgetString()
{
    delete ui;
}

bool WidgetString::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetString::getAlias()
{
    return ui->alias->text();
}

QString WidgetString::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetString::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetString::getDefaultValue()
{
    return ui->defaultValue->text();
}

QString WidgetString::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetString::on_alias_textChanged(const QString &arg1)
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
