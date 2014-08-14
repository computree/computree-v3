#include "gminmaxattributesscalarconfiguration.h"
#include "ui_gminmaxattributesscalarconfiguration.h"

GMinMaxAttributesScalarConfiguration::GMinMaxAttributesScalarConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GMinMaxAttributesScalarConfiguration)
{
    ui->setupUi(this);

    m_pa = NULL;
}

GMinMaxAttributesScalarConfiguration::~GMinMaxAttributesScalarConfiguration()
{
    delete ui;
}

void GMinMaxAttributesScalarConfiguration::setAttributes(const DM_AbstractAttributesScalar *pa)
{
    m_pa = (DM_AbstractAttributesScalar*)pa;

    ui->doubleSpinBoxMin->setValue(m_pa->min());
    ui->doubleSpinBoxMax->setValue(m_pa->max());
    ui->checkBoxAutoAdjust->setChecked(m_pa->isAutoAdjust());
}

void GMinMaxAttributesScalarConfiguration::accept()
{
    double min = ui->doubleSpinBoxMin->value();
    double max = ui->doubleSpinBoxMax->value();

    if(max < min)
    {
        min = max;
        max = ui->doubleSpinBoxMin->value();
    }

    m_pa->setAutoAdjust(ui->checkBoxAutoAdjust->isChecked());

    if(!m_pa->isAutoAdjust())
    {
        m_pa->setMin(min);
        m_pa->setMax(max);
    }

    QDialog::accept();
}

void GMinMaxAttributesScalarConfiguration::on_doubleSpinBoxMin_valueChanged(double d)
{
    if(m_pa->min() != d)
    {
        ui->checkBoxAutoAdjust->setChecked(false);
    }
}

void GMinMaxAttributesScalarConfiguration::on_doubleSpinBoxMax_valueChanged(double d)
{
    if(m_pa->max() != d)
    {
        ui->checkBoxAutoAdjust->setChecked(false);
    }
}
