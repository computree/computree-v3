#include "pb_actionvalidateinventoryattributesdialog.h"
#include "ui_pb_actionvalidateinventoryattributesdialog.h"

PB_ActionValidateInventoryAttributesDialog::PB_ActionValidateInventoryAttributesDialog(const QStringList &speciesList, const QString &speciesValue, const QString &idValue, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PB_ActionValidateInventoryAttributesDialog)
{
    ui->setupUi(this);

    ui->cb_species->addItems(speciesList);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        ui->cb_species->setCurrentIndex(ui->cb_species->findText(speciesValue));
#else
        ui->cb_species->setCurrentText(speciesValue);
#endif

    ui->le_id->setText(idValue);
}

PB_ActionValidateInventoryAttributesDialog::~PB_ActionValidateInventoryAttributesDialog()
{
    delete ui;
}

QString PB_ActionValidateInventoryAttributesDialog::getSpecies()
{
    return ui->cb_species->currentText();
}

QString PB_ActionValidateInventoryAttributesDialog::getId()
{
    return ui->le_id->text();
}
