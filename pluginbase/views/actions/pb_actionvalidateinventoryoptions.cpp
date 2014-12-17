#include "pb_actionvalidateinventoryoptions.h"
#include "ui_pb_actionvalidateinventoryoptions.h"

#include "actions/pb_actionvalidateinventory.h"

#include <QColorDialog>

PB_ActionValidateInventoryOptions::PB_ActionValidateInventoryOptions(const PB_ActionValidateInventory *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionValidateInventoryOptions())
{
    ui->setupUi(this);

    connect(ui->pb_reset, SIGNAL(clicked()), this, SIGNAL(askForCurrentItemReset()));
}

PB_ActionValidateInventoryOptions::~PB_ActionValidateInventoryOptions()
{
    delete ui;
}

bool PB_ActionValidateInventoryOptions::shouldAutoCenterCamera()
{
    return ui->cb_centerCamera->isChecked();
}

bool PB_ActionValidateInventoryOptions::shouldShowData()
{
    return ui->cb_showData->isChecked();
}

bool PB_ActionValidateInventoryOptions::shouldShowCandidateItems()
{
    return ui->cb_showCandidateItems->isChecked();
}

PB_ActionValidateInventoryOptions::Mode PB_ActionValidateInventoryOptions::getMode()
{
    if (ui->rb_attributes->isChecked())
    {
        return Mode_attributes;
    }
    return Mode_Item;
}

void PB_ActionValidateInventoryOptions::toggleMode()
{
    if (ui->rb_dbh->isChecked())
    {
        ui->rb_attributes->setChecked(true);
    } else {
        ui->rb_dbh->setChecked(true);
    }
}

void PB_ActionValidateInventoryOptions::on_rb_dbh_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}

void PB_ActionValidateInventoryOptions::on_cb_showData_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}



void PB_ActionValidateInventoryOptions::on_cb_showCandidateItems_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit parametersChanged();
}

