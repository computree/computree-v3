#include "outitemwidget.h"
#include "ui_outitemwidget.h"
#include "models/abstractoutmodel.h"

OUTItemWidget::OUTItemWidget(AbstractOutModel* model, QWidget *parent) :
    AbstractOutWidget(model, parent),
    ui(new Ui::OUTItemWidget)
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


OUTItemWidget::FinderMode OUTItemWidget::getFinderMode()
{
    if (ui->cb_finderMode->currentText()=="Obligatory")
    {
        return OUTItemWidget::F_Obligatory;
    } else
    {
        return OUTItemWidget::F_Optional;
    }
}

OUTItemWidget::ChoiceMode OUTItemWidget::getChoiceMode()
{
    if (ui->cb_choiceMode->currentText()=="ChooseOneIfMultiple")
    {
        return OUTItemWidget::C_OneIfMultiple;
    } else
    {
        return OUTItemWidget::C_MultipleIfMultiple;
    }
}

void OUTItemWidget::on_alias_textChanged(const QString &arg1)
{
    _model->onAliasChange();
}
