#include "ct_itemdrawablehierarchycollectionwidget.h"
#include "ui_ct_itemdrawablehierarchycollectionwidget.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

CT_ItemDrawableHierarchyCollectionWidget::CT_ItemDrawableHierarchyCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CT_ItemDrawableHierarchyCollectionWidget)
{
    ui->setupUi(this);

    m_dm = NULL;
    m_choice = false;

    m_modelCollection = NULL;
    m_selectionModelForCloud = new CT_ItemDrawableHierarchyCollectionSelectionModel("");

    ui->treeView->setModel(&m_model);
}

CT_ItemDrawableHierarchyCollectionWidget::~CT_ItemDrawableHierarchyCollectionWidget()
{
    delete ui;
    delete m_modelCollection;
    delete m_selectionModelForCloud;
}

void CT_ItemDrawableHierarchyCollectionWidget::setDocumentManager(const DocumentManagerInterface *dm)
{
    m_dm = (DocumentManagerInterface*)dm;
}

void CT_ItemDrawableHierarchyCollectionWidget::setModel(CT_ItemDrawableHierarchyCollectionModel *model)
{
    delete m_modelCollection;
    m_modelCollection = model;

    m_selectionModelForCloud->clearExcludeModel();

    disconnect(&m_model, NULL, this, NULL);
    m_model.clear();
    m_choice = false;

    constructHeader();

    if(m_modelCollection != NULL)
    {
        int index = 0;
        QListIterator<CT_ItemDrawableHierarchyCollectionSelectionModel*> it(m_modelCollection->models());

        m_choice = it.hasNext();

        while(it.hasNext())
        {
            CT_ItemDrawableHierarchyCollectionSelectionModel *sm = it.next();

            if(sm->mustExcludeModel(-1))
                m_selectionModelForCloud->addExcludeModel(index);

            QList<QStandardItem*> items = createItems(sm, index);

            if(!items.isEmpty())
                m_model.invisibleRootItem()->appendRow(items);

            ++index;
        }
    }

    if(m_dm != NULL)
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
                    QSharedPointer<CT_StandardColorCloudRegistered> colors = view->colorCloudOf(GraphicsViewInterface::CPointCloud);

                    if(!colors.isNull())
                    {
                        if(iDoc == NULL)
                        {
                            iDoc = new QStandardItem();
                            iDoc->setEditable(false);
                            iDoc->setText(doc->getTitle());
                        }

                        iDoc->appendRow(createItemsForCloud(colors));
                        m_choice = true;
                    }
                }
            }

            if(iDoc != NULL)
                root->appendRow(iDoc);

            m_model.invisibleRootItem()->appendRow(root);
        }
    }

    connect(&m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

    ui->treeView->expandAll();
}

bool CT_ItemDrawableHierarchyCollectionWidget::hasChoice() const
{
    return m_choice;
}

QSharedPointer<CT_StandardColorCloudRegistered> CT_ItemDrawableHierarchyCollectionWidget::colorCloudSelected() const
{
    QList<QStandardItem*> items = recursiveItemsSelected(m_model.invisibleRootItem());

    if((items.size() == 1)
            && (items.first()->data().toInt() == -1))
    {
        CT_StandardColorCloudRegistered *cloud = (CT_StandardColorCloudRegistered*)items.first()->data(Qt::UserRole+2).value<void*>();

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
                    QSharedPointer<CT_StandardColorCloudRegistered> colors = view->colorCloudOf(GraphicsViewInterface::CPointCloud);

                    if(colors.data() == cloud)
                        return colors;
                }
            }
        }
    }

    return QSharedPointer<CT_StandardColorCloudRegistered>(NULL);
}

QList<CT_AbstractSingularItemDrawable *> CT_ItemDrawableHierarchyCollectionWidget::itemDrawableSelected() const
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QList<QStandardItem*> items = recursiveItemsSelected(m_model.invisibleRootItem());

    if(!items.isEmpty()
            && (items.first()->data().toInt() != -1))
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

QList<QStandardItem *> CT_ItemDrawableHierarchyCollectionWidget::createItemsForCloud(QSharedPointer<CT_StandardColorCloudRegistered> colors) const
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
    item->setData(qVariantFromValue((void*)m_selectionModelForCloud), Qt::UserRole + 3);

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

void CT_ItemDrawableHierarchyCollectionWidget::itemChanged(QStandardItem *item)
{
    if(item->column() == 1
            && item->checkState() == Qt::Checked)
    {
        // we uncheck all other items that must be unchecked
        recursiveUncheckAllItemsExcept(m_model.invisibleRootItem(), item, (CT_ItemDrawableHierarchyCollectionSelectionModel*)item->data(Qt::UserRole + 3).value<void*>());
    }
}
