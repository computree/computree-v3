#include "gitemdrawablemodelmanager.h"
#include "ui_gitemdrawablemodelmanager.h"

#include <QDebug>

#include "view/DocumentView/gdocumentmanagerview.h"
#include "view/Tools/modelqstandarditem.h"

#include "dm_guimanager.h"
#include "dm_documentview.h"
#include "qtcolorpicker.h"

#include "ct_point.h"
#include "ct_result/tools/iterator/ct_resultiterator.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "tools/graphicsview/dm_colorlinearinterpolator.h"
#include "tools/graphicsview/dm_pointscolourist.h"
#include "tools/graphicsview/dm_pointsrecoverer.h"

GItemDrawableModelManager::GItemDrawableModelManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GItemDrawableModelManager)
{
    ui->setupUi(this);

    m_docManagerView = NULL;
    _result = NULL;

    m_contextMenu = new QMenu(this);

    m_contextMenuColorAdder = new DM_ContextMenuColouristAdder(*this, this);
    m_contextMenuColorAdder->setAutomaticColorProducer(&_colorOptions);

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
}

void GItemDrawableModelManager::setDocumentManagerView(const GDocumentManagerView *docManagerView)
{
    if(m_docManagerView != NULL)
    {
        disconnect(m_docManagerView, NULL, this, NULL);
    }

    m_docManagerView = (GDocumentManagerView*)docManagerView;
    m_contextMenuColorAdder->setDocumentManager(m_docManagerView);

    if(m_docManagerView != NULL)
    {
        connect(m_docManagerView, SIGNAL(documentAdded(DM_DocumentView*)), this, SLOT(documentAdded(DM_DocumentView*)));
        connect(m_docManagerView, SIGNAL(documentRemoved()), this, SLOT(documentRemoved()));
        connect(m_docManagerView, SIGNAL(documentHidden(DM_DocumentView*)), this, SLOT(refreshCheckbox()));
        connect(m_docManagerView, SIGNAL(documentShowned(DM_DocumentView*)), this, SLOT(refreshCheckbox()));
    }
}

void GItemDrawableModelManager::setResult(const CT_AbstractResult *res)
{
    if(_result != NULL)
        disconnect(_result, NULL, this, NULL);

    _result = (CT_AbstractResult*)res;

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

DM_ContextMenuColouristAdder *GItemDrawableModelManager::contextMenuColouristAdder() const
{
    return m_contextMenuColorAdder;
}

CT_AbstractResult* GItemDrawableModelManager::result() const
{
    return _result;
}

QList<const CT_AbstractModel*> GItemDrawableModelManager::getSelectedIModels() const
{
    QList<const CT_AbstractModel*> retList;

    QModelIndexList list = ui->treeView->selectionModel()->selectedRows(1);

    QListIterator<QModelIndex> it(list);

    while(it.hasNext())
    {
        ModelQStandardItem *mItem = dynamic_cast<ModelQStandardItem*>(_viewModel.itemFromIndex(it.next()));

        if((mItem != NULL)
                && (mItem->getModel() != NULL))
            retList.append(mItem->getModel());
    }

    return retList;
}

void GItemDrawableModelManager::reconstruct()
{
    constructModel();
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
            && (_result->model() != NULL))
    {
        _viewModel.invisibleRootItem()->appendRow(recursiveCreateItemsForModel(_result->model()));

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

void GItemDrawableModelManager::reConstructContextMenu()
{
    m_contextMenu->clear();
    m_contextMenuColorAdder->initContextMenu(m_contextMenu);
}

QList<QStandardItem*> GItemDrawableModelManager::recursiveCreateItemsForModel(const CT_OutAbstractModel *model)
{
    QList<QStandardItem*> retList = createItemsForModel(model);

    if(retList.isEmpty())
        return retList;

    QStandardItem *rootItem = retList.first();

    QList<CT_AbstractModel*> items = model->childrens();
    QListIterator<CT_AbstractModel*> it(items);

    while(it.hasNext())
    {
        QList<QStandardItem *> lNew = recursiveCreateItemsForModel((CT_OutAbstractModel*)it.next());

        if(!lNew.isEmpty())
            rootItem->appendRow(lNew);
    }

    return retList;
}

QList<QStandardItem *> GItemDrawableModelManager::createItemsForModel(const CT_OutAbstractModel *model)
{
    if(dynamic_cast<const CT_OutAbstractItemModel*>(model) != NULL)
        return createItemsForItemModel((const CT_OutAbstractItemModel*)model);
    else if(dynamic_cast<const CT_OutAbstractResultModel*>(model) != NULL)
        return createItemsForResultModel((const CT_OutAbstractResultModel*)model);

    return QList<QStandardItem*>();
}

QList<QStandardItem*> GItemDrawableModelManager::createItemsForResultModel(const CT_OutAbstractResultModel *model)
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

QList<QStandardItem *> GItemDrawableModelManager::createItemsForItemModel(const CT_OutAbstractItemModel *model)
{
    QList<QStandardItem*> retList;

    QStandardItem *item = new QStandardItem(model->displayableName());
    item->setEditable(false);

    if (model->itemDrawable() != NULL)
    {
        item->setToolTip(model->itemDrawable()->name());
    }

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

    connect(model, SIGNAL(visibilityInDocumentChanged(const DocumentInterface*,bool)), this, SLOT(modelVisibilityInDocumentChanged(const DocumentInterface*,bool)), Qt::QueuedConnection);

    return retList;
}

QStandardItem* GItemDrawableModelManager::recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, int column) const
{
    int count = pItem->rowCount();

    for(int i=0; i<count; ++i)
    {
        ModelQStandardItem *itM = dynamic_cast<ModelQStandardItem*>(pItem->child(i, column));

        if((itM != NULL)
                && (itM->getModel() == model))
            return itM;

        QStandardItem *it = recursiveGetStandardItemForModel(pItem->child(i, 0), model, column);

        if(it != NULL)
            return it;
    }

    return NULL;
}

QStandardItem *GItemDrawableModelManager::recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, const DocumentInterface *doc) const
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
                return itM;
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

