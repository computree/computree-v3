#include "outmodeldialog.h"
#include "ui_outmodeldialog.h"

#include "qdebug.h"

#include "qmessagebox.h"

#include "models/outresultmodel.h"
#include "models/outgroupmodel.h"
#include "models/outitemmodel.h"
#include "models/abstractoutmodel.h"
#include "tools.h"

OUTModelDialog::OUTModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OUTModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    _layout = new QVBoxLayout();
    ui->widgetZone->setLayout(_layout);
    _activeWidget = NULL;
}

OUTModelDialog::~OUTModelDialog()
{
    delete ui;
}

QString OUTModelDialog::getOutIncludes()
{
    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    QSet<QString> list;
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);
        item->getOutModelsIncludesList(list);
    }

    return AbstractOutModel::getQStringListConcat(list);
}

void OUTModelDialog::getOutItemTypesIncludes(QSet<QString> &list)
{
    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);
        item->getOutItemsTypesIncludesList(list);
    }
}

QString OUTModelDialog::getOutDefines()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);
        result += item->getOutModelsDefines();
    }

    return result;
}

QString OUTModelDialog::getOutModelsDefinitions()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);

        result += item->getOutModelsDefinition();
    }

    return result;
}

QString OUTModelDialog::getOutComputeBeginning(int nbOfCopyResults)
{
    QString result = "";
    result += Tools::getIndentation(1) + "QList<CT_ResultGroup*> outResultList = getOutResultList();\n";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);

        result += item->getOutComputeBeginning(i + nbOfCopyResults);
    }

    return result;
}

QString OUTModelDialog::getOutComputeItemsCreations()
{
    QString result = "";

    QStandardItem* root = _model->invisibleRootItem();
    int count = root->rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) root->child(i);

        result += item->getOutComputeItemsCreations();
    }

    return result;
}


void OUTModelDialog::on_pb_addResult_clicked()
{
    QStandardItem *item = _model->invisibleRootItem();

    OUTResultModel *item2 = new OUTResultModel();

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


void OUTModelDialog::on_pb_addGroup_clicked()
{
    AbstractOutModel *item = (AbstractOutModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }

    if (item->getModelType()!=AbstractOutModel::M_Result_OUT && item->getModelType()!=AbstractOutModel::M_Group_OUT)
    {
        return;
    }

    if (item->getModelType()==AbstractOutModel::M_Result_OUT && item->rowCount()!=0)
    {
        return;
    }


    OUTGroupModel *item2 = new OUTGroupModel();

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


void OUTModelDialog::on_pb_addItem_clicked()
{
    AbstractOutModel *item = (AbstractOutModel*) _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        return;
    }
    if (item->getModelType()!=AbstractOutModel::M_Group_OUT)
    {
        return;
    }

    OUTItemModel *item2 = new OUTItemModel();

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

void OUTModelDialog::on_pb_delete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractOutModel *item = (AbstractOutModel*) _model->itemFromIndex(index);
    ui->treeView->clearSelection();
    if (item != NULL)
    {
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
            AbstractOutModel* itemToDelete = (AbstractOutModel*) itemsToBeDeleted.takeLast();
            if (itemToDelete->getWidget() == _activeWidget) {_activeWidget = NULL;}
            delete itemToDelete;
        }

    }
}

void OUTModelDialog::on_treeView_clicked(const QModelIndex &index)
{
    AbstractOutModel *item = (AbstractOutModel*) _model->itemFromIndex(index);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item->getWidget();
    _layout->addWidget(_activeWidget);

}

void OUTModelDialog::on_pb_clear_clicked()
{
    ui->treeView->clearSelection();

    //delete all children of parent;
    QStandardItem * loopItem = _model->invisibleRootItem();; //main loop item
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
    qDeleteAll(itemsToBeDeleted);
    _activeWidget = NULL;
}

void OUTModelDialog::on_buttonBox_rejected()
{
    on_pb_clear_clicked();
}

void OUTModelDialog::accept()
{
    bool ok = true;
    int count = _model->rowCount();
    QList<QString> liste;
    for (int i = 0 ; i < count && ok; i++)
    {
        AbstractOutModel* item = (AbstractOutModel*) _model->item(i);
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
