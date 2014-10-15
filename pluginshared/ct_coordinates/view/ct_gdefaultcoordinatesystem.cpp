#include "ct_gdefaultcoordinatesystem.h"
#include "ui_ct_gdefaultcoordinatesystem.h"

CT_GDefaultCoordinateSystem::CT_GDefaultCoordinateSystem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CT_GDefaultCoordinateSystem)
{
    ui->setupUi(this);
    m_cs = NULL;
}

CT_GDefaultCoordinateSystem::~CT_GDefaultCoordinateSystem()
{
    delete ui;
}

void CT_GDefaultCoordinateSystem::setCoordinateSystem(const CT_DefaultCoordinateSystem *cs)
{
    m_cs = (CT_DefaultCoordinateSystem*)cs;
}

void CT_GDefaultCoordinateSystem::setReadOnly(bool val)
{
    ui->dsbXOffset->setReadOnly(val);
    ui->dsbYOffset->setReadOnly(val);
    ui->dsbZOffset->setReadOnly(val);
}

void CT_GDefaultCoordinateSystem::accept()
{
    if(!ui->dsbXOffset->isReadOnly())
    {
        m_cs->setOffset(ui->dsbXOffset->value(),
                        ui->dsbYOffset->value(),
                        ui->dsbZOffset->value());
    }

    QDialog::accept();
}
