#include "copymodeldialog.h"
#include "ui_copymodeldialog.h"

#include "qdebug.h"

#include "qmessagebox.h"

#include "models/copyresultmodel.h"
#include "models/copygroupmodel.h"
#include "models/copyitemmodel.h"
#include "tools.h"

COPYModelDialog::COPYModelDialog(QStandardItemModel *inModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::COPYModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    _layout = new QVBoxLayout();
    ui->widgetZone->setLayout(_layout);
    _activeWidget = NULL;
    _inModel = inModel;
}

COPYModelDialog::~COPYModelDialog()
{
    delete ui;
}

void COPYModelDialog::init()
{
    _activeWidget = NULL;
    _model->clear();
    QStandardItem* inRoot = _inModel->invisibleRootItem();
    QStandardItem* copyRoot = _model->invisibleRootItem();
    int count = inRoot->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        INResultModel* inResult = (INResultModel*) inRoot->child(i);

        if (inResult->isCopyResult())
        {
            COPYResultModel* copyResult = new COPYResultModel();
            copyResult->init(inResult);
            copyRoot->appendRow(copyResult);
            recursiveAddChildren(copyResult, inResult);
        }
    }
    ui->treeView->expandAll();

}

// static
void COPYModelDialog::recursiveAddChildren(AbstractCopyModel* copyModel, AbstractInModel* inModel)
{
    int count = inModel->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractInModel* inItem = (AbstractInModel*) inModel->child(i);

        if (inItem->getModelType() == AbstractInModel::M_Group_IN)
        {
            COPYGroupModel* copyItem = new COPYGroupModel();
            copyItem->init((INGroupModel*) inItem);
            copyModel->appendRow(copyItem);
            recursiveAddChildren(copyItem, inItem);

        } else if (inItem->getModelType() == AbstractInModel::M_Item_IN)
        {
            COPYItemModel* copyItem = new COPYItemModel();
            copyItem->init((INItemModel*) inItem);
            copyModel->appendRow(copyItem);
            recursiveAddChildren(copyItem, inItem);
        }
    }
}

AbstractCopyModel::CopyIncludesNeeds COPYModelDialog::copyIncludesNeeded()
{
    AbstractCopyModel::CopyIncludesNeeds result = AbstractCopyModel::C_None;

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);

        if (item->copyIncludesNeeded() == AbstractCopyModel::C_Add) {result =  AbstractCopyModel::C_Add;}
        if ((result!=AbstractCopyModel::C_Add) && (item->copyIncludesNeeded() == AbstractCopyModel::C_Delete)) {result = AbstractCopyModel::C_Delete;}
    }
    return result;
}



void COPYModelDialog::on_pb_addGroup_clicked()
{
    AbstractCopyModel *item = (AbstractCopyModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }

    if (item->getModelType()!=AbstractCopyModel::M_Result_COPY && item->getModelType()!=AbstractCopyModel::M_Group_COPY)
    {
        return;
    }

    if (item->getModelType()==AbstractCopyModel::M_Result_COPY && item->rowCount()!=0)
    {
        return;
    }


    COPYGroupModel *item2 = new COPYGroupModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
    ui->treeView->setCurrentIndex(item2->index());
    _activeWidget->setFocus();

}


void COPYModelDialog::on_pb_addItem_clicked()
{
    AbstractCopyModel *item = (AbstractCopyModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }
    if (item->getModelType()!=AbstractCopyModel::M_Group_COPY)
    {
        return;
    }

    COPYItemModel *item2 = new COPYItemModel();

    item->appendRow(item2);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item2->getWidget();
    _layout->addWidget(_activeWidget);

    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
    _activeWidget->setFocus();
}

void COPYModelDialog::on_pb_delete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractCopyModel *item = (AbstractCopyModel*) _model->itemFromIndex(index);
    ui->treeView->clearSelection();
    if (item != NULL && item->getModelType()!=AbstractCopyModel::M_Result_COPY)
    {
        deleteItem(item, _model, _activeWidget);

    }
}

void COPYModelDialog::deleteItem(AbstractCopyModel * item, QStandardItemModel* model, AbstractCopyWidget* &activeWidget)
{
    if (item->getStatus()==AbstractCopyModel::S_Added)
    {
        //delete all children of parent;
        QStandardItem * loopItem = item; //main loop item
        QList<QStandardItem *> carryItems; //Last In First Copy stack of items
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
        if (item->parent()!=NULL)
        {
            item->parent()->takeRow(item->row());
        } else
        {
            QStandardItem *root = model->invisibleRootItem();
            root->takeRow(item->row());
        }

        itemsToBeDeleted.append(item);

        while (!itemsToBeDeleted.isEmpty())        {
            AbstractCopyModel* itemToDelete = (AbstractCopyModel*) itemsToBeDeleted.takeLast();
            if (itemToDelete->getWidget() == activeWidget) {activeWidget = NULL;}
            delete itemToDelete;
        }
    } else {
        item->setDeleted();
        int count = item->rowCount();
        for (int i = 0 ; i < count ; i++)
        {
            AbstractCopyModel* item2 = (AbstractCopyModel*) item->child(i);
            deleteItem(item2, model, activeWidget);
        }
    }
}

void COPYModelDialog::cancelDelete(AbstractCopyModel * item)
{
    if (item != NULL && item->getStatus()==AbstractCopyModel::S_DeletedCopy)
    {
        item->setNotDeleted();
        if (item->parent()!=NULL)
        {
            cancelDelete((AbstractCopyModel*)item->parent());
        }
    }
}


void COPYModelDialog::on_treeView_clicked(const QModelIndex &index)
{
    AbstractCopyModel *item = (AbstractCopyModel*) _model->itemFromIndex(index);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item->getWidget();
    _layout->addWidget(_activeWidget);

}

void COPYModelDialog::on_buttonBox_rejected()
{
    init();
}

void COPYModelDialog::closeEvent(QCloseEvent *event)
{
    on_buttonBox_rejected();
}

void COPYModelDialog::accept()
{
    bool ok = true;
    int count = _model->rowCount();
    QList<QString> liste;
    for (int i = 0 ; i < count && ok; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) _model->item(i);
        if (liste.contains(item->getAlias()) || !item->isValid())
        {
            ok = false;
        } else {
            liste.append(item->getAlias());
        }
    }

    if (!ok) {
        QMessageBox::warning(this, "Validation impossible", "Erreurs possibles :\n- Les alias ne sont pas tous définis et / ou uniques\n- Chaque résultat doit contenir exactement un groupe racine");
    } else {
        done(QDialog::Accepted);
    }
}

void COPYModelDialog::on_pb_cancelDelete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractCopyModel *item = (AbstractCopyModel*) _model->itemFromIndex(index);
    cancelDelete(item);
}
