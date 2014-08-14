#include "pb_actionselectitemdrawablegvoptions.h"
#include "ui_pb_actionselectitemdrawablegvoptions.h"

PB_ActionSelectItemDrawableGVOptions::PB_ActionSelectItemDrawableGVOptions(const PB_ActionSelectItemDrawableGV *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionSelectItemDrawableGVOptions)
{
    ui->setupUi(this);

    ui->buttonGroupSelection->setId(ui->toolButtonSelectOne, GraphicsViewInterface::SELECT_ONE);
    ui->buttonGroupSelection->setId(ui->toolButtonAddOne, GraphicsViewInterface::ADD_ONE);
    ui->buttonGroupSelection->setId(ui->toolButtonRemoveOne, GraphicsViewInterface::REMOVE_ONE);
    ui->buttonGroupSelection->setId(ui->toolButtonSelectMulti, GraphicsViewInterface::SELECT);
    ui->buttonGroupSelection->setId(ui->toolButtonAddMulti, GraphicsViewInterface::ADD);
    ui->buttonGroupSelection->setId(ui->toolButtonRemoveMulti, GraphicsViewInterface::REMOVE);

//    ui->buttonGroupOptimization->setId(ui->radioButtonSelectAsShown, PB_ActionSelectItemDrawableGV::SELECT_CURRENT_MODE);
//    ui->buttonGroupOptimization->setId(ui->radioButtonNORMAL, PB_ActionSelectItemDrawableGV::SELECT_NORMAL_MODE);
//    ui->buttonGroupOptimization->setId(ui->radioButtonFAST, PB_ActionSelectItemDrawableGV::SELECT_FAST_MODE);
}

PB_ActionSelectItemDrawableGVOptions::~PB_ActionSelectItemDrawableGVOptions()
{
    delete ui;
}

GraphicsViewInterface::SelectionMode PB_ActionSelectItemDrawableGVOptions::selectionMode() const
{
    int mode = ui->buttonGroupSelection->checkedId()-1;

    if(ui->radioButtonPoints->isChecked())
        mode += GraphicsViewInterface::SELECT_POINTS;
    else if(ui->radioButtonFaces->isChecked())
        mode += GraphicsViewInterface::SELECT_FACES;
    else if(ui->radioButtonEdges->isChecked())
        mode += GraphicsViewInterface::SELECT_EDGES;
    else
        ++mode;

    return (GraphicsViewInterface::SelectionMode)mode;
}

//PB_ActionSelectItemDrawableGV::SelectionDrawMode PB_ActionSelectItemDrawableGVOptions::drawMode() const
//{
//    return (PB_ActionSelectItemDrawableGV::SelectionDrawMode)ui->buttonGroupOptimization->checkedId();
//}

void PB_ActionSelectItemDrawableGVOptions::on_buttonGroupType_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionSelectItemDrawableGV*>(action()))->setSelectionMode(selectionMode());
}

//void PB_ActionSelectItemDrawableGVOptions::on_buttonGroupOptimization_buttonReleased(int id)
//{
//    Q_UNUSED(id)

//    (dynamic_cast<PB_ActionSelectItemDrawableGV*>(action()))->setDrawMode(drawMode());
//}

void PB_ActionSelectItemDrawableGVOptions::on_buttonGroupSelection_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionSelectItemDrawableGV*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionSelectItemDrawableGVOptions::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    if(mode != GraphicsViewInterface::NONE)
    {
        int m = mode;

        while(m > GraphicsViewInterface::REMOVE_ONE)
            m -= GraphicsViewInterface::REMOVE_ONE;

        ui->buttonGroupSelection->button(m)->setChecked(true);
    }
}
