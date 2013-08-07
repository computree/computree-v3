#include "copyitemwidget.h"
#include "ui_copyitemwidget.h"
#include "models/abstractcopymodel.h"
#include "tools.h"

COPYItemWidget::COPYItemWidget(AbstractCopyModel* model, QWidget *parent) :
    AbstractCopyWidget(model, parent),
    ui(new Ui::COPYItemWidget)
{
    ui->setupUi(this);

    // Ajout des classes d'itemdrawables disponibles
    int size = Tools::ITEMTYPE.size();
    for (int i = 0 ; i < size ; i++)
    {
        ui->cb_itemType->addItem(Tools::ITEMTYPE.at(i));
    }

    ui->cb_itemType->setCurrentIndex(0);

    setFocusProxy(ui->alias);
}

void COPYItemWidget::init(QString itemType, QString alias, QString name, QString desc)
{
    ui->alias->setText(alias);
    ui->alias->setEnabled(false);
    ui->modelName->setText(name);
    ui->modelName->setEnabled(false);
    ui->modelDescription->setPlainText(desc);
    ui->modelDescription->setEnabled(false);
    ui->cb_itemType->addItem(itemType);
    ui->cb_itemType->setCurrentIndex(0);
    ui->cb_itemType->setEnabled(false);
}

COPYItemWidget::~COPYItemWidget()
{
    delete ui;
}

bool COPYItemWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString COPYItemWidget::getItemType()
{
    return ui->cb_itemType->currentText();
}


QString COPYItemWidget::getAlias()
{
    return ui->alias->text();
}

QString COPYItemWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString COPYItemWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}


void COPYItemWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
