#include "ct_elementlistdialog.h"
#include "ui_ct_elementlistdialog.h"

CT_ElementListDialog::CT_ElementListDialog(QList<CT_AbstractConfigurableElement *> &elements, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CT_ElementListDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Choisissez un élément"));

    bool first = true;

    QListIterator<CT_AbstractConfigurableElement *> it(elements);
    while (it.hasNext())
    {
        CT_AbstractConfigurableElement* element = it.next();

        QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
        newItem->setText(element->getName());
        newItem->setToolTip(QString("<b>%1</b><br>%2").arg(element->getShortDescription()).arg(element->getDetailledDescription()));

        _elements.insert(newItem, element);

        if (first) {newItem->setSelected(true); first = false;}
    }
}

CT_ElementListDialog::~CT_ElementListDialog()
{
    delete ui;
}

CT_AbstractConfigurableElement *CT_ElementListDialog::getSelectedElement()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {return NULL;}

    return _elements.value(selectedItems.first(), NULL);
}
