#include "gitemdrawablemodelmanager.h"
#include "ui_gitemdrawablemodelmanager.h"

#include "interfaces.h"

#include "view/DocumentView/gdocumentmanagerview.h"
#include "view/Tools/modelqstandarditem.h"

#include "dm_guimanager.h"
#include "dm_documentview.h"
#include "qtcolorpicker.h"

GItemDrawableModelManager::GItemDrawableModelManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GItemDrawableModelManager)
{
    ui->setupUi(this);

    m_docManagerView = NULL;
    _result = NULL;
    _contextMenu = constructContextMenu();
    _colorPicker = new QtColorPicker(NULL, -1, true, false);

    ui->treeView->setModel(&_viewModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)), Qt::QueuedConnection);

    constructHeader();
}

GItemDrawableModelManager::~GItemDrawableModelManager()
{
    delete ui;
    delete _colorPicker;
}

void GItemDrawableModelManager::setDocumentManagerView(const GDocumentManagerView *docManagerView)
{
    if(m_docManagerView != NULL)
    {
        disconnect(m_docManagerView, NULL, this, NULL);
    }

    m_docManagerView = (GDocumentManagerView*)docManagerView;

    if(m_docManagerView != NULL)
    {
        connect(m_docManagerView, SIGNAL(documentAdded(DM_DocumentView*)), this, SLOT(documentAdded(DM_DocumentView*)));
        connect(m_docManagerView, SIGNAL(documentRemoved()), this, SLOT(documentRemoved()));
        connect(m_docManagerView, SIGNAL(documentHidden(DM_DocumentView*)), this, SLOT(refreshCheckbox()));
        connect(m_docManagerView, SIGNAL(documentShowned(DM_DocumentView*)), this, SLOT(refreshCheckbox()));
    }
}

void GItemDrawableModelManager::setResult(const Result *res)
{
    if(_result != NULL)
        disconnect(_result, NULL, this, NULL);

    _result = (Result*)res;

    if(_result != NULL)
    {
        connect(_result, SIGNAL(busyStateChanged(bool)), this, SLOT(refreshCheckbox()), Qt::QueuedConnection);
        connect(_result, SIGNAL(destroyed()), this, SLOT(resultDestroyedDirect()), Qt::DirectConnection);
    }

    constructModel();
}

void GItemDrawableModelManager::setColorOptions(const DM_ItemDrawableManagerOptions &options)
{
    _colorOptions = options;
}

Result* GItemDrawableModelManager::result() const
{
    return _result;
}

QList<const IModel*> GItemDrawableModelManager::getSelectedIModels() const
{
    QList<const IModel*> retList;

    QModelIndexList list = ui->treeView->selectionModel()->selectedRows(1);

    QListIterator<QModelIndex> it(list);

    while(it.hasNext())
    {
        ModelQStandardItem *mItem = dynamic_cast<ModelQStandardItem*>(_viewModel.itemFromIndex(it.next()));

        if((mItem != NULL)
                && (mItem->model() != NULL))
        {
            retList.append(mItem->getModel());
        }
    }

    return retList;
}

// PRIVATE //

void GItemDrawableModelManager::clearModel()
{
    _viewModel.invisibleRootItem()->removeRows(0, _viewModel.invisibleRootItem()->rowCount());
}

void GItemDrawableModelManager::constructModel()
{
    clearModel();
    constructHeader();

    if((_result != NULL)
            && (_result->getModel() != NULL))
    {
        _viewModel.invisibleRootItem()->appendRow(recursiveCreateItemsForResultModel(_result->getModel()));

        ui->treeView->expandAll();

        refreshCheckbox();
    }
}

void GItemDrawableModelManager::constructHeader()
{
    QStringList header;
    header << tr("Nom");

    if(m_docManagerView != NULL)
    {
        int s = m_docManagerView->nbDocumentView();

        for(int i=0; i<s; ++i)
            header << QString().setNum(m_docManagerView->getDocumentView(i)->getNumber());
    }

    _viewModel.setHorizontalHeaderLabels(header);
    _viewModel.setColumnCount(header.size());

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
#else
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
#endif
}

QMenu* GItemDrawableModelManager::constructContextMenu()
{
    QMenu *contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Couleur uni"), this, SLOT(setUniqueColorForModelSelected()));
    contextMenu->addAction(tr("Couleur automatique"), this, SLOT(setAutomaticColorForModelSelected()));

    return contextMenu;
}

