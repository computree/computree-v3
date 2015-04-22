#include "pb_actionmanualinventoryoptions.h"
#include "ui_pb_actionmanualinventoryoptions.h"

#include "actions/pb_actionmanualinventory.h"

#include <QColorDialog>

PB_ActionManualInventoryOptions::PB_ActionManualInventoryOptions(const PB_ActionManualInventory *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionManualInventoryOptions())
{
    ui->setupUi(this);

    ui->rb_select->setToolTip(tr("Séléction de la scène active, sans modification [S]"));
    ui->rb_dbh->setToolTip(tr("Choix d'un cercle pour une scène [D]"));
    ui->rb_attributes->setToolTip(tr("Ouverture de la saisie des attributs pour une scène [F] ou [C] pour la scène active"));
    ui->pb_upper->setToolTip(tr("Séléctionne le cercle supérieur (CTRL MOLETTE +)"));
    ui->pb_lower->setToolTip(tr("Séléctionne le cercle inférieur (CTRL MOLETTE -)"));
    ui->pb_toValidated->setToolTip(tr("Valide la scène active [V] ou [Fin]"));
}

PB_ActionManualInventoryOptions::~PB_ActionManualInventoryOptions()
{
    delete ui;
}

bool PB_ActionManualInventoryOptions::shouldAutoCenterCamera()
{
    return ui->cb_centerCamera->isChecked();
}

void PB_ActionManualInventoryOptions::chooseSelectMode()
{
    ui->rb_select->setChecked(true);
}

void PB_ActionManualInventoryOptions::chooseDbhMode()
{
    ui->rb_dbh->setChecked(true);
}

void PB_ActionManualInventoryOptions::chooseAttributesMode()
{
    ui->rb_attributes->setChecked(true);
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

bool PB_ActionManualInventoryOptions::isShowTrashChecked()
{
    return ui->cb_trash->isChecked();
}

bool PB_ActionManualInventoryOptions::isShowTrashedScenesChecked()
{
    return ui->cb_trashScenes->isChecked();
}

bool PB_ActionManualInventoryOptions::isAutoValidateChecked()
{
    return ui->cb_autoValidate->isChecked();
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

void PB_ActionManualInventoryOptions::on_pb_upper_clicked()
{
    emit chooseUpperCircle();
}

void PB_ActionManualInventoryOptions::on_pb_lower_clicked()
{
    emit chooseLowerCircle();
}

void PB_ActionManualInventoryOptions::on_pb_toTrash_clicked()
{
    emit sendToTrash();
}

void PB_ActionManualInventoryOptions::on_pb_fromTrash_clicked()
{
    emit retrieveFromTrash();
}

void PB_ActionManualInventoryOptions::on_pb_toValidated_clicked()
{
    emit sendToValidated();
}

void PB_ActionManualInventoryOptions::on_cb_trash_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionManualInventoryOptions::on_cb_trashScenes_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}
