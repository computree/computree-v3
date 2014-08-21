#include "pbg_csvdatareflistwidget.h"
#include "ui_pbg_csvdatareflistwidget.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

#include <QMimeData>
#include <QModelIndex>

QMimeData* PBG_CSVDataRefListWidgetDragModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = QStandardItemModel::mimeData(indexes);

    if(!indexes.isEmpty())
        mimeData->setText(indexes.first().data(Qt::UserRole+1).toString());

    return mimeData;
}

PBG_CSVDataRefListWidget::PBG_CSVDataRefListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PBG_CSVDataRefListWidget)
{
    ui->setupUi(this);

    ui->treeView->setModel(&_model);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->treeView->setDragEnabled(true);

    createHeader();
}

PBG_CSVDataRefListWidget::~PBG_CSVDataRefListWidget()
{
    delete ui;
}

void PBG_CSVDataRefListWidget::setList(const QList<CT_OutAbstractSingularItemModel*> &list)
{
    _list = list;

    initView();
}

// PRIVATE //

void PBG_CSVDataRefListWidget::createHeader()
{
    QStringList headers;
    headers << tr("Nom");
    headers << tr("Type");

    _model.setHorizontalHeaderLabels(headers);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
#else
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
}

void PBG_CSVDataRefListWidget::initView()
{
    _model.invisibleRootItem()->removeRows(0, _model.invisibleRootItem()->rowCount());

    QListIterator<CT_OutAbstractSingularItemModel*> it(_list);

    while(it.hasNext())
    {
        CT_OutAbstractSingularItemModel *sItem = it.next();

        QList<QStandardItem*> items = createItemsForSingularItem(sItem);

        _model.invisibleRootItem()->appendRow(items);

        QStandardItem *item = items.first();

        QListIterator<CT_OutAbstractItemAttributeModel*> itR(sItem->itemAttributes());

        while(itR.hasNext())
        {
            QList<QStandardItem*> items = createItemsForItemAttribute(sItem, itR.next());

            item->appendRow(items);
        }
    }

    ui->treeView->expandAll();
}

QList<QStandardItem*> PBG_CSVDataRefListWidget::createItemsForSingularItem(const CT_OutAbstractSingularItemModel *sModel) const
{
    QList<QStandardItem*> items;

    QStandardItem *item = new QStandardItem(sModel->displayableName());
    item->setEditable(false);
    item->setDragEnabled(false);
    items.append(item);

    return items;
}

QList<QStandardItem*> PBG_CSVDataRefListWidget::createItemsForItemAttribute(const CT_OutAbstractSingularItemModel *sModel,
                                                                            const CT_OutAbstractItemAttributeModel *iaModel) const
{
    QList<QStandardItem*> items;

    // drag data = index du modèle de l'item dans la liste ;; index du modèle de l'attribute dans la liste des attributs d'item
    QString dragText = QString("%1;;%2").arg(_list.indexOf((CT_OutAbstractSingularItemModel*)sModel)).arg(sModel->itemAttributes().indexOf((CT_OutAbstractItemAttributeModel*)iaModel));

    QStandardItem *item = new QStandardItem(iaModel->displayableName());
    item->setEditable(false);
    item->setDragEnabled(true);
    item->setData(dragText, Qt::UserRole+1);

    items.append(item);

    item = new QStandardItem(iaModel->itemAttribute()->typeToString());
    item->setEditable(false);
    item->setDragEnabled(true);
    item->setData(dragText, Qt::UserRole+1);

    items.append(item);

    return items;
}

void PBG_CSVDataRefListWidget::on_treeView_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = _model.itemFromIndex(index);

    if(item->data().isValid())
        emit attributeChoosed(item->data().toString());
}

void PBG_CSVDataRefListWidget::on_pushButtonAddAll_clicked()
{
    int size = _model.invisibleRootItem()->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *item = _model.invisibleRootItem()->child(i);

        int sizeI = item->rowCount();

        for(int j=0; j<sizeI; ++j)
            emit attributeChoosed(item->child(j)->data().toString());
    }
}
