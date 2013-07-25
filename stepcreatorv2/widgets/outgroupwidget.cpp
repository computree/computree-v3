#include "outgroupwidget.h"
#include "ui_outgroupwidget.h"
#include "models/abstractoutmodel.h"

OUTGroupWidget::OUTGroupWidget(AbstractOutModel* model, QWidget *parent) :
    AbstractOutWidget(model, parent),
    ui(new Ui::OUTGroupWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

OUTGroupWidget::~OUTGroupWidget()
{
    delete ui;
}


bool OUTGroupWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString OUTGroupWidget::getAlias()
{
    return ui->alias->text();
}

QString OUTGroupWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString OUTGroupWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

void OUTGroupWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
