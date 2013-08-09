#include "parameters/widgets/widgetemptyline.h"
#include "ui_widgetemptyline.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetEmptyLine::WidgetEmptyLine(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetEmptyLine)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetEmptyLine::~WidgetEmptyLine()
{
    delete ui;
}

bool WidgetEmptyLine::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetEmptyLine::getAlias()
{
    return ui->alias->text();
}

QString WidgetEmptyLine::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetEmptyLine::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetEmptyLine::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetEmptyLine::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetEmptyLine::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetEmptyLine::on_alias_textChanged(const QString &arg1)
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
