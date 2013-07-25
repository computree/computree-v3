#include "copyresultwidget.h"
#include "ui_copyresultwidget.h"
#include "models/abstractcopymodel.h"

COPYResultWidget::COPYResultWidget(AbstractCopyModel* model, QWidget *parent) :
    AbstractCopyWidget(model, parent),
    ui(new Ui::COPYResultWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);

}

COPYResultWidget::~COPYResultWidget()
{
    delete ui;
}

bool COPYResultWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString COPYResultWidget::getAlias()
{
    return ui->alias->text();
}

QString COPYResultWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString COPYResultWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

void COPYResultWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
