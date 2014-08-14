#include "pbg_csvdatareflistwidget.h"
#include "ui_pbg_csvdatareflistwidget.h"

#include "interfaces.h"

#include <QMimeData>
#include <QModelIndex>

QMimeData* PBG_CSVDataRefListWidgetDragModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = QStandardItemModel::mimeData(indexes);

    if(!indexes.isEmpty())
    {
        mimeData->setText(indexes.first().data(Qt::UserRole+1).toString());
    }

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

void PBG_CSVDataRefListWidget::setListOfDataRefList(const QList<const IItemDataRefList*> *list)
{
    if(list == NULL)
        _list.clear();
    else
        _list = (*list);

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

    QListIterator<const IItemDataRefList*> it(_list);

    while(it.hasNext())
    {
        const IItemDataRefList *refList = it.next();

        QList<QStandardItem*> items = createItemsForIItemDataRefList(refList);

        _model.invisibleRootItem()->appendRow(items);

        QStandardItem *item = items.first();

        QListIterator<IItemDataRef*> itR(refList->references());

        while(itR.hasNext())
        {
            QList<QStandardItem*> items = createItemsForIItemDataRef(refList, itR.next());

            item->appendRow(items);
        }
    }

    ui->treeView->expandAll();
}

QList<QStandardItem*> PBG_CSVDataRefListWidget::createItemsForIItemDataRefList(const IItemDataRefList *list) const
{
    QList<QStandardItem*> items;

    QStandardItem *item = new QStandardItem(list->name());
    item->setEditable(false);
    item->setDragEnabled(false);
    items.append(item);

    return items;
}

QList<QStandardItem*> PBG_CSVDataRefListWidget::createItemsForIItemDataRef(const IItemDataRefList *refList,
                                                                           const IItemDataRef *ref) const
{
    QList<QStandardItem*> items;

    // drag data = index du IItemDataRefList dans la liste ;; index de la référence dans la liste de IItemDataRefList
    QString dragText = QString("%1;;%2").arg(_list.indexOf(refList)).arg(refList->references().indexOf((IItemDataRef*)ref));


    QStandardItem *item = new QStandardItem(ref->displayableName());
    item->setEditable(false);
    item->setDragEnabled(true);
    item->setData(dragText, Qt::UserRole+1);

    items.append(item);

    if(ref->dataType() == IItemDataValue::IDVT_BOOL)
        item = new QStandardItem(tr("Bool"));
    else if(ref->dataType() == IItemDataValue::IDVT_INT)
        item = new QStandardItem(tr("Int"));
    else if(ref->dataType() == IItemDataValue::IDVT_UINT64)
        item = new QStandardItem(tr("Int64"));
    else if(ref->dataType() == IItemDataValue::IDVT_SIZE_T)
        item = new QStandardItem(tr("SizeT"));
    else if(ref->dataType() == IItemDataValue::IDVT_DOUBLE)
        item = new QStandardItem(tr("Double"));
    else if(ref->dataType() == IItemDataValue::IDVT_LONG_DOUBLE)
        item = new QStandardItem(tr("Long Double"));
    else if(ref->dataType() == IItemDataValue::IDVT_FLOAT)
        item = new QStandardItem(tr("Float"));
    else if(ref->dataType() == IItemDataValue::IDVT_STRING)
        item = new QStandardItem(tr("String"));
    else if(ref->dataType() == IItemDataValue::IDVT_INT_LIST)
        item = new QStandardItem(tr("IntList"));
    else if(ref->dataType() == IItemDataValue::IDVT_DOUBLE_LIST)
        item = new QStandardItem(tr("DoubleList"));
    else if(ref->dataType() == IItemDataValue::IDVT_FLOAT_LIST)
        item = new QStandardItem(tr("FloatList"));
    else if(ref->dataType() == IItemDataValue::IDVT_STRING_LIST)
        item = new QStandardItem(tr("StringList"));
    else if(ref->dataType() == IItemDataValue::IDVT_BOOL_1D_ARRAY)
        item = new QStandardItem(tr("Bool 1D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_INT_1D_ARRAY)
        item = new QStandardItem(tr("Int 1D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_DOUBLE_1D_ARRAY)
        item = new QStandardItem(tr("Double 1D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_FLOAT_1D_ARRAY)
        item = new QStandardItem(tr("Float 1D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_STRING_1D_ARRAY)
        item = new QStandardItem(tr("String 1D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_BOOL_2D_ARRAY)
        item = new QStandardItem(tr("Bool 2D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_INT_2D_ARRAY)
        item = new QStandardItem(tr("Int 2D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_DOUBLE_2D_ARRAY)
        item = new QStandardItem(tr("Double 2D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_FLOAT_2D_ARRAY)
        item = new QStandardItem(tr("Float 2D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_STRING_2D_ARRAY)
        item = new QStandardItem(tr("String 2D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_BOOL_3D_ARRAY)
        item = new QStandardItem(tr("Bool 3D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_INT_3D_ARRAY)
        item = new QStandardItem(tr("Int 3D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_DOUBLE_3D_ARRAY)
        item = new QStandardItem(tr("Double 3D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_FLOAT_3D_ARRAY)
        item = new QStandardItem(tr("Float 3D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_STRING_3D_ARRAY)
        item = new QStandardItem(tr("String 3D Array"));
    else if(ref->dataType() == IItemDataValue::IDVT_POINT_CLOUD_DATA)
        item = new QStandardItem(tr("PointCloudData"));
    else
        item = new QStandardItem(tr("Unknown"));

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
