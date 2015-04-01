#include "ct_itemdrawablehierarchycollectionwidget.h"
#include "ui_ct_itemdrawablehierarchycollectionwidget.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"

CT_ItemDrawableHierarchyCollectionWidget::CT_ItemDrawableHierarchyCollectionWidget(CloudType cloudType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CT_ItemDrawableHierarchyCollectionWidget)
{
    ui->setupUi(this);

    setCanSelectColors(false);
    setCanSelectNormals(false);

    m_cloudType = cloudType;

    if(cloudType == POINTS) {
        m_colorCloudType = GraphicsViewInterface::CPointCloud;
        m_normalCloudType = GraphicsViewInterface::NPointCloud;
    } else if(cloudType == FACES) {
        m_colorCloudType = GraphicsViewInterface::CFaceCloud;
        m_normalCloudType = GraphicsViewInterface::NFaceCloud;
    } else {
        m_colorCloudType = GraphicsViewInterface::CEdgeCloud;
        m_normalCloudType = GraphicsViewInterface::NEdgeCloud;
    }

    m_dm = NULL;
    m_nChoice = 0;

    m_modelCollection = NULL;

    m_selectionModelForPointCloud = new CT_ItemDrawableHierarchyCollectionSelectionModel("");
    m_selectionModelForNormalCloud = new CT_ItemDrawableHierarchyCollectionSelectionModel("");

    ui->treeView->setModel(&m_model);
}

CT_ItemDrawableHierarchyCollectionWidget::~CT_ItemDrawableHierarchyCollectionWidget()
{
    delete ui;
    delete m_modelCollection;
    delete m_selectionModelForPointCloud;
    delete m_selectionModelForNormalCloud;
}

void CT_ItemDrawableHierarchyCollectionWidget::setCanSelectColors(bool enable)
{
    m_canSelectColors = enable;
}

void CT_ItemDrawableHierarchyCollectionWidget::setCanSelectNormals(bool enable)
{
    m_canSelectNormals = enable;
}

void CT_ItemDrawableHierarchyCollectionWidget::setDocumentManager(const DocumentManagerInterface *dm)
{
    m_dm = (DocumentManagerInterface*)dm;
}

void CT_ItemDrawableHierarchyCollectionWidget::setModel(CT_ItemDrawableHierarchyCollectionModel *model)
{
    delete m_modelCollection;
    m_modelCollection = model;

    m_selectionModelForPointCloud->clearExcludeModel();
    m_selectionModelForPointCloud->addExcludeModel(-1);

    m_selectionModelForNormalCloud->clearExcludeModel();
    m_selectionModelForNormalCloud->addExcludeModel(-2);

    disconnect(&m_model, NULL, this, NULL);
    m_model.clear();
    m_nChoice = 0;

    constructHeader();

    if(m_modelCollection != NULL)
    {
        int index = 0;
        QListIterator<CT_ItemDrawableHierarchyCollectionSelectionModel*> it(m_modelCollection->models());

        while(it.hasNext())
        {
            CT_ItemDrawableHierarchyCollectionSelectionModel *sm = it.next();

            if(sm->mustExcludeModel(-1))
                m_selectionModelForPointCloud->addExcludeModel(index);

            if(sm->mustExcludeModel(-2))
                m_selectionModelForNormalCloud->addExcludeModel(index);

            QList<QStandardItem*> items = createItems(sm, index);

            if(!items.isEmpty())
            {
                m_model.invisibleRootItem()->appendRow(items);
                ++m_nChoice;
            }

            ++index;
        }
    }

    if((m_dm != NULL) && (m_cloudType != NONE) && m_canSelectColors)
    {
        QStandardItem *root = new QStandardItem();
        root->setEditable(false);
        root->setText(tr("Documents"));

        QList<DocumentInterface*> docs = m_dm->documents();
        QListIterator<DocumentInterface*> it(docs);

        while(it.hasNext())
        {
            QStandardItem *iDoc = NULL;

            DocumentInterface *doc = it.next();
            QList<InDocumentViewInterface*> views = doc->views();

            QListIterator<InDocumentViewInterface*> itV(views);

            while(itV.hasNext())
            {
                GraphicsViewInterface *view = dynamic_cast<GraphicsViewInterface*>(itV.next());

                if(view != NULL)
                {
                    if(m_canSelectColors)
                    {
                        QSharedPointer<CT_StandardColorCloudRegistered> colors = view->colorCloudOf(m_colorCloudType);

                        if(!colors.isNull())
                        {
                            if(iDoc == NULL)
                            {
                                iDoc = new QStandardItem();
                                iDoc->setEditable(false);
                                iDoc->setText(doc->getTitle());
                            }

                            iDoc->appendRow(createItemsForColorCloud(colors));
                            ++m_nChoice;
                        }
                    }
                }
            }

            if(iDoc != NULL)
                root->appendRow(iDoc);
        }

        if(root->hasChildren())
            m_model.invisibleRootItem()->appendRow(root);
        else
            delete root;
    }

    connect(&m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

    ui->treeView->expandAll();
}

bool CT_ItemDrawableHierarchyCollectionWidget::hasChoice() const
{
    return (m_nChoice > 0);
}

bool CT_ItemDrawableHierarchyCollectionWidget::canChoiceBeSelectedByDefault() const
{
    return (m_nChoice <= 1);
}

bool CT_ItemDrawableHierarchyCollectionWidget::selectChoiceByDefault()
{
    QList<QStandardItem*> checkable = checkableItems(m_model.invisibleRootItem());

    if(checkable.isEmpty())
        return true;

    if(checkable.size() > 1)
        return false;

    checkable.first()->setCheckState(Qt::Checked);

    return true;
}

QSharedPointer<CT_StandardColorCloudRegistered> CT_ItemDrawableHierarchyCollectionWidget::colorCloudSelected() const
{
    QList<QStandardItem*> items = recursiveItemsSelected(m_model.invisibleRootItem());

    QListIterator<QStandardItem*> it(items);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        if(item->data().toInt() == -1)
        {
            CT_StandardColorCloudRegistered *cloud = (CT_StandardColorCloudRegistered*)item->data(Qt::UserRole+2).value<void*>();

            QList<DocumentInterface*> docs = m_dm->documents();
            QListIterator<DocumentInterface*> it(docs);

            while(it.hasNext())
            {
                DocumentInterface *doc = it.next();
                QList<InDocumentViewInterface*> views = doc->views();

                QListIterator<InDocumentViewInterface*> itV(views);

                while(itV.hasNext())
                {
                    GraphicsViewInterface *view = dynamic_cast<GraphicsViewInterface*>(itV.next());

                    if(view != NULL)
                    {
                        QSharedPointer<CT_StandardColorCloudRegistered> colors = view->colorCloudOf(m_colorCloudType);

                        if(colors.data() == cloud)
                            return colors;
                    }
                }
            }
        }
    }

    return QSharedPointer<CT_StandardColorCloudRegistered>(NULL);
}

