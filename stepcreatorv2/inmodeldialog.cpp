#include "inmodeldialog.h"
#include "ui_inmodeldialog.h"

#include "widgets/inresultwidget.h"
INModelDialog::INModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::INModelDialog)
{
    ui->setupUi(this);
    _model = new QStandardItemModel();
    ui->treeView->setModel(_model);
    QVBoxLayout *layout = new QVBoxLayout();
    ui->widgetZone->setLayout(layout);
    layout->addWidget(new INResultWidget());
}

INModelDialog::~INModelDialog()
{
    delete ui;
}

void INModelDialog::on_pb_addResult_clicked()
{
    QStandardItem *item = _model->itemFromIndex(ui->treeView->currentIndex());
    if (item == NULL)
    {
        item = _model->invisibleRootItem();
    }
    item->appendRow(new QStandardItem("item"));
    ui->treeView->setExpanded(ui->treeView->currentIndex(), true);
}

void INModelDialog::on_pb_delete_clicked()
{
    QStandardItem *item = _model->itemFromIndex(ui->treeView->currentIndex());
    if (item != NULL)
    {
    }
}
