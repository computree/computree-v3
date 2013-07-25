#include "outitemwidget.h"
#include "ui_outitemwidget.h"
#include "models/abstractoutmodel.h"
#include "tools.h"

OUTItemWidget::OUTItemWidget(AbstractOutModel* model, QWidget *parent) :
    AbstractOutWidget(model, parent),
    ui(new Ui::OUTItemWidget)
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

OUTItemWidget::~OUTItemWidget()
{
    delete ui;
}

bool OUTItemWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString OUTItemWidget::getItemType()
{
    return ui->cb_itemType->currentText();
}


QString OUTItemWidget::getAlias()
{
    return ui->alias->text();
}

QString OUTItemWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString OUTItemWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}


void OUTItemWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