QSharedPointer<CT_StandardNormalCloudRegistered> CT_ItemDrawableHierarchyCollectionWidget::normalCloudSelected() const
{
    QList<QStandardItem*> items = recursiveItemsSelected(m_model.invisibleRootItem());

    QListIterator<QStandardItem*> it(items);

    while(it.hasNext())
    {
        QStandardItem *item = it.next();

        if(item->data().toInt() == -2)
        {
            CT_StandardNormalCloudRegistered *cloud = (CT_StandardNormalCloudRegistered*)item->data(Qt::UserRole+2).value<void*>();

            QList<DocumentInterface*> docs = m_dm->documents();
            QListIterator<DocumentInterface*> it(docs);

            while(it.hasNext())
            {
                DocumentInterface *doc = it.next();
                QList<InDocumentViewInterface*> views = doc->views();

                QListIterator<InDocumentViewInterface*> itV(views);

                while(itV.hasNext())
                {
                    GraphicsViewInterface *view = dynamic_cast<GraphicsViewInterface*>(itV.next());

                    if(view != NULL)
                    {
                        QSharedPointer<CT_StandardNormalCloudRegistered> normals = view->normalCloudOf(m_normalCloudType);

                        if(normals.data() == cloud)
                            return normals;
                    }
                }
            }
        }
    }

    return QSharedPointer<CT_StandardNormalCloudRegistered>(NULL);
}

QList<CT_AbstractSingularItemDrawable *> CT_ItemDrawableHierarchyCollectionWidget::itemDrawableSelected() const
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QList<QStandardItem*> items = recursiveItemsSelected(m_model.invisibleRootItem());

    if(!items.isEmpty()
            && (items.first()->data().toInt() >= 0))
    {
        QListIterator<QStandardItem*> it(items);

        while(it.hasNext())
            l.append((CT_AbstractSingularItemDrawable*)it.next()->data(Qt::UserRole+2).value<void*>());
    }

    return l;
}

