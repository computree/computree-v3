#include "parameters/widgets/widgettextline.h"
#include "ui_widgettextline.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetTextLine::WidgetTextLine(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetTextLine)
{
    ui->setupUi(this);
    setFocusProxy(ui->libBefore);
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
    return "";
}

QString WidgetTextLine::getBeforeText()
{
    return ui->libBefore->text();
}

QString WidgetTextLine::getAfterText()
{
    return ui->libAfter->text();
}

QString WidgetTextLine::getMiddleText()
{
    return ui->libMiddle->text();
}

void WidgetTextLine::on_libBefore_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
