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
    item2->setText(QString("result%1").arg(_idCount++));

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
    item2->setText(QString("group%1").arg(_idCount++));

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
    item2->setText(QString("item%1").arg(_idCount++));

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
    QModelIndex index = ui->treeView->currentIndex();
    AbtractModel *item = (AbtractModel*) _model->itemFromIndex(index);
    ui->treeView->clearSelection();
    if (item != NULL)
    {
        //ui->treeView->setModel(NULL);


        //delete all children of parent;
        QStandardItem * loopItem = item; //main loop item
        QList<QStandardItem *> carryItems; //Last In First Out stack of items
        QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
        while (loopItem->rowCount())
        {
            itemsToBeDeleted << loopItem->takeRow(0);
            //if the row removed has children:
            if (itemsToBeDeleted.at(0)->hasChildren())
            {
                carryItems << loopItem; //put on the stack the current loopItem
                loopItem = itemsToBeDeleted.at(0); //set the row with children as the loopItem
            }
            //if current loopItem has no more rows but carryItems list is not empty:
            if (!loopItem->rowCount() && !carryItems.isEmpty()) loopItem = carryItems.takeFirst();

        }
        item->parent()->takeRow(item->row());
        itemsToBeDeleted.append(item);

        while (!itemsToBeDeleted.isEmpty())        {
            AbtractModel* itemToDelete = (AbtractModel*) itemsToBeDeleted.takeLast();
            if (itemToDelete->getWidget() == _activeWidget) {_activeWidget = NULL;}
            delete itemToDelete;
        }

        //ui->treeView->setModel(_model);
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

void INModelDialog::recursiveDeleteItem(QStandardItem* item, QStandardItemModel* model)
{
    int count = item->rowCount();
    for (int i = count - 1 ; i >= 0 ; i--)
    {
        QStandardItem* child = item->child(i);
        recursiveDeleteItem(child, model);
        model->removeRow(child->index().row(), child->index().parent());
        delete child;
    }
}