void CT_ItemDrawableHierarchyCollectionWidget::constructHeader()
{
    QStringList headers;
    headers << tr("Nom");
    headers << tr("Utiliser");

    m_model.setHorizontalHeaderLabels(headers);

    #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
        ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    #else
        ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::createItems(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm, const int &index) const
{
    QList<QStandardItem *> l;

    QStandardItem *root = new QStandardItem();
    root->setEditable(false);
    root->setText(sm->name());

    QListIterator<CT_ItemDrawableCollectionHierarchyStep> it(sm->collection());

    while(it.hasNext())
    {
        const CT_ItemDrawableCollectionHierarchyStep &pas = it.next();

        if(pas.step != NULL)
        {
            QStandardItem *iStep = new QStandardItem();
            iStep->setEditable(false);
            iStep->setText(pas.step->getStepName());

            QListIterator< CT_ItemDrawableCollectionHierarchyResult > itR(pas.results);

            while(itR.hasNext())
            {
                const CT_ItemDrawableCollectionHierarchyResult &par = itR.next();

                if(par.result != NULL)
                {
                    QStandardItem *iResult = new QStandardItem();
                    iResult->setEditable(false);
                    iResult->setText(par.result->model()->displayableName());

                    QListIterator<CT_AbstractSingularItemDrawable*> itA(par.collection);

                    while(itA.hasNext())
                    {
                        CT_AbstractSingularItemDrawable *pa = itA.next();

                        iResult->appendRow(createItemsForItemDrawable(sm, pa, index));
                    }

                    if(iResult->rowCount() > 0)
                        iStep->appendRow(iResult);
                    else
                        delete iResult;
                }
            }

            if(iStep->rowCount() > 0)
                root->appendRow(iStep);
            else
                delete iStep;
        }
    }

    l << root;

    return l;
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::createItemsForItemDrawable(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm,
                                                                                            const CT_AbstractSingularItemDrawable *itemd,
                                                                                            const int &index) const
{
    QList<QStandardItem *> l;

    QStandardItem *item = new QStandardItem();
    item->setEditable(false);
    item->setText(itemd->displayableName());
    l.append(item);

    item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData(index, Qt::UserRole + 1);
    item->setData(qVariantFromValue((void*)itemd), Qt::UserRole + 2);
    item->setData(qVariantFromValue((void*)sm), Qt::UserRole + 3);

    l.append(item);

    return l;
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::createItemsForColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> colors) const
{
    QList<QStandardItem *> l;

    QStandardItem *item = new QStandardItem();
    item->setEditable(false);
    item->setText(tr("Couleur"));
    l.append(item);

    item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData(-1, Qt::UserRole + 1);
    item->setData(qVariantFromValue((void*)colors.data()), Qt::UserRole + 2);
    item->setData(qVariantFromValue((void*)m_selectionModelForPointCloud), Qt::UserRole + 3);

    l.append(item);

    return l;
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::createItemsForNormalCloud(QSharedPointer<CT_StandardNormalCloudRegistered> normals) const
{
    QList<QStandardItem *> l;

    QStandardItem *item = new QStandardItem();
    item->setEditable(false);
    item->setText(tr("Normale"));
    l.append(item);

    item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData(-2, Qt::UserRole + 1);
    item->setData(qVariantFromValue((void*)normals.data()), Qt::UserRole + 2);
    item->setData(qVariantFromValue((void*)m_selectionModelForNormalCloud), Qt::UserRole + 3);

    l.append(item);

    return l;
}

void CT_ItemDrawableHierarchyCollectionWidget::recursiveUncheckAllItemsExcept(QStandardItem *root,
                                                                              const QStandardItem *item,
                                                                              const CT_ItemDrawableHierarchyCollectionSelectionModel *sm)
{
    int s = root->rowCount();

    for(int i=0; i<s; ++i)
    {
        QStandardItem *checkableItem = root->child(i, 1);

        if(checkableItem != NULL)
        {
            recursiveUncheckAllItemsExcept(checkableItem, item, sm);

            if((checkableItem != item)
                    && sm->mustExcludeModel(checkableItem->data().toInt()))
            {
                checkableItem->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            checkableItem = root->child(i, 0);

            if(checkableItem != NULL)
                recursiveUncheckAllItemsExcept(checkableItem, item, sm);
        }
    }
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::recursiveItemsSelected(QStandardItem *root) const
{
    QList<QStandardItem *> l;

    int s = root->rowCount();

    for(int i=0; i<s; ++i)
    {
        QStandardItem *checkableItem = root->child(i, 1);

        if(checkableItem != NULL)
        {
            if(checkableItem->rowCount() > 0)
                l.append(recursiveItemsSelected(checkableItem));

            if(checkableItem->checkState() == Qt::Checked)
                l.append(checkableItem);
        }
        else
        {
            checkableItem = root->child(i, 0);

            if((checkableItem != NULL)
                    && (checkableItem->rowCount() > 0))
            {
                l.append(recursiveItemsSelected(checkableItem));
            }
        }
    }

    return l;
}

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::checkableItems(QStandardItem *root) const
{
    QList<QStandardItem *> l;

    int s = root->rowCount();

    for(int i=0; i<s; ++i)
    {
        QStandardItem *checkableItem = root->child(i, 1);

        if(checkableItem != NULL)
        {
            if(checkableItem->isCheckable())
                l.append(checkableItem);

            l.append(checkableItems(checkableItem));
        }
        else
        {
            checkableItem = root->child(i, 0);

            if(checkableItem != NULL)
                l.append(checkableItems(checkableItem));
        }
    }

    return l;
}

void CT_ItemDrawableHierarchyCollectionWidget::itemChanged(QStandardItem *item)
{
    if(item->column() == 1
            && item->checkState() == Qt::Checked)
    {
        // we uncheck all other items that must be unchecked
        recursiveUncheckAllItemsExcept(m_model.invisibleRootItem(), item, (CT_ItemDrawableHierarchyCollectionSelectionModel*)item->data(Qt::UserRole + 3).value<void*>());
    }
}