QList<QStandardItem*> GItemDrawableModelManager::recursiveCreateItemsForResultModel(const IResultModel *model)
{
    QList<QStandardItem*> retList = createItemsForResultModel(model);
    QStandardItem *rootItem = retList.first();

    QList<IItemModel*> items = model->getRootItemModel();
    QListIterator<IItemModel*> it(items);

    while(it.hasNext())
    {
        QList<QStandardItem*> list = recursiveCreateItemsForItemModel(it.next());

        rootItem->appendRow(list);
    }

    return retList;
}

QList<QStandardItem*> GItemDrawableModelManager::recursiveCreateItemsForItemModel(const IItemModel *model)
{
    QList<QStandardItem*> retList = createItemsForItemModel(model);
    QStandardItem *rootItem = retList.first();

    QList<IItemModel*> items = model->getChildrenItemModel();
    QListIterator<IItemModel*> it(items);

    while(it.hasNext())
    {
        QList<QStandardItem*> list = recursiveCreateItemsForItemModel(it.next());

        rootItem->appendRow(list);
    }

    return retList;
}

QList<QStandardItem*> GItemDrawableModelManager::createItemsForResultModel(const IResultModel *model)
{
    QList<QStandardItem*> retList;

    QStandardItem *item = new QStandardItem(model->displayableName());
    item->setEditable(false);
    retList.append(item);

    item = new QStandardItem();
    item->setEditable(false);
    retList.append(item);

    return retList;
}

QList<QStandardItem*> GItemDrawableModelManager::createItemsForItemModel(const IItemModel *model)
{
    QList<QStandardItem*> retList;

    QStandardItem *item = new QStandardItem(model->displayableName());
    item->setEditable(false);
    retList.append(item);

    if(m_docManagerView != NULL)
    {
        int size = m_docManagerView->nbDocumentView();

        for(int i=0; i<size; ++i)
        {
            DM_DocumentView *view = m_docManagerView->getDocumentView(i);

            ModelQStandardItem *mItem = new ModelQStandardItem();
            mItem->setModel(model);
            mItem->setEditable(false);
            mItem->setCheckable(true);
            mItem->setCheckState(model->isVisibleInDocument(view) ? Qt::Checked : Qt::Unchecked);
            mItem->setData(qVariantFromValue((void*)view));
            retList.append(mItem);
        }
    }

    connect(model->getSignalEmitter(), SIGNAL(visibilityInDocumentChanged(const DocumentInterface*,bool)), this, SLOT(modelVisibilityInDocumentChanged(const DocumentInterface*,bool)), Qt::QueuedConnection);

    return retList;
}

QStandardItem* GItemDrawableModelManager::recursiveGetStandardItemForModel(QStandardItem *pItem, IItemModel *model, int column) const
{
    int count = pItem->rowCount();

    for(int i=0; i<count; ++i)
    {
        ModelQStandardItem *itM = dynamic_cast<ModelQStandardItem*>(pItem->child(i, column));

        if((itM != NULL)
                && (itM->getModel() == model))
        {
            return itM;
        }

        QStandardItem *it = recursiveGetStandardItemForModel(pItem->child(i, 0), model, column);

        if(it != NULL)
            return it;
    }

    return NULL;
}

QStandardItem *GItemDrawableModelManager::recursiveGetStandardItemForModel(QStandardItem *pItem, IItemModel *model, const DocumentInterface *doc) const
{
    int count = pItem->rowCount();
    int cCount = pItem->columnCount();

    for(int i=0; i<count; ++i)
    {
        for(int j=1; j<cCount; ++j)
        {
            ModelQStandardItem *itM = dynamic_cast<ModelQStandardItem*>(pItem->child(i, j));

            if((itM != NULL)
                    && (itM->getModel() == model)
                    && ((DocumentInterface*)itM->data().value<void*>()) == doc)
            {
                return itM;
            }
        }

        QStandardItem *it = recursiveGetStandardItemForModel(pItem->child(i, 0), model, doc);

        if(it != NULL)
            return it;
    }

    return NULL;
}

void GItemDrawableModelManager::recursiveSetCheckBoxEnable(QStandardItem *parent, bool enable)
{
    int n = parent->rowCount();

    for(int i=0; i<n; ++i)
    {
        if(m_docManagerView != NULL)
        {
            int size = m_docManagerView->nbDocumentView();

            for(int j=0; j<size; ++j)
            {
                QStandardItem *item = parent->child(i, j+1);

                if(item != NULL)
                    item->setEnabled(enable && m_docManagerView->getDocumentView(j)->canAddItemDrawable(NULL));
            }
        }

        recursiveSetCheckBoxEnable(parent->child(i, 0), enable);
    }
}

void GItemDrawableModelManager::documentAdded(DM_DocumentView *view)
{
    Q_UNUSED(view)

    setResult(_result);
}

void GItemDrawableModelManager::documentRemoved()
{
    setResult(_result);
}

