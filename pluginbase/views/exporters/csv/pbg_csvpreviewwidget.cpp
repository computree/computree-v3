#include "pbg_csvpreviewwidget.h"
#include "ui_pbg_csvpreviewwidget.h"

#include "interfaces.h"
#include "exporters/csv/pb_csvexporterconfiguration.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPropertyAnimation>
#include <QMimeData>

PBG_CSVPreviewWidget::PBG_CSVPreviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PBG_CSVPreviewWidget)
{
    ui->setupUi(this);

    ui->tableView->setModel(&_model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    _configuration = NULL;
    _currentDropPreviewColumnIndex = -9999;
    _headerContextMenu = new QMenu(this);
    QAction *action = _headerContextMenu->addAction("Supprimer");

    connect(ui->tableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showHorizontalHeaderViewContextMenu(QPoint)));
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedColumn()));

    setAcceptDrops(true);
}

PBG_CSVPreviewWidget::~PBG_CSVPreviewWidget()
{
    delete ui;
    delete _headerContextMenu;
}

void PBG_CSVPreviewWidget::setConfiguration(const PB_CSVExporterConfiguration *configuration)
{
    _configuration = (PB_CSVExporterConfiguration*)configuration;

    updateViewFromConfiguration();
}

void PBG_CSVPreviewWidget::setListOfDataRefList(const QList<const IItemDataRefList*> *list)
{
    if(list == NULL)
        _list.clear();
    else
        _list = (*list);

    updateViewFromConfiguration();
}

bool PBG_CSVPreviewWidget::updateConfiguration()
{
    if(_configuration == NULL)
        return false;

    _configuration->clearColumns();

    int n = _model.columnCount();

    for(int i=0; i<n; ++i)
    {
        QStandardItem *item = _model.horizontalHeaderItem(i);

        int refListIndex = item->data(Qt::UserRole).toInt();
        int refIndex = item->data(Qt::UserRole+1).toInt();

        const IItemDataRefList *refList = _list.at(refListIndex);
        _configuration->addColumn(refList, refList->references().at(refIndex));
    }

    return true;
}

// PROTECTED //

void PBG_CSVPreviewWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if((event->source() != NULL)
            && acceptItemAttribute(event->mimeData()->text()))
    {
        event->acceptProposedAction();
    }
}

void PBG_CSVPreviewWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if((event->source() != NULL))
    {
        QStringList values = event->mimeData()->text().split(";;");

        if(values.size() == 2)
        {
            bool ok;
            int refListIndex = values.first().toInt(&ok);

            if(!ok)
                return;

            int refIndex = values.at(1).toInt(&ok);

            if(ok)
            {
                if((refListIndex >= 0) && (refListIndex < _list.size()))
                {
                    const IItemDataRefList *refList = _list.at(refListIndex);

                    if((refIndex >= 0) && (refIndex < refList->references().size()))
                    {
                        IItemDataRef *ref = refList->references().at(refIndex);

                        createColumn(refList, ref, event->pos(), true);
                    }
                }
            }
        }
    }
}

void PBG_CSVPreviewWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    if(_currentDropPreviewColumnIndex >= 0)
    {
        _model.removeColumn(_currentDropPreviewColumnIndex);
        _currentDropPreviewColumnIndex = -9999;
    }
}

void PBG_CSVPreviewWidget::dropEvent(QDropEvent *event)
{
    setItemAttribute(event->mimeData()->text(), event);
}

void PBG_CSVPreviewWidget::keyPressEvent(QKeyEvent *e)
{
    if(!e->isAutoRepeat()
            && e->key() == Qt::Key_Delete)
        removeSelectedColumn();
}

void PBG_CSVPreviewWidget::createColumn(const IItemDataRefList *refList,
                                        IItemDataRef *ref,
                                        const QPoint &pos,
                                        bool preview)
{
    int columnIndex = ui->tableView->columnAt(pos.x());

    if(columnIndex < 0)
    {
        if((_model.invisibleRootItem()->columnCount() == 1)
                && (_currentDropPreviewColumnIndex == 0))
        {
            columnIndex = 0;
        }
        else if(_model.invisibleRootItem()->columnCount() > 0)
        {
           int lastX =  ui->tableView->columnViewportPosition(_model.invisibleRootItem()->columnCount()-1);

           if(pos.x() > lastX)
               columnIndex = _model.invisibleRootItem()->columnCount();

           if((_currentDropPreviewColumnIndex+1) == columnIndex)
               columnIndex = _currentDropPreviewColumnIndex;
        }
        else
        {
            columnIndex = 0;
        }
    }
    else if((_currentDropPreviewColumnIndex >=0)
            && ((_currentDropPreviewColumnIndex+1) == columnIndex))
    {
        columnIndex = _currentDropPreviewColumnIndex;
    }

    if((_currentDropPreviewColumnIndex != columnIndex)
            || !preview)
    {
        if(_currentDropPreviewColumnIndex >= 0)
        {
            _model.removeColumn(_currentDropPreviewColumnIndex);

            if(columnIndex > _currentDropPreviewColumnIndex)
                --columnIndex;
        }

        bool addColumn = true;

        if(!preview)
            addColumn = ref->configure(); // TODO : problème si on ajoute plusieurs fois la même ref

        if(addColumn)
            createColumn(refList, ref, columnIndex, preview);
    }

    if(!preview)
    {
        _currentDropPreviewColumnIndex = -9999;
    }
}

