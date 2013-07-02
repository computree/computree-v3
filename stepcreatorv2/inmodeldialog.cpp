#include "inmodeldialog.h"
#include "ui_inmodeldialog.h"

#include "qdebug.h"

#include "models/inresultmodel.h"
#include "models/ingroupmodel.h"
#include "models/initemmodel.h"
#include "models/abtractmodel.h"

INModelDialog::INModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::INModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    _layout = new QVBoxLayout();
    ui->widgetZone->setLayout(_layout);
    _activeWidget = NULL;

    _idCount = 0;
}

INModelDialog::~INModelDialog()
{
    delete ui;
}

void INModelDialog::on_pb_addResult_clicked()
{
    QStandardItem *item = _model->invisibleRootItem();

    INResultModel *item2 = new INResultModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
}


void INModelDialog::on_pb_addGroup_clicked()
{
    AbtractModel *item = (AbtractModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }

    if (item->getModelType()!=AbtractModel::M_Result_IN && item->getModelType()!=AbtractModel::M_Group_IN)
    {
        return;
    }

    INGroupModel *item2 = new INGroupModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);

}


void INModelDialog::on_pb_addItem_clicked()
{
    AbtractModel *item = (AbtractModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }
    if (item->getModelType()!=AbtractModel::M_Group_IN)
    {
        return;
    }

    INItemModel *item2 = new INItemModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
}

void INModelDialog::on_pb_delete_clicked()
{
    QStandardItem *item = _model->itemFromIndex(ui->treeView->currentIndex());
    if (item != NULL)
    {
        qDebug() << "Item supprimé = " << item->text();
        qDebug() << "Item fils = " << item->child(0)->text();
        qDebug() << "Nb Items fils = " << item->rowCount();
    }
}

void INModelDialog::on_treeView_clicked(const QModelIndex &index)
{
    AbtractModel *item = (AbtractModel*) _model->itemFromIndex(index);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item->getWidget();
    _layout->addWidget(_activeWidget);

}
