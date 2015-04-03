#include "pb_actionmodifyclustersgroupsoptions.h"
#include "ui_pb_actionmodifyclustersgroupsoptions.h"
#include "ct_global/ct_context.h"

#include <QColorDialog>

PB_ActionModifyClustersGroupsOptions::PB_ActionModifyClustersGroupsOptions(const PB_ActionModifyClustersGroups *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionModifyClustersGroupsOptions)
{
    ui->setupUi(this);

    _colorA = QColor(0  ,255,0  ); // Vert
    _colorB = QColor(0  ,0  ,255); // Bleu

    ui->pb_ColorA->setStyleSheet("QPushButton { background-color: " + _colorA.name() + "; }");
    ui->pb_ColorB->setStyleSheet("QPushButton { background-color: " + _colorB.name() + "; }");
}

PB_ActionModifyClustersGroupsOptions::~PB_ActionModifyClustersGroupsOptions()
{
    delete ui;
}

GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroupsOptions::selectionMode() const
{   
    int mode = GraphicsViewInterface::NONE;

    if (ui->toolButtonSelectOne->isChecked())
    {
        if (ui->toolButtonReplaceMode->isChecked()) {
            mode = GraphicsViewInterface::SELECT_ONE;
        } else if (ui->toolButtonAddMode->isChecked()) {
            mode = GraphicsViewInterface::ADD_ONE;
        } else {
            mode = GraphicsViewInterface::REMOVE_ONE;
        }
    } else if (ui->toolButtonSelectMulti->isChecked()) {
        if (ui->toolButtonReplaceMode->isChecked()) {
            mode = GraphicsViewInterface::SELECT;
        } else if (ui->toolButtonAddMode->isChecked()) {
            mode = GraphicsViewInterface::ADD;
        } else {
            mode = GraphicsViewInterface::REMOVE;
        }
    } else {
        return (GraphicsViewInterface::SelectionMode)mode;
    }

    if(!ui->radioButtonItems->isChecked()) {
        mode--;
    }

    if(ui->radioButtonPoints->isChecked()) {
        mode += GraphicsViewInterface::SELECT_POINTS;
    }

    return (GraphicsViewInterface::SelectionMode)mode;
}

bool PB_ActionModifyClustersGroupsOptions::isOnlyABChecked() const
{
    return ui->cb_ShowOnlyAAndB->isChecked();
}

void PB_ActionModifyClustersGroupsOptions::on_buttonGroupType_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions::on_buttonGroupMode_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions::on_buttonGroupSelection_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    if(mode != GraphicsViewInterface::NONE)
    {
        int m = mode;

        while(m > GraphicsViewInterface::REMOVE_ONE)
            m -= GraphicsViewInterface::REMOVE_ONE;       

        if (mode == GraphicsViewInterface::SELECT) {
            ui->toolButtonSelectMulti->setChecked(true);
            ui->toolButtonReplaceMode->setChecked(true);
        } else if (mode == GraphicsViewInterface::ADD) {
            ui->toolButtonSelectMulti->setChecked(true);
            ui->toolButtonAddMode->setChecked(true);
        } else if (mode == GraphicsViewInterface::REMOVE) {
            ui->toolButtonSelectMulti->setChecked(true);
            ui->toolButtonRemoveMode->setChecked(true);
        } else if (mode == GraphicsViewInterface::SELECT_ONE) {
            ui->toolButtonSelectOne->setChecked(true);
            ui->toolButtonReplaceMode->setChecked(true);
        } else if (mode == GraphicsViewInterface::ADD_ONE) {
            ui->toolButtonSelectOne->setChecked(true);
            ui->toolButtonAddMode->setChecked(true);
        } else if (mode == GraphicsViewInterface::REMOVE_ONE) {
            ui->toolButtonSelectOne->setChecked(true);
            ui->toolButtonRemoveMode->setChecked(true);
        }
    }
}

void PB_ActionModifyClustersGroupsOptions::on_pb_SetSceneA_clicked()
{
    emit selectPositionA();
}

void PB_ActionModifyClustersGroupsOptions::on_pb_SetSceneB_clicked()
{
    emit selectPositionB();
}

void PB_ActionModifyClustersGroupsOptions::on_pb_ColorA_clicked()
{
    _colorA = QColorDialog::getColor(_colorA);
    if (!_colorA.isValid()) {return;}

    ui->pb_ColorA->setStyleSheet("QPushButton { background-color: " + _colorA.name() + "; }");
    emit setColorA(_colorA);
}

void PB_ActionModifyClustersGroupsOptions::on_pb_ColorB_clicked()
{
    _colorB = QColorDialog::getColor(_colorB);
    if (!_colorB.isValid()) {return;}

    ui->pb_ColorB->setStyleSheet("QPushButton { background-color: " + _colorB.name() + "; }");
    emit setColorB(_colorB);
}

void PB_ActionModifyClustersGroupsOptions::on_cb_ShowOnlyAAndB_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit onlyABChanged();
}