void PBG_CSVPreviewWidget::createColumn(const IItemDataRefList *refList,
                                        IItemDataRef *ref,
                                        int columnIndex,
                                        bool preview)
{
    QStandardItem *item = new QStandardItem();

    if(preview)
        item->setData(QColor(Qt::gray), Qt::BackgroundColorRole);

    _model.insertColumn(columnIndex, QList<QStandardItem*>() << item);

    item = new QStandardItem();
    item->setData(ref->displayableName(), Qt::DisplayRole);
    item->setData(_list.indexOf(refList), Qt::UserRole);
    item->setData(refList->references().indexOf(ref), Qt::UserRole+1);

    if(preview)
        item->setData(QColor(Qt::gray), Qt::BackgroundColorRole);

    _model.setHorizontalHeaderItem(columnIndex, item);

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    #else
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    #endif

    if(preview)
    {
        _currentDropPreviewColumnIndex = columnIndex;

        QPropertyAnimation *animation = new QPropertyAnimation(this, "currentDropPreviewColumnWidth", this);
        animation->setDuration(200);
        animation->setStartValue(0);
        animation->setEndValue(ui->tableView->columnWidth(columnIndex));

        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
        #else
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        #endif

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        #else
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        #endif
    }
}

int PBG_CSVPreviewWidget::currentDropPreviewColumnWidth() const
{
    if(_currentDropPreviewColumnIndex >= 0)
    {
        return ui->tableView->columnWidth(_currentDropPreviewColumnIndex);
    }

    return 0;
}

void PBG_CSVPreviewWidget::updateViewFromConfiguration()
{
    while(_model.columnCount() > 0)
        _model.removeColumn(0);

    if(_configuration != NULL)
    {
        QListIterator< QPair<const IItemDataRefList*, const IItemDataRef*> > it(_configuration->getColumns());

        while(it.hasNext())
        {
            const QPair<const IItemDataRefList*, const IItemDataRef*> &pair = it.next();
            createColumn(pair.first, (IItemDataRef*)pair.second, _model.columnCount(), false);
        }
    }
}

bool PBG_CSVPreviewWidget::acceptItemAttribute(const QString &mimeData)
{
    // on accepte le drag si c'est un texte qui contient deux élément
    // séparés par ";;" (le premier étant l'index du IItemDataRefList dans la liste
    // et le second l'index du IItemDataRef dans la liste des références de IItemDataRefList
    QStringList values = mimeData.split(";;");

    if(values.size() == 2)
    {
        bool ok;
        int refListIndex = values.first().toInt(&ok);

        if(!ok)
            return false;

        int refIndex = values.at(1).toInt(&ok);

        if(ok)
        {
            if((refListIndex >= 0) && (refListIndex < _list.size()))
            {
                const IItemDataRefList *refList = _list.at(refListIndex);

                if((refIndex >= 0) && (refIndex < refList->references().size()))
                    return true;
            }
        }
    }

    return false;
}

void PBG_CSVPreviewWidget::setCurrentDropPreviewColumnWidth(int size)
{
    if(_currentDropPreviewColumnIndex >= 0)
    {
        ui->tableView->setColumnWidth(_currentDropPreviewColumnIndex, size);
    }
}

void PBG_CSVPreviewWidget::showHorizontalHeaderViewContextMenu(const QPoint &pos)
{
    ui->tableView->selectColumn(ui->tableView->columnAt(pos.x()));
    _headerContextMenu->exec(ui->tableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}

void PBG_CSVPreviewWidget::removeSelectedColumn()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();

    if(!list.isEmpty())
    {
        QStandardItem *item = _model.itemFromIndex(list.first());

        if(item != NULL)
        {
            _model.removeColumn(item->column());
        }
    }
}

void PBG_CSVPreviewWidget::setItemAttribute(const QString &mimeData, QDropEvent *event)
{
    if(acceptItemAttribute(mimeData))
    {
        QStringList values = mimeData.split(";;");

        int refListIndex = values.first().toInt();
        int refIndex = values.at(1).toInt();

        const IItemDataRefList *refList = _list.at(refListIndex);
        IItemDataRef *ref = refList->references().at(refIndex);

        if(event != NULL)
            createColumn(refList, ref, event->pos(), false);
        else
            createColumn(refList, ref, _model.columnCount(), false);

    }
}
