#include "parametermodeldialog.h"
#include "ui_parametermodeldialog.h"

#include "qdebug.h"
#include "qmessagebox.h"
#include "tools.h"
#include "parameters/abstractparameter.h"
#include "parameters/parameterbool.h"
#include "parameters/parameterdouble.h"
#include "parameters/parameteremptyline.h"
#include "parameters/parameterexcludevalue.h"
#include "parameters/parameterfilechoice.h"
#include "parameters/parameterint.h"
#include "parameters/parameterstring.h"
#include "parameters/parameterstringchoice.h"
#include "parameters/parametertextline.h"

ParameterModelDialog::ParameterModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    _layout = new QVBoxLayout();
    ui->widgetZone->setLayout(_layout);
    _activeWidget = NULL;
}

ParameterModelDialog::~ParameterModelDialog()
{
    delete ui;
}


void ParameterModelDialog::on_treeView_clicked(const QModelIndex &index)
{
    AbstractParameter *item = (AbstractParameter*) _model->itemFromIndex(index);
    if (_activeWidget!=NULL)
    {
        _layout->removeWidget(_activeWidget);
        _activeWidget->setParent(0);
    }
    _activeWidget = item->getWidget();
    _layout->addWidget(_activeWidget);

}

void ParameterModelDialog::on_pb_clear_clicked()
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


QString ParameterModelDialog::getParametersDeclaration()
{
    QString result = "";
    int count = _model->rowCount();
    for (int i = 0 ; i < count; i++)
    {
        AbstractParameter* item = (AbstractParameter*) _model->item(i);
        result += item->getParameterDeclaration();
    }
    return result;
}

QString ParameterModelDialog::getParametersInitialization()
{
    QString result = "";
    int count = _model->rowCount();
    for (int i = 0 ; i < count; i++)
    {
        AbstractParameter* item = (AbstractParameter*) _model->item(i);
        result += item->getParameterInitialization();
    }
    return result;
}

QString ParameterModelDialog::getParametersDialogCommands()
{
    QString result = "";
    int count = _model->rowCount();
    for (int i = 0 ; i < count; i++)
    {
        AbstractParameter* item = (AbstractParameter*) _model->item(i);
        result += item->getParameterDialogCommands();
    }
    return result;
}

QString ParameterModelDialog::getParamatersDoc()
{
    QString result = "";
    int count = _model->rowCount();
    for (int i = 0 ; i < count; i++)
    {
        AbstractParameter* item = (AbstractParameter*) _model->item(i);
        result += item->getParamaterDoc();
    }
    return result;
}

void ParameterModelDialog::on_buttonBox_rejected()
{
    on_pb_clear_clicked();
}

void ParameterModelDialog::closeEvent(QCloseEvent *event)
{
    on_buttonBox_rejected();
}

void ParameterModelDialog::accept()
{
    bool ok = true;
    int count = _model->rowCount();
    QList<QString> liste;
    for (int i = 0 ; i < count && ok; i++)
    {
        AbstractParameter* item = (AbstractParameter*) _model->item(i);
        if (item->isDataParameter())
        {
            if (liste.contains(item->getName()) || !item->isValid())
            {
                ok = false;
            } else {
                liste.append(item->getName());
            }
        }
    }

    if (!ok) {
        QMessageBox::warning(this, "Validation impossible", "Les noms de paramètres ne sont pas tous définis et / ou uniques");
    } else {
        done(QDialog::Accepted);
    }
}

void ParameterModelDialog::addItem(QStandardItem* item)
{
    QStandardItem * root = _model->invisibleRootItem();
    root->appendRow(item);
    ui->treeView->setCurrentIndex(item->index());
    on_treeView_clicked(item->index());
    _activeWidget->setFocus();
}

void ParameterModelDialog::on_pb_bool_clicked()
{
    addItem(new ParameterBool());
}

void ParameterModelDialog::on_pb_double_clicked()
{
    addItem(new ParameterDouble());
}

void ParameterModelDialog::on_pb_int_clicked()
{
    addItem(new ParameterInt());
}

void ParameterModelDialog::on_pb_string_clicked()
{
    addItem(new ParameterString());
}

void ParameterModelDialog::on_pb_stringchoice_clicked()
{
    addItem(new ParameterStringChoice());
}

void ParameterModelDialog::on_pb_excludevalue_clicked()
{
    addItem(new ParameterExcludeValue());
}

void ParameterModelDialog::on_pb_filechoice_clicked()
{
    addItem(new ParameterFileChoice());
}

void ParameterModelDialog::on_pb_text_clicked()
{
    addItem(new ParameterTextLine());
}

void ParameterModelDialog::on_pb_empty_clicked()
{
    addItem(new ParameterEmptyLine());
}

void ParameterModelDialog::on_pb_up_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractParameter *item = (AbstractParameter*) _model->itemFromIndex(index);

    int row = item->row();
    if (row >0)
    {
        if (_activeWidget!=NULL)
        {
            _layout->removeWidget(_activeWidget);
            _activeWidget->setParent(0);
            _activeWidget = NULL;
        }

        _model->takeRow(row);
        _model->insertRow(row-1, item);

        ui->treeView->setCurrentIndex(item->index());
        _activeWidget = item->getWidget();
        _layout->addWidget(_activeWidget);
    }

}

void ParameterModelDialog::on_pb_down_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    AbstractParameter *item = (AbstractParameter*) _model->itemFromIndex(index);
    int row = item->row();
    if (row < (_model->rowCount()-1))
    {

        if (_activeWidget!=NULL)
        {
            _layout->removeWidget(_activeWidget);
            _activeWidget->setParent(0);
            _activeWidget = NULL;
        }

        _model->takeRow(row);
        _model->insertRow(row+1, item);
        ui->treeView->setCurrentIndex(item->index());
        _activeWidget = item->getWidget();
        _layout->addWidget(_activeWidget);
    }
}
