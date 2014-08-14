#include "pb_actionselectcellsingrid3dcolonizedialog.h"
#include "ui_pb_actionselectcellsingrid3dcolonizedialog.h"

PB_ActionSelectCellsInGrid3DColonizeDialog::PB_ActionSelectCellsInGrid3DColonizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PB_ActionSelectCellsInGrid3DColonizeDialog)
{
    ui->setupUi(this);
}

PB_ActionSelectCellsInGrid3DColonizeDialog::~PB_ActionSelectCellsInGrid3DColonizeDialog()
{
    delete ui;
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setXp(bool val)
{
    ui->cb_xp->setChecked(val);
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setXm(bool val)
{
    ui->cb_xm->setChecked(val);
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setYp(bool val)
{
    ui->cb_yp->setChecked(val);
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setYm(bool val)
{
    ui->cb_ym->setChecked(val);
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setZp(bool val)
{
    ui->cb_zp->setChecked(val);
}

void PB_ActionSelectCellsInGrid3DColonizeDialog::setZm(bool val)
{
    ui->cb_zm->setChecked(val);
}

bool PB_ActionSelectCellsInGrid3DColonizeDialog::getXp()
{
    return ui->cb_xp->isChecked();
}

bool PB_ActionSelectCellsInGrid3DColonizeDialog::getXm()
{
    return ui->cb_xm->isChecked();
}

bool PB_ActionSelectCellsInGrid3DColonizeDialog::getYp()
{
    return ui->cb_yp->isChecked();
}

bool PB_ActionSelectCellsInGrid3DColonizeDialog::getYm()
{
    return ui->cb_ym->isChecked();
}
bool PB_ActionSelectCellsInGrid3DColonizeDialog::getZp()
{
    return ui->cb_zp->isChecked();
}

bool PB_ActionSelectCellsInGrid3DColonizeDialog::getZm()
{
    return ui->cb_zm->isChecked();
}
