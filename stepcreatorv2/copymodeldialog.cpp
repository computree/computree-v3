#include "copymodeldialog.h"
#include "ui_copymodeldialog.h"

#include "qdebug.h"

#include "qmessagebox.h"

#include "models/copyresultmodel.h"
#include "models/copygroupmodel.h"
#include "models/copyitemmodel.h"
#include "models/abstractcopymodel.h"
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

}


QString COPYModelDialog::getCopyIncludes()
{
    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    QSet<QString> list;
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);
        item->getCopyModelsIncludesList(list);
    }

    return AbstractCopyModel::getQStringListConcat(list);
}

void COPYModelDialog::getCopyItemTypesIncludes(QSet<QString> &list)
{
    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);
        item->getCopyItemsTypesIncludesList(list);
    }
}

QString COPYModelDialog::getCopyDefines()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);
        result += item->getCopyModelsDefines();
    }

    return result;
}

QString COPYModelDialog::getCopyModelsDefinitions()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);

        result += item->getCopyModelsDefinition();
    }

    return result;
}

QString COPYModelDialog::getCopyComputeBeginning(int nbOfCopyResults)
{
    QString result = "";
    result += Tools::getIndentation(1) + "QList<CT_ResultGroup*> outResultList = getCopyResultList();\n";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);

        result += item->getCopyComputeBeginning(i + nbOfCopyResults);
    }

    return result;
}

QString COPYModelDialog::getCopyComputeItemsCreations()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) root->child(i);

        result += item->getCopyComputeItemsCreations();
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
    if (item != NULL)
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
            QStandardItem *root = _model->invisibleRootItem();
            root->takeRow(item->row());
        }

        itemsToBeDeleted.append(item);

        while (!itemsToBeDeleted.isEmpty())        {
            AbstractCopyModel* itemToDelete = (AbstractCopyModel*) itemsToBeDeleted.takeLast();
            if (itemToDelete->getWidget() == _activeWidget) {_activeWidget = NULL;}
            delete itemToDelete;
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
