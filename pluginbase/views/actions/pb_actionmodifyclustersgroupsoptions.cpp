#include "pb_actionmodifyclustersgroupsoptions.h"
#include "ui_pb_actionmodifyclustersgroupsoptions.h"
#include "ct_global/ct_context.h"

PB_ActionModifyClustersGroupsOptions::PB_ActionModifyClustersGroupsOptions(const PB_ActionModifyClustersGroups *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionModifyClustersGroupsOptions)
{
    ui->setupUi(this);

//    ui->buttonGroupSelection->setId(ui->toolButtonSelectOne, GraphicsViewInterface::SELECT_ONE);
//    ui->buttonGroupSelection->setId(ui->toolButtonAddOne, GraphicsViewInterface::ADD_ONE);
//    ui->buttonGroupSelection->setId(ui->toolButtonRemoveOne, GraphicsViewInterface::REMOVE_ONE);
//    ui->buttonGroupSelection->setId(ui->toolButtonSelectMulti, GraphicsViewInterface::SELECT);
//    ui->buttonGroupSelection->setId(ui->toolButtonAddMulti, GraphicsViewInterface::ADD);
//    ui->buttonGroupSelection->setId(ui->toolButtonRemoveMulti, GraphicsViewInterface::REMOVE);

//    ui->buttonGroupOptimization->setId(ui->radioButtonSelectAsShown, PB_ActionModifyClustersGroups::SELECT_CURRENT_MODE);
//    ui->buttonGroupOptimization->setId(ui->radioButtonNORMAL, PB_ActionModifyClustersGroups::SELECT_NORMAL_MODE);
//    ui->buttonGroupOptimization->setId(ui->radioButtonFAST, PB_ActionModifyClustersGroups::SELECT_FAST_MODE);
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
    else if(ui->radioButtonFaces->isChecked()) {
        mode += GraphicsViewInterface::SELECT_FACES;
    }
    else if(ui->radioButtonEdges->isChecked()) {
        mode += GraphicsViewInterface::SELECT_EDGES;
    }

    return (GraphicsViewInterface::SelectionMode)mode;
}

//PB_ActionModifyClustersGroups::SelectionDrawMode PB_ActionModifyClustersGroupsOptions::drawMode() const
//{
//    return (PB_ActionModifyClustersGroups::SelectionDrawMode)ui->buttonGroupOptimization->checkedId();
//}

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

//void PB_ActionModifyClustersGroupsOptions::on_buttonGroupOptimization_buttonReleased(int id)
//{
//    Q_UNUSED(id)

//    (dynamic_cast<PB_ActionModifyClustersGroups*>(action()))->setDrawMode(drawMode());
//}

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
