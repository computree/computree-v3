#include "ct_manageconfigurableelementsdialog.h"
#include "ui_ct_manageconfigurableelementsdialog.h"

CT_ManageConfigurableElementsDialog::CT_ManageConfigurableElementsDialog(QString title, QList<CT_AbstractConfigurableElement *> &elements, QList<CT_AbstractConfigurableElement *> *selectedElements, QWidget *parent) :
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

    _selectedElements = selectedElements;

    for (int i = 0 ; i < _selectedElements->size() ; i++)
    {
        CT_AbstractConfigurableElement* newElement = (CT_AbstractConfigurableElement*) _selectedElements->at(i);

        QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
        newItem->setText(newElement->getCompleteName());
        newItem->setToolTip(QString("<b>%1</b><br>%2").arg(newElement->getShortDescription()).arg(newElement->getDetailledDescription()));
        newItem->setSelected(true);
        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
        newItem->setCheckState(Qt::Checked);

        _addedElements.insert(newItem, newElement);
    }
}

CT_ManageConfigurableElementsDialog::~CT_ManageConfigurableElementsDialog()
{
    delete ui;
}

QString CT_ManageConfigurableElementsDialog::getConfig() const
{
    QString config;

    for (int i = 0 ; i < _selectedElements->size() ; i++)
    {
        CT_AbstractConfigurableElement* element = (CT_AbstractConfigurableElement*) _selectedElements->at(i);

        config.append(element->getName());
        config.append("\t");
        config.append(element->getParametersAsString());
        if (i < (_selectedElements->size() - 1)) {config.append("\n");}
    }

    return config;
}

QString CT_ManageConfigurableElementsDialog::setConfig(const QString &config)
{
    QString error = "";
    QStringList elementsAsString = config.split("\n", QString::SkipEmptyParts);

    for (int i = 0 ; i < elementsAsString.size() ; i++)
    {
        QString str = elementsAsString.at(i);
        QStringList parts = str.split("\t", QString::SkipEmptyParts);

        if (parts.size() > 0)
        {
            QString name = parts.first();
            QString param = "";
            if (parts.size() > 1)
            {
                param = parts.at(1);
            }

            bool found = false;
            for (int j = 0 ; j < _baseElements.size() && !found; j++)
            {
                CT_AbstractConfigurableElement* element = (CT_AbstractConfigurableElement*) _baseElements.at(j);
                if (element->getName() == name)
                {
                    CT_AbstractConfigurableElement* newElement = element->copy();
                    if (newElement->setParametersFromString(param))
                    {
                        newElement->postConfigure();

                        QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
                        newItem->setText(newElement->getCompleteName());
                        newItem->setToolTip(QString("<b>%1</b><br>%2").arg(newElement->getShortDescription()).arg(newElement->getDetailledDescription()));
                        newItem->setSelected(true);
                        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
                        newItem->setCheckState(Qt::Checked);

                        _addedElements.insert(newItem, newElement);
                        _selectedElements->append(newElement);

                        found = true;
                    }
                }

                if (!found)
                {
                    error.append(tr("Element %1 not available\n").arg(name));
                }
            }
        } else {
            error.append(tr("Unknown Error for element : %1\n").arg(str));
        }
    }
    return error;
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
        newItem->setToolTip(QString("<b>%1</b><br>%2").arg(newElement->getShortDescription()).arg(newElement->getDetailledDescription()));
        newItem->setSelected(true);
        newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
        newItem->setCheckState(Qt::Checked);

        _addedElements.insert(newItem, newElement);
        _selectedElements->append(newElement);
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
    _selectedElements->removeOne(selectedElement);


    if (selectedElement != NULL) {delete selectedElement;}
    if (item != NULL) {delete item;}
}

void CT_ManageConfigurableElementsDialog::cancelClicked()
{
    qDeleteAll(*_selectedElements);
    _addedElements.clear();
    _selectedElements->clear();
}

void CT_ManageConfigurableElementsDialog::trimLE(QString str)
{
    Q_UNUSED(str);
    ui->le_suffix->setText(ui->le_suffix->text().remove(QRegularExpression("\\W")));
}
