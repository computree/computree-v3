#include "outresultwidget.h"
#include "ui_outresultwidget.h"
#include "models/abstractoutmodel.h"

OUTResultWidget::OUTResultWidget(AbstractOutModel* model, QWidget *parent) :
    AbstractOutWidget(model, parent),
    ui(new Ui::OUTResultWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);

}

OUTResultWidget::~OUTResultWidget()
{
    delete ui;
}

bool OUTResultWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString OUTResultWidget::getAlias()
{
    return ui->alias->text();
}

QString OUTResultWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString OUTResultWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

void OUTResultWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
