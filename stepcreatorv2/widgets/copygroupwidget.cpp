#include "copygroupwidget.h"
#include "ui_copygroupwidget.h"
#include "models/abstractcopymodel.h"

COPYGroupWidget::COPYGroupWidget(AbstractCopyModel* model, QWidget *parent) :
    AbstractCopyWidget(model, parent),
    ui(new Ui::COPYGroupWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
}

COPYGroupWidget::COPYGroupWidget(AbstractCopyModel* model, QString alias, QString name, QString desc, QWidget *parent) :
    AbstractCopyWidget(model, parent),
    ui(new Ui::COPYGroupWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->alias);
    ui->alias->setText(alias);
    ui->alias->setEnabled(false);
    ui->modelName->setText(name);
    ui->modelName->setEnabled(false);
    ui->modelDescription->setPlainText(desc);
    ui->modelDescription->setEnabled(false);
}


COPYGroupWidget::~COPYGroupWidget()
{
    delete ui;
}


bool COPYGroupWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString COPYGroupWidget::getAlias()
{
    return ui->alias->text();
}

QString COPYGroupWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString COPYGroupWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}

void COPYGroupWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