void GItemDrawableModelManager::refreshCheckbox()
{
    QStandardItem *item = _viewModel.invisibleRootItem();

    disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
    recursiveSetCheckBoxEnable(item, (_result != NULL ? !_result->isBusy() : true));
    connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
}

void GItemDrawableModelManager::modelVisibilityChanged(bool visible)
{
    IModelSignalEmitter *emitter = (IModelSignalEmitter*)sender();

    if(emitter != NULL)
    {
        QStandardItem *item = recursiveGetStandardItemForModel(_viewModel.invisibleRootItem(), dynamic_cast<IItemModel*>(emitter->model()), 1);

        if(item != NULL)
        {
            disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
            item->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
            connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
        }
    }
}

void GItemDrawableModelManager::modelVisibilityInDocumentChanged(const DocumentInterface *doc, bool visible)
{
    IModelSignalEmitter *emitter = (IModelSignalEmitter*)sender();

    if((emitter != NULL)
            && (m_docManagerView != NULL))
    {
        QStandardItem *item = recursiveGetStandardItemForModel(_viewModel.invisibleRootItem(), dynamic_cast<IItemModel*>(emitter->model()), doc);

        if((item != NULL)
                && ((item->checkState() == Qt::Checked) != visible))
        {
            disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
            item->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
            connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
        }
    }
}

void GItemDrawableModelManager::itemChanged(QStandardItem *item)
{
    ModelQStandardItem *mItem = dynamic_cast<ModelQStandardItem*>(item);

    if((mItem != NULL)
            && (_result != NULL))
    {
        IItemModel *mo = dynamic_cast<IItemModel*>(mItem->getModel());
        DM_DocumentView *view = (DM_DocumentView*)mItem->data().value<void*>();

        if((mo != NULL)
                && (view != NULL))
        {
            bool visi = mo->isVisibleInDocument(view);

            if(mItem->isChecked()
                    && !visi)
            {
                if(_result != NULL)
                {
                    disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
                    mItem->setCheckState(GUI_MANAGER->asyncAddAllItemDrawableOfModelOnView(*_result, *mo, *view, NULL) ? Qt::Checked : Qt::Unchecked);
                    connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
                }
            }
            else if(visi)
            {
                disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
                mItem->setCheckState(GUI_MANAGER->asyncRemoveAllItemDrawableOfModelFromView(*mo, *view, NULL) ? Qt::Unchecked : Qt::Checked);
                connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
            }
        }
    }
}

void GItemDrawableModelManager::resultDestroyedDirect()
{
    _result = NULL;
}

void GItemDrawableModelManager::showContextMenu(const QPoint &point)
{
    QList<const IModel*> sModel = getSelectedIModels();

    if(!sModel.isEmpty())
    {
        const IItemModel *iModel = dynamic_cast<const IItemModel*>(sModel.first());

        if(iModel != NULL)
            _contextMenu->exec(ui->treeView->viewport()->mapToGlobal(point));
    }
}

void GItemDrawableModelManager::setUniqueColorForModelSelected()
{
    Result *res = result();

    if(res != NULL)
    {
        QList<const IModel*> sModel = getSelectedIModels();

        if(!sModel.isEmpty())
        {
            _colorPicker->showColorDialog();

            if(!_colorPicker->isDialogCanceled())
            {
                const IItemModel *iModel = dynamic_cast<const IItemModel*>(sModel.first());

                if((iModel != NULL)
                        && (res->recursiveBeginIterateItemDrawableWithModel(*iModel) > 0))
                {
                    ItemDrawable *item;

                    while((item = res->recursiveNextItemDrawable()) != NULL)
                        item->getItemDrawableSignalSlotManager()->setColor(_colorPicker->currentColor());

                    GUI_MANAGER->getDocumentManagerView()->getActiveDocumentView()->redrawGraphics();
                }
            }
        }
    }
}

void GItemDrawableModelManager::setAutomaticColorForModelSelected()
{
    Result *res = result();

    if(res != NULL)
    {
        QList<const IModel*> sModel = getSelectedIModels();

        if(!sModel.isEmpty())
        {
            const IItemModel *iModel = dynamic_cast<const IItemModel*>(sModel.first());

            if((iModel != NULL)
                    && (res->recursiveBeginIterateItemDrawableWithModel(*iModel) > 0))
            {
                ItemDrawable *item;

                while((item = res->recursiveNextItemDrawable()) != NULL)
                    item->getItemDrawableSignalSlotManager()->setColor(_colorOptions.getNextColor());

                GUI_MANAGER->getDocumentManagerView()->getActiveDocumentView()->redrawGraphics();
            }
        }
    }
}
