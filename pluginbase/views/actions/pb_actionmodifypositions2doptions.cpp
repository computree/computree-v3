#include "pb_actionmodifypositions2doptions.h"
#include "ui_pb_actionmodifypositions2doptions.h"

#include "actions/pb_actionmodifypositions2d.h"

#include <QColorDialog>

PB_ActionModifyPositions2DOptions::PB_ActionModifyPositions2DOptions(const PB_ActionModifyPositions2D *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionModifyPositions2DOptions())
{
    ui->setupUi(this);

    ui->pb_freeMove->setToolTip(tr("Déplacement normal dans la vue"));
    ui->pb_move->setToolTip(tr("Modification d'une position [M maintenu]"));
    ui->pb_add->setToolTip(tr("Ajout d'une position [P maintenu]"));
    ui->pb_remove->setToolTip(tr("Suppression d'une position [SUPPR avant chaque suppression]"));

    connect(ui->buttonGroup_step, SIGNAL(buttonClicked(int)), this, SLOT(singleStepChanged(int)));
}

PB_ActionModifyPositions2DOptions::~PB_ActionModifyPositions2DOptions()
{
    delete ui;
}

bool PB_ActionModifyPositions2DOptions::isFreeMoveSelected()
{
    return ui->pb_freeMove->isChecked();
}

bool PB_ActionModifyPositions2DOptions::isMovePositionSelected()
{
    return ui->pb_move->isChecked();
}

bool PB_ActionModifyPositions2DOptions::isAddPositionSelected()
{
    return ui->pb_add->isChecked();
}

bool PB_ActionModifyPositions2DOptions::isRemovePositionSelected()
{
    return ui->pb_remove->isChecked();
}

double PB_ActionModifyPositions2DOptions::getZValue()
{
    return ui->dsb_zval->value();
}

bool PB_ActionModifyPositions2DOptions::isDrawPlaneSelected()
{
    return ui->cb_drawPlane->isChecked();
}

bool PB_ActionModifyPositions2DOptions::isDrawLinesSelected()
{
    return ui->cb_drawLines->isChecked();
}

bool PB_ActionModifyPositions2DOptions::isUpdateLinesSelected()
{
    return ui->cb_updateLines->isChecked();
}

void PB_ActionModifyPositions2DOptions::selectFreeMove()
{
    ui->pb_freeMove->setChecked(true);
}

void PB_ActionModifyPositions2DOptions::selectMovePosition()
{
    ui->pb_move->setChecked(true);
}

void PB_ActionModifyPositions2DOptions::selectAddPosition()
{
    ui->pb_add->setChecked(true);
}

void PB_ActionModifyPositions2DOptions::selectRemovePosition()
{
    ui->pb_remove->setChecked(true);
}

void PB_ActionModifyPositions2DOptions::decreaseZValue()
{
    ui->dsb_zval->stepDown();
}

void PB_ActionModifyPositions2DOptions::increaseZValue()
{
    ui->dsb_zval->stepUp();
}

void PB_ActionModifyPositions2DOptions::on_pb_freeMove_clicked()
{

}

void PB_ActionModifyPositions2DOptions::on_pb_move_clicked()
{

}

void PB_ActionModifyPositions2DOptions::on_pb_add_clicked()
{

}

void PB_ActionModifyPositions2DOptions::on_pb_remove_clicked()
{

}

void PB_ActionModifyPositions2DOptions::on_dsb_zval_valueChanged(double arg1)
{
    emit parametersChanged();
}

void PB_ActionModifyPositions2DOptions::singleStepChanged(int button)
{
    Q_UNUSED(button);

    if (ui->rb_1m->isChecked())
    {
        ui->dsb_zval->setSingleStep(1.0);
    } else if (ui->rb_10cm->isChecked())
    {
        ui->dsb_zval->setSingleStep(0.10);
    } else  {
        ui->dsb_zval->setSingleStep(0.01);
    }
}

void PB_ActionModifyPositions2DOptions::on_cb_drawPlane_toggled(bool checked)
{
    emit parametersChanged();
}

void PB_ActionModifyPositions2DOptions::on_cb_drawLines_toggled(bool checked)
{
    emit parametersChanged();
}
