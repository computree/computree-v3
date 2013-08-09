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

double WidgetDouble::getMin()
{
    return ui->min->value();
}

double WidgetDouble::getMax()
{
    return ui->max->value();
}

int WidgetDouble::getDec()
{
    return ui->nbDec->value();
}

double WidgetDouble::getMult()
{
    return ui->mult->value();
}

double WidgetDouble::getDefaultValue()
{
    return ui->defaultValue->value();
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

void WidgetDouble::on_nbDec_valueChanged(int arg1)
{
    ui->min->setDecimals(arg1);
    ui->max->setDecimals(arg1);
    ui->defaultValue->setDecimals(arg1);
}

void WidgetDouble::on_min_valueChanged(double arg1)
{
    ui->defaultValue->setMinimum(arg1);
}

void WidgetDouble::on_max_valueChanged(double arg1)
{
    ui->defaultValue->setMaximum(arg1);
}
