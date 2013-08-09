#include "parameters/widgets/widgetfilechoice.h"
#include "ui_widgetfilechoice.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetFileChoice::WidgetFileChoice(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetFileChoice)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);

    ui->neededFileType->addItem("OneExistingFile");
    ui->neededFileType->addItem("OneOrMoreExistingFiles");
    ui->neededFileType->addItem("OneNewFile");
    ui->neededFileType->addItem("OneExistingFolder");
}

WidgetFileChoice::~WidgetFileChoice()
{
    delete ui;
}

bool WidgetFileChoice::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString WidgetFileChoice::getAlias()
{
    return ui->alias->text();
}

QString WidgetFileChoice::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetFileChoice::getNeededFileType()
{
    return ui->neededFileType->currentText();
}

QString WidgetFileChoice::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetFileChoice::getDescription()
{
    return ui->description->toPlainText().replace("\n","\\n");
}

void WidgetFileChoice::on_alias_textChanged(const QString &arg1)
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