QList<CT_AbstractItemDrawable *> GItemDrawableModelManager::getItemDrawableToColorize() const
{
    QList<CT_AbstractItemDrawable *> items;

    CT_AbstractResult *res = result();

    if(res != NULL)
    {
        QList<const CT_AbstractModel*> sModel = getSelectedIModels();

        if(!sModel.isEmpty())
        {
            const CT_OutAbstractItemModel *iModel = dynamic_cast<const CT_OutAbstractItemModel*>(sModel.first());

            if(iModel != NULL)
            {
                CT_ResultIterator it((CT_ResultGroup*)res, iModel);

                while(it.hasNext())
                    items.append((CT_AbstractItemDrawable*)it.next());
            }
        }
    }

    return items;
}

QList<CT_AbstractModel *> GItemDrawableModelManager::getSelectedModelsToUseInColorizerMenu() const
{
    QList<const CT_AbstractModel*> list = getSelectedIModels();
    QList<CT_AbstractModel*> listRet;

    foreach (const CT_AbstractModel* m, list) {
        listRet.append((CT_AbstractModel*)m);
    }

    return listRet;
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
    QStandardItem *item = recursiveGetStandardItemForModel(_viewModel.invisibleRootItem(), dynamic_cast<CT_OutAbstractModel*>(sender()), 1);

    if(item != NULL)
    {
        disconnect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
        item->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
        connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);
    }
}

void GItemDrawableModelManager::modelVisibilityInDocumentChanged(const DocumentInterface *doc, bool visible)
{
    if(m_docManagerView != NULL)
    {
        QStandardItem *item = recursiveGetStandardItemForModel(_viewModel.invisibleRootItem(), dynamic_cast<CT_OutAbstractModel*>(sender()), doc);

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
        CT_OutAbstractItemModel *mo = (dynamic_cast<CT_OutAbstractItemModel*>(mItem->getModel()));
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
    QList<const CT_AbstractModel*> sModel = getSelectedIModels();

    if(!sModel.isEmpty())
    {
        const CT_OutAbstractModel *iModel = dynamic_cast<const CT_OutAbstractModel*>(sModel.first());

        if(iModel != NULL)
        {
            reConstructContextMenu();
            m_contextMenu->exec(ui->treeView->viewport()->mapToGlobal(point));
        }
    }
}
