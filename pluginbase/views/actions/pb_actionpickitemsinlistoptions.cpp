#include "pb_actionpickitemsinlistoptions.h"
#include "ui_pb_actionpickitemsinlistoptions.h"

#include "actions/pb_actionpickitemsinlist.h"

#include <QColorDialog>

PB_ActionPickItemsInListOptions::PB_ActionPickItemsInListOptions(const PB_ActionPickItemsInList *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionPickItemsInListOptions())
{
    ui->setupUi(this);
    _itemColor = QColor(0, 250, 0);
}

PB_ActionPickItemsInListOptions::~PB_ActionPickItemsInListOptions()
{
    delete ui;
}

QColor PB_ActionPickItemsInListOptions::getColor()
{
    return _itemColor;
}

bool PB_ActionPickItemsInListOptions::isInAddMode()
{
    return ui->pb_add->isChecked();
}

float PB_ActionPickItemsInListOptions::getMaxDist()
{
    return (float) ui->dsb_maxDist->value();
}

void PB_ActionPickItemsInListOptions::on_pb_color_clicked()
{
    _itemColor = QColorDialog::getColor(_itemColor);
    if (!_itemColor.isValid()) {_itemColor = QColor(0, 250, 0);}
    ui->pb_color->setStyleSheet("QPushButton { background-color: " + _itemColor.name() + "; }");

    emit parametersChanged();
}


void PB_ActionPickItemsInListOptions::on_dsb_maxDist_valueChanged(double arg1)
{
    emit parametersChanged();
}
