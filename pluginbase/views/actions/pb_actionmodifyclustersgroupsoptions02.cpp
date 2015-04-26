#include "pb_actionmodifyclustersgroupsoptions02.h"
#include "ui_pb_actionmodifyclustersgroupsoptions02.h"
#include "ct_global/ct_context.h"

#include <QColorDialog>

PB_ActionModifyClustersGroupsOptions02::PB_ActionModifyClustersGroupsOptions02(const PB_ActionModifyClustersGroups02 *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionModifyClustersGroupsOptions02)
{
    ui->setupUi(this);

    connect(ui->buttonGroupType, SIGNAL(buttonClicked(int)), this, SLOT(modeChanged(int)));
    connect(ui->buttonGroupBy, SIGNAL(buttonClicked(int)), this, SLOT(byChanged(int)));
}

PB_ActionModifyClustersGroupsOptions02::~PB_ActionModifyClustersGroupsOptions02()
{
    delete ui;
}

GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroupsOptions02::selectionMode() const
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

    if(ui->pb_SplitMode->isChecked()) {
        mode--;
        mode += GraphicsViewInterface::SELECT_POINTS;
    }

    return (GraphicsViewInterface::SelectionMode)mode;
}

bool PB_ActionModifyClustersGroupsOptions02::isAVisible() const
{
    return ui->cb_Avisible->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isBVisible() const
{
    return ui->cb_Bvisible->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isTMPVisible() const
{
    return ui->cb_tmpVisible->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isTrashVisible() const
{
    return ui->cb_trashVisible->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isOthersVisible() const
{
    return ui->cb_othersVisible->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isInSceneMode() const
{
    return ui->pb_SceneMode->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isInLimitMode() const
{
    return ui->pb_LimitMode->isChecked();
}

bool PB_ActionModifyClustersGroupsOptions02::isInSplitMode() const
{
    return ui->pb_SplitMode->isChecked();
}

void PB_ActionModifyClustersGroupsOptions02::on_buttonGroupType_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups02*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions02::on_buttonGroupMode_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups02*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions02::on_buttonGroupSelection_buttonReleased(int id)
{
    Q_UNUSED(id)

    (dynamic_cast<PB_ActionModifyClustersGroups02*>(action()))->setSelectionMode(selectionMode());
}

void PB_ActionModifyClustersGroupsOptions02::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
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

void PB_ActionModifyClustersGroupsOptions02::on_pb_SetSceneA_clicked()
{
    emit selectPositionA();
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_SetSceneB_clicked()
{
    emit selectPositionB();
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_ColorA_clicked()
{
    QColor tmp = QColorDialog::getColor(_colorA);
    if (!tmp.isValid()) {return;}
    selectColorA(tmp);
    emit setColorA(tmp);
}

void PB_ActionModifyClustersGroupsOptions02::selectColorA(QColor color)
{
    _colorA = color;
    ui->pb_ColorA->setStyleSheet("QPushButton { background-color: " + _colorA.name() + "; }");
    ui->pb_toA->setStyleSheet("QToolButton { background-color: " + _colorA.name() + "; }");
    ui->pb_frontierA->setStyleSheet("QPushButton { background-color: " + _colorA.name() + "; }");
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_ColorB_clicked()
{
    QColor tmp = QColorDialog::getColor(_colorB);
    if (!tmp.isValid()) {return;}
    selectColorB(tmp);
    emit setColorB(tmp);
}

void PB_ActionModifyClustersGroupsOptions02::selectColorB(QColor color)
{
    _colorB = color;
    ui->pb_ColorB->setStyleSheet("QPushButton { background-color: " + _colorB.name() + "; }");
    ui->pb_toB->setStyleSheet("QToolButton { background-color: " + _colorB.name() + "; }");
    ui->pb_frontierB->setStyleSheet("QPushButton { background-color: " + _colorB.name() + "; }");
}

void PB_ActionModifyClustersGroupsOptions02::selectSceneMode()
{
    ui->pb_SceneMode->setChecked(true);
    modeChanged(0);
}

void PB_ActionModifyClustersGroupsOptions02::selectLimitMode()
{
    ui->pb_LimitMode->setChecked(true);
    modeChanged(0);
}

void PB_ActionModifyClustersGroupsOptions02::selectSplitMode()
{
    ui->pb_SplitMode->setChecked(true);
    modeChanged(0);
}

void PB_ActionModifyClustersGroupsOptions02::setMultiSelect(bool multi)
{
    if (multi)
    {
        ui->toolButtonSelectMulti->setChecked(true);
    } else {
        ui->toolButtonSelectOne->setChecked(true);
    }
}

void PB_ActionModifyClustersGroupsOptions02::setDistance(int val)
{
    ui->sb_LengthOnAxis->setValue(val);
}

void PB_ActionModifyClustersGroupsOptions02::setMaxDistance(int val)
{
    ui->sb_LengthOnAxis->setMaximum(val);
    ui->sb_LengthOnAxis->setSuffix(QString("/ %1").arg(val));
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_toA_clicked()
{
    emit affectClusterToA();
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_toB_clicked()
{
    emit affectClusterToB();
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_toTmp_clicked()
{
    emit affectClusterToTMP();
}

void PB_ActionModifyClustersGroupsOptions02::on_pb_toTrash_clicked()
{
    emit affectClusterToTrash();
}

void PB_ActionModifyClustersGroupsOptions02::on_cb_Avisible_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionModifyClustersGroupsOptions02::on_cb_Bvisible_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionModifyClustersGroupsOptions02::on_cb_othersVisible_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionModifyClustersGroupsOptions02::on_cb_tmpVisible_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionModifyClustersGroupsOptions02::on_cb_trashVisible_toggled(bool checked)
{
    Q_UNUSED(checked);
    emit visibilityChanged();
}

void PB_ActionModifyClustersGroupsOptions02::modeChanged(int button)
{
    Q_UNUSED(button);

    if (ui->pb_SceneMode->isChecked())
    {
        ui->wid_sceneMode->setVisible(true);
        ui->wid_limitMode->setVisible(false);
    } else if (ui->pb_LimitMode->isChecked())
    {
        ui->wid_sceneMode->setVisible(false);
        ui->wid_limitMode->setVisible(true);

        setSelectionMode(GraphicsViewInterface::SELECT_ONE);

        emit enterLimitMode();

    } else if (ui->pb_SplitMode->isChecked())
    {
        ui->wid_sceneMode->setVisible(false);
        ui->wid_limitMode->setVisible(false);
    }
}

void PB_ActionModifyClustersGroupsOptions02::byChanged(int button)
{
    Q_UNUSED(button);
    if (ui->rb_001->isChecked())
    {
        ui->sb_LengthOnAxis->setSingleStep(1);
    } else if (ui->rb_010->isChecked())
    {
        ui->sb_LengthOnAxis->setSingleStep(10);
    } else  {
        ui->sb_LengthOnAxis->setSingleStep(100);
    }
}

void PB_ActionModifyClustersGroupsOptions02::on_sb_LengthOnAxis_valueChanged(int arg1)
{
    emit distanceChanged(arg1);
}
