#include "pb_actionmanualinventoryattributesdialog.h"
#include "ui_pb_actionmanualinventoryattributesdialog.h"

PB_ActionManualInventoryAttributesDialog::PB_ActionManualInventoryAttributesDialog(const QStringList &speciesList, const QString &speciesValue, const QString &idValue, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PB_ActionManualInventoryAttributesDialog)
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

PB_ActionManualInventoryAttributesDialog::~PB_ActionManualInventoryAttributesDialog()
{
    delete ui;
}

QString PB_ActionManualInventoryAttributesDialog::getSpecies()
{
    return ui->cb_species->currentText();
}

QString PB_ActionManualInventoryAttributesDialog::getId()
{
    return ui->le_id->text();
}
