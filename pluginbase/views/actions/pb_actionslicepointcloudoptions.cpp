#include "pb_actionslicepointcloudoptions.h"
#include "ui_pb_actionslicepointcloudoptions.h"

#include "actions/pb_actionslicepointcloud.h"

#include <QColorDialog>

PB_ActionSlicePointCloudOptions::PB_ActionSlicePointCloudOptions(const PB_ActionSlicePointCloud *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionSlicePointCloudOptions())
{
    ui->setupUi(this);

    connect(ui->dsb_thickness, SIGNAL(valueChanged(double)), this, SIGNAL(parametersChanged()));
    connect(ui->dsb_space, SIGNAL(valueChanged(double)), this, SIGNAL(parametersChanged()));
}

PB_ActionSlicePointCloudOptions::~PB_ActionSlicePointCloudOptions()
{
    delete ui;
}

double PB_ActionSlicePointCloudOptions::getThickness() const
{
    return ui->dsb_thickness->value();
}

double PB_ActionSlicePointCloudOptions::getSpacing() const
{
    return ui->dsb_space->value();
}

double PB_ActionSlicePointCloudOptions::getIncrement() const
{
    return 0.01;
}

void PB_ActionSlicePointCloudOptions::setThickness(double t) const
{
    ui->dsb_thickness->setValue(t);
}

void PB_ActionSlicePointCloudOptions::setSpacing(double s) const
{
    ui->dsb_space->setValue(s);
}

void PB_ActionSlicePointCloudOptions::setIncrement(double i) const
{

}

