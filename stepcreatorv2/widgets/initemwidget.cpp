#include "initemwidget.h"
#include "ui_initemwidget.h"
#include "models/abstractinmodel.h"

INItemWidget::INItemWidget(AbstractInModel* model, QWidget *parent) :
    AbstractInWidget(model, parent),
    ui(new Ui::INItemWidget)
{
    ui->setupUi(this);

    ui->cb_finderMode->addItem("Obligatory");
    ui->cb_finderMode->addItem("Optional");
    ui->cb_finderMode->setCurrentIndex(0);

    ui->cb_choiceMode->addItem("ChooseOneIfMultiple");
    ui->cb_choiceMode->addItem("ChooseMultipleIfMultiple");
    ui->cb_choiceMode->setCurrentIndex(0);

    // Ajout des classes d'itemdrawables disponibles
    ui->cb_itemType->addItem("CT_AbstractItemDrawableWithPointCloud");
    ui->cb_itemType->addItem("CT_Scene");
    ui->cb_itemType->addItem("CT_PointCluster");
    ui->cb_itemType->addItem("CT_PolyLine");

    ui->cb_itemType->addItem("CT_AbstractItemDrawableWithoutPointCloud");
    ui->cb_itemType->addItem("CT_ReferencePoint");
    ui->cb_itemType->addItem("CT_AbstractShape");
    ui->cb_itemType->addItem("CT_Circle");
    ui->cb_itemType->addItem("CT_Ellipse");
    ui->cb_itemType->addItem("CT_Line");
    ui->cb_itemType->addItem("CT_Cylinder");
    ui->cb_itemType->addItem("CT_Raster2DFloat");
    ui->cb_itemType->addItem("CT_Raster2DInt");
    ui->cb_itemType->addItem("CT_RectangularGridDataFloat");
    ui->cb_itemType->addItem("CT_RectangularGridDataInt");
    ui->cb_itemType->addItem("CT_RectangularGridDataDouble");
    ui->cb_itemType->addItem("CT_RectangularGridDataBool");
    ui->cb_itemType->addItem("CT_Ray");
    ui->cb_itemType->addItem("CT_Scanner");
    ui->cb_itemType->addItem("CT_Triangulation2D");
    ui->cb_itemType->setCurrentIndex(0);

    setFocusProxy(ui->alias);
}

INItemWidget::~INItemWidget()
{
    delete ui;
}

bool INItemWidget::isvalid()
{
    if (getAlias().isEmpty()) {return false;}
    return true;
}

QString INItemWidget::getItemType()
{
    return ui->cb_itemType->currentText();
}


QString INItemWidget::getAlias()
{
    return ui->alias->text();
}

QString INItemWidget::getDisplayableName()
{
    return ui->modelName->text();
}

QString INItemWidget::getDescription()
{
    return ui->modelDescription->toPlainText().replace("\n","\\n");
}


INItemWidget::FinderMode INItemWidget::getFinderMode()
{
    if (ui->cb_finderMode->currentText()=="Obligatory")
    {
        return INItemWidget::F_Obligatory;
    } else
    {
        return INItemWidget::F_Optional;
    }
}

INItemWidget::ChoiceMode INItemWidget::getChoiceMode()
{
    if (ui->cb_choiceMode->currentText()=="ChooseOneIfMultiple")
    {
        return INItemWidget::C_OneIfMultiple;
    } else
    {
        return INItemWidget::C_MultipleIfMultiple;
    }
}

void INItemWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
