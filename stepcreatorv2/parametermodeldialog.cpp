#include "parametermodeldialog.h"
#include "ui_parametermodeldialog.h"

#include "qdebug.h"
#include "qmessagebox.h"
#include "tools.h"
#include "parameters/abstractparameter.h"
#include "parameters/parameterbool.h"

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
    _activeWidget = NULL;

    //delete all children of parent;
    QStandardItem * root = _model->invisibleRootItem(); //main loop item

    while (root->hasChildren())
    {
        delete root->takeChild(0);
    }
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

void ParameterModelDialog::on_pb_bool_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();
    root->appendRow(new ParameterBool());
}

void ParameterModelDialog::on_pb_double_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_int_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_stringchoice_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_excludevalue_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_filechoice_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_text_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

}

void ParameterModelDialog::on_pb_empty_clicked()
{
    QStandardItem * root = _model->invisibleRootItem();

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
