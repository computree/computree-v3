#include "parameters/widgets/widgettextline.h"
#include "ui_widgettextline.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetTextLine::WidgetTextLine(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetTextLine)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

WidgetTextLine::~WidgetTextLine()
{
    delete ui;
}

bool WidgetTextLine::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetTextLine::getAlias()
{
    return ui->alias->text();
}

QString WidgetTextLine::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetTextLine::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetTextLine::getCheckboxText()
{
    return ui->libCheckbox->text();
}

bool WidgetTextLine::getDefaultValue()
{
    return ui->defaultValue->isChecked();
}

QString WidgetTextLine::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetTextLine::on_alias_textChanged(const QString &arg1)
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
