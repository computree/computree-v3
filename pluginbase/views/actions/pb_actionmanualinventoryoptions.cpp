#include "pb_actionmanualinventoryoptions.h"
#include "ui_pb_actionmanualinventoryoptions.h"

#include "actions/pb_actionmanualinventory.h"

#include <QColorDialog>

PB_ActionManualInventoryOptions::PB_ActionManualInventoryOptions(const PB_ActionManualInventory *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionManualInventoryOptions())
{
    ui->setupUi(this);
}

PB_ActionManualInventoryOptions::~PB_ActionManualInventoryOptions()
{
    delete ui;
}

bool PB_ActionManualInventoryOptions::shouldAutoCenterCamera()
{
    return ui->cb_centerCamera->isChecked();
}

bool PB_ActionManualInventoryOptions::shouldShowScenes()
{
    return ui->cb_showScenes->isChecked();
}

bool PB_ActionManualInventoryOptions::shouldShowData()
{
    return ui->cb_showData->isChecked();
}

PB_ActionManualInventoryOptions::Mode PB_ActionManualInventoryOptions::getMode()
{
    if (ui->rb_attributes->isChecked())
    {
        return Mode_attributes;
    }
    return Mode_dbh;
}

void PB_ActionManualInventoryOptions::toggleMode()
{
    if (ui->rb_dbh->isChecked())
    {
        ui->rb_attributes->setChecked(true);
    } else {
        ui->rb_dbh->setChecked(true);
    }
}

void PB_ActionManualInventoryOptions::on_rb_dbh_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}

void PB_ActionManualInventoryOptions::on_cb_showScenes_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}

void PB_ActionManualInventoryOptions::on_cb_showData_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}
