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
    if (getName().isEmpty()) {return false;}
    return true;
}

QString WidgetBool::getName()
{
    return ui->alias->text();
}

