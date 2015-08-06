#include "ct_manageconfigurableelementsdialog.h"
#include "ui_ct_manageconfigurableelementsdialog.h"

CT_ManageConfigurableElementsDialog::CT_ManageConfigurableElementsDialog(QList<CT_AbstractConfigurableElement *> &elements, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CT_ManageConfigurableElementsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->lb_title2->setText(title);

    _baseElements = elements;
    _listDialog = new CT_ElementListDialog(_baseElements, this);

    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pb_modify_clicked()));
    connect(this, SIGNAL(rejected()), this, SLOT(cancelClicked()));
    connect(ui->le_suffix, SIGNAL(textEdited(QString)), this, SLOT(trimLE(QString)));
}

CT_ManageConfigurableElementsDialog::~CT_ManageConfigurableElementsDialog()
{
    delete ui;
}

QList<CT_AbstractConfigurableElement *> CT_ManageConfigurableElementsDialog::getSeletedElements() const
{
    return _addedElements.values();
}

void CT_ManageConfigurableElementsDialog::setSuffix(QString suffixe)
{
    ui->lb_title2->setText(suffixe);
}

QString CT_ManageConfigurableElementsDialog::getSuffix()
{
    return ui->lb_title2->text();
}

void CT_ManageConfigurableElementsDialog::on_pb_new_clicked()
{
    _listDialog->exec();
    CT_AbstractConfigurableElement* selectedElement = _listDialog->getSelectedElement();

    if (selectedElement != NULL)
    {
        CT_AbstractConfigurableElement* newElement = selectedElement->copy();
        newElement->configure();
        newElement->postConfigure();

        QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
        newItem->setText(newElement->getCompleteName());
        newItem->setToolTip(newElement->getDetailledDescription());
        newItem->setSelected(true);
        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
        newItem->setCheckState(Qt::Checked);

        _addedElements.insert(newItem, newElement);
    }
}

void CT_ManageConfigurableElementsDialog::on_pb_modify_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    if (list.isEmpty()) {return;}

    QListWidgetItem* item = list.first();
    CT_AbstractConfigurableElement* selectedElement = _addedElements.value(item, NULL);

    if (selectedElement != NULL)
    {
        selectedElement->configure();
        item->setText(selectedElement->getCompleteName());
    }
}

void CT_ManageConfigurableElementsDialog::on_pb_delete_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    if (list.isEmpty()) {return;}

    QListWidgetItem *item = list.first();
    CT_AbstractConfigurableElement* selectedElement = _addedElements.take(item);

    if (selectedElement != NULL) {delete selectedElement;}
    if (item != NULL) {delete item;}
}

void CT_ManageConfigurableElementsDialog::cancelClicked()
{
    qDeleteAll(_addedElements.values());
    _addedElements.clear();
}

void CT_ManageConfigurableElementsDialog::trimLE(QString str)
{
    Q_UNUSED(str);
    ui->le_suffix->setText(ui->le_suffix->text().remove(QRegularExpression("\\W")));
}
