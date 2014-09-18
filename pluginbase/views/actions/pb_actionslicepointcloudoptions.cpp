#include "pb_actionslicepointcloudoptions.h"
#include "ui_pb_actionslicepointcloudoptions.h"

#include "actions/pb_actionslicepointcloud.h"

#include <QColorDialog>

PB_ActionSlicePointCloudOptions::PB_ActionSlicePointCloudOptions(const PB_ActionSlicePointCloud *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionSlicePointCloudOptions())
{
    ui->setupUi(this);

    ui->pb_exemple->setToolTip("Add 1 (key A, CTRL-left-clic ou CTRL-wheel)");
    ui->cb_drawScenes->setToolTip("Show green circle");

    ui->cb_drawScenes->setChecked(true);
}

PB_ActionSlicePointCloudOptions::~PB_ActionSlicePointCloudOptions()
{
    delete ui;
}

bool PB_ActionSlicePointCloudOptions::shouldDrawScenes()
{
    return ui->cb_drawScenes->isChecked();
}

void PB_ActionSlicePointCloudOptions::on_pb_exemple_clicked()
{
    emit buttonPushed();
    emit parametersChanged();
}

void PB_ActionSlicePointCloudOptions::on_cb_drawScenes_clicked()
{
    emit parametersChanged();
}
