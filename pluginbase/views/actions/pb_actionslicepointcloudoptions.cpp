#include "pb_actionslicepointcloudoptions.h"
#include "ui_pb_actionslicepointcloudoptions.h"

#include "actions/pb_actionslicepointcloud.h"

#include <QColorDialog>

PB_ActionSlicePointCloudOptions::PB_ActionSlicePointCloudOptions(const PB_ActionSlicePointCloud *action) :
    CT_GAbstractActionForGraphicsViewOptions(action),
    ui(new Ui::PB_ActionSlicePointCloudOptions())
{
    ui->setupUi(this);

    connect(ui->dsb_thickness, SIGNAL(valueChanged(double)), this, SIGNAL(parametersChanged()));
    connect(ui->dsb_space, SIGNAL(valueChanged(double)), this, SIGNAL(parametersChanged()));

    ui->dsb_thickness->setToolTip(tr("CTRL Molette"));
    ui->lb_thickness->setToolTip(tr("CTRL Molette"));
    ui->dsb_space->setToolTip(tr("SHIFT Molette"));
    ui->lb_spacing->setToolTip(tr("SHIFT Molette"));


    connect(ui->bg_singleStep, SIGNAL(buttonClicked(int)), this, SLOT(changeSingleStep(int)));
    ui->rb_10cm->setChecked(true);

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
    if (ui->rb_1mm->isChecked()) {return 0.001;}
    if (ui->rb_1cm->isChecked()) {return 0.01;}
    if (ui->rb_10cm->isChecked()) {return 0.1;}
    if (ui->rb_1m->isChecked()) {return 1;}

    return 0.1;
}

void PB_ActionSlicePointCloudOptions::setThickness(double t) const
{
    ui->dsb_thickness->setValue(t);
}

void PB_ActionSlicePointCloudOptions::setSpacing(double s) const
{
    ui->dsb_space->setValue(s);
}

void PB_ActionSlicePointCloudOptions::increaseIncrement()
{
    if (ui->rb_1mm->isChecked()) {ui->rb_1cm->setChecked(true);}
    else if (ui->rb_1cm->isChecked()) {ui->rb_10cm->setChecked(true);}
    else if (ui->rb_10cm->isChecked()) {ui->rb_1m->setChecked(true);}
}

void PB_ActionSlicePointCloudOptions::decreaseIncrement()
{
    if (ui->rb_1m->isChecked()) {ui->rb_10cm->setChecked(true);}
    else if (ui->rb_10cm->isChecked()) {ui->rb_1cm->setChecked(true);}
    else if (ui->rb_1cm->isChecked()) {ui->rb_1mm->setChecked(true);}

}

void PB_ActionSlicePointCloudOptions::changeSingleStep(int button)
{
    Q_UNUSED(button);

    if (ui->rb_1mm->isChecked()) {ui->dsb_thickness->setSingleStep(0.001);ui->dsb_space->setSingleStep(0.001);}
    else if (ui->rb_1cm->isChecked()) {ui->dsb_thickness->setSingleStep(0.01);ui->dsb_space->setSingleStep(0.01);}
    else if (ui->rb_10cm->isChecked()) {ui->dsb_thickness->setSingleStep(0.1);ui->dsb_space->setSingleStep(0.1);}
    else if (ui->rb_1m->isChecked()) {ui->dsb_thickness->setSingleStep(1);ui->dsb_space->setSingleStep(1);}

}

