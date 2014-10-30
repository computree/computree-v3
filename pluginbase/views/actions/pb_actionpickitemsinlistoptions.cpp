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
    ui->pb_color->setStyleSheet("QPushButton { background-color: " + _itemColor.name() + "; }");
}

PB_ActionPickItemsInListOptions::~PB_ActionPickItemsInListOptions()
{
    delete ui;
}

QColor PB_ActionPickItemsInListOptions::getColor()
{
    return _itemColor;
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

void PB_ActionPickItemsInListOptions::on_pb_all_clicked()
{
    emit allClicked();
}

void PB_ActionPickItemsInListOptions::on_pb_none_clicked()
{
    emit noneClicked();
}
