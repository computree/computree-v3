#include "parameters/widgets/widgetemptyline.h"
#include "ui_widgetemptyline.h"
#include "parameters/abstractparameter.h"
#include "tools.h"

WidgetEmptyLine::WidgetEmptyLine(AbstractParameter* model, QWidget *parent) :
    AbstractParameterWidget(model, parent),
    ui(new Ui::WidgetEmptyLine)
{
    ui->setupUi(this);
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
    return "";
}
