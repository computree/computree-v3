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

bool PB_ActionManualInventoryOptions::isSelectModeSelected()
{
    return ui->rb_select->isChecked();
}

bool PB_ActionManualInventoryOptions::isDbhModeSelected()
{
    return ui->rb_dbh->isChecked();
}

bool PB_ActionManualInventoryOptions::isAttributesModeSelected()
{
    return ui->rb_attributes->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowDataChecked()
{
    return ui->cb_showData->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowActiveCirclesChecked()
{
    return ui->cb_activeCircles->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowOtherCirclesChecked()
{
    return ui->cb_otherCircles->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowActiveSceneChecked()
{
    return ui->cb_activeScene->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowOtherScenesChecked()
{
    return ui->cb_otherScenes->isChecked();
}

void PB_ActionManualInventoryOptions::on_rb_select_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit modeChanged();
}

void PB_ActionManualInventoryOptions::on_rb_dbh_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit modeChanged();
}

void PB_ActionManualInventoryOptions::on_rb_attributes_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit modeChanged();
}

void PB_ActionManualInventoryOptions::on_cb_showData_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionManualInventoryOptions::on_cb_activeCircles_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionManualInventoryOptions::on_cb_otherCircles_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionManualInventoryOptions::on_cb_activeScene_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionManualInventoryOptions::on_cb_otherScenes_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}



