#include "gtreeview.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCompleter>
#include <QWhatsThis>
#include <QLineEdit>
#include <QAction>
#include <QEvent>
#include <QHeaderView>

#include "qtcolorpicker/qtcolorpicker.h"

#include "dm_guimanager.h"

#include "tools/treeview/dm_sortfiltermathproxymodel.h"

#include "view/StepResultTreeView/myqstandarditem.h"
#include "view/ItemDrawableView/gitemdrawablemanageroptionscolor.h"

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

int GTreeView::COLUMN_FIRST_DATA_VALUE = 1;

GTreeView::GTreeView(QWidget *parent) : QWidget(parent), GItemModelView()
{
    m_actionsHandler = new DM_ActionsHandlerForTreeView(*GUI_MANAGER->getActionsManager());
}

GTreeView::~GTreeView()
{
    delete m_actionsHandler;
    delete m_treeView->model();
    delete m_model;
}

void GTreeView::init()
{
    m_typeBuilder.setGetter(&CT_AbstractSingularItemDrawable::model);

    m_contextMenu = new QMenu(this);

    m_model = new QStandardItemModel();
    DM_SortFilterMathProxyModel *filterModel = new DM_SortFilterMathProxyModel();
    filterModel->setSourceModel(m_model);
    filterModel->setVariableInMathExpression("(val)");
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_treeViewController.setStandardItemBuilder(this);
    m_treeViewController.setTreeViewManager(this);
    m_treeViewController.setModel(m_model);

    // OPTIONS

    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    // a button to sync with other documents
    QPushButton *bSyncWith = new QPushButton(tr("Sync with.."), this);
    hLayout->addWidget(bSyncWith);

    QString wt = tr("<html>"
                    "<p><font size=\"4\"><b>Filtrer une colonne</b></font></p>"
                    "<p>Vous pouvez filtrer une colonne en écrivant son nom puis la valeur à rechercher.</p>"
                    "<p><i>MaColonne : mot</i></p>"
                    "<p></p>"
                    "<p>Vous pouvez aussi filtrer une colonne contenant des nombres en utilisant des fonctions mathématiques. "
                    "Pour celà il vous faut utiliser la syntaxe (val) qui sera remplacée par la valeur de la cellule de la colonne.</p>"
                    "<p><i>MaColonne : (val) &gt; 0 and (val) &lt; 500</i></p>"
                    "</html>");

    m_lineFilter = new QLineEdit(this);
    m_lineFilter->setPlaceholderText(tr("Filtrer la colonne..."));
    m_lineFilter->setWhatsThis(wt);

    QAction *whatThis = QWhatsThis::createAction(NULL);
    whatThis->setText(wt);

    #if QT_VERSION < QT_VERSION_CHECK(5,2,0)
    m_lineFilter->addAction(whatThis);
    #else
    m_lineFilter->addAction(whatThis,QLineEdit::LeadingPosition);
    #endif

    hLayout->addWidget(m_lineFilter);

    QPushButton *buttonValidFilter = new QPushButton(tr("Valider"), this);
    hLayout->addWidget(buttonValidFilter);

    QPushButton *buttonShowColors = new QPushButton(this);
    buttonShowColors->setIcon(QIcon(":/Icones/Icones/triple-color-icon.png"));
    hLayout->addWidget(buttonShowColors);

    vLayout->addLayout(hLayout);

    m_treeView = new QTreeView(this);
    m_treeView->setModel(filterModel);
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeView->setExpandsOnDoubleClick(true);
    m_treeView->setSortingEnabled(true);
    m_treeView->setAcceptDrops(true);
    vLayout->addWidget(m_treeView);

    delete layout();
    setLayout(vLayout);

    // QMENU
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->header()->setContextMenuPolicy(Qt::CustomContextMenu);

    // EVENT
    m_treeView->installEventFilter(this);
    m_treeView->viewport()->installEventFilter(this);
    m_treeView->viewport()->setMouseTracking(true);

    // SIGNALS-SLOTS CONNECTION
    connect(document(), SIGNAL(itemDrawableAdded(CT_AbstractItemDrawable&)), this, SLOT(slotAddItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);
    connect(document(), SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), this, SLOT(slotRemoveItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);

    qRegisterMetaType<DM_ItemDrawableType<CT_OutAbstractItemModel*, CT_AbstractSingularItemDrawable> >("DM_ItemDrawableType<CT_OutAbstractItemModel*, CT_AbstractSingularItemDrawable> ");
    connect(&m_typeBuilder, SIGNAL(removed()), this, SLOT(slotItemTypeRemoved()), Qt::QueuedConnection);
    connect(&m_typeBuilder, SIGNAL(added()), this, SLOT(slotNewItemTypeDetected()), Qt::QueuedConnection);

    connect(bSyncWith, SIGNAL(clicked()), this, SLOT(slotButtonSyncWithClicked()));

    connect(buttonShowColors, SIGNAL(clicked()), this, SLOT(slotShowColorOptions()));

    connect(m_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)), Qt::QueuedConnection);
    connect(m_treeView->header(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowHeaderContextMenu(QPoint)), Qt::QueuedConnection);

    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(slotActivated(QModelIndex)), Qt::DirectConnection);
    connect(m_treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClicked(QModelIndex)), Qt::DirectConnection);
    connect(m_treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)), Qt::DirectConnection);
    connect(m_treeView, SIGNAL(expanded(QModelIndex)), this, SLOT(slotExpanded(QModelIndex)), Qt::DirectConnection);
    connect(m_treeView, SIGNAL(collapsed(QModelIndex)), this, SLOT(slotCollapsed(QModelIndex)), Qt::DirectConnection);

    connect(buttonValidFilter, SIGNAL(clicked()), this, SLOT(slotSetTextFilter()));

    m_typeBuilder.setNLevel(m_options.nLevelFiltering());

    refreshHeaders();
}

QWidget* GTreeView::getViewWidget() const
{
    return const_cast<GTreeView*>(this);
}

void GTreeView::beginAddMultipleItemDrawable()
{
    m_treeViewController.beginAddMultipleItemDrawable();
}

void GTreeView::slotAddItemDrawable(CT_AbstractItemDrawable &item)
{
    m_typeBuilder.addItemDrawable(item);

    m_treeViewController.addItemDrawable(item);
}

void GTreeView::endAddMultipleItemDrawable()
{
    m_treeViewController.endAddMultipleItemDrawable();
}

void GTreeView::beginRemoveMultipleItemDrawable()
{
    m_treeViewController.beginRemoveMultipleItemDrawable();
}

void GTreeView::slotRemoveItemDrawable(CT_AbstractItemDrawable &item)
{
    m_typeBuilder.removeItemDrawable(item);

    m_treeViewController.removeItemDrawable(item);
}

void GTreeView::endRemoveMultipleItemDrawable()
{
    m_treeViewController.endRemoveMultipleItemDrawable();
}

void GTreeView::slotButtonSyncWithClicked()
{
    emit syncWith(this);
}

void GTreeView::slotActivated(const QModelIndex &index)
{
    actionsHandlerTreeView()->indexActivatedEvent(index);
}

void GTreeView::slotClicked(const QModelIndex &index)
{
    actionsHandlerTreeView()->indexClickedEvent(index);
}

void GTreeView::slotDoubleClicked(const QModelIndex &index)
{
    actionsHandlerTreeView()->indexDoubleClickedEvent(index);
}

void GTreeView::slotExpanded(const QModelIndex &index)
{
    m_expandedItems.append(itemDrawableFromItem(itemFromIndex(index)));
    actionsHandlerTreeView()->indexExpandedEvent(index);
}

void GTreeView::slotCollapsed(const QModelIndex &index)
{
    m_expandedItems.removeOne(itemDrawableFromItem(itemFromIndex(index)));
    actionsHandlerTreeView()->indexCollapsedEvent(index);
}

void GTreeView::slotSetTextFilter()
{
    QString textToUse = m_lineFilter->text();
    int columnToUse = -1;
    bool continueLoop = true;

    QListIterator<CT_OutAbstractModel*> it(m_dataReferencesToUse);

    while(it.hasNext()
            && continueLoop)
    {
        ++columnToUse;
        QString str = it.next()->displayableName() + " : ";

        if(m_lineFilter->text().startsWith(str))
        {
            textToUse.remove(str);
            continueLoop = false;
        }
    }

    if(!continueLoop)
    {
        columnToUse += COLUMN_FIRST_DATA_VALUE;

        ((QSortFilterProxyModel*)m_treeView->model())->setFilterKeyColumn(columnToUse);

        if(!textToUse.contains(((DM_SortFilterMathProxyModel*)m_treeView->model())->variableInMathExpression()))
        {
            ((QSortFilterProxyModel*)m_treeView->model())->setFilterFixedString(textToUse);

            setValidColorForLineFilter(true);
        }
        else
        {
            setValidColorForLineFilter(((DM_SortFilterMathProxyModel*)m_treeView->model())->setMathExpression(textToUse));
        }

        return;
    }

    ((QSortFilterProxyModel*)m_treeView->model())->setFilterKeyColumn(-1);
    ((DM_SortFilterMathProxyModel*)m_treeView->model())->setMathExpression("");

    setValidColorForLineFilter(m_lineFilter->text().isEmpty());
}

void GTreeView::slotShowContextMenu(const QPoint &p)
{
    if(!actionsHandlerTreeView()->showContextMenu(p))
    {
        initContextMenu();
        m_contextMenu->exec(treeViewport()->mapToGlobal(p));
    }
}

void GTreeView::slotShowHeaderContextMenu(const QPoint &p)
{
    actionsHandlerTreeView()->showContextMenuOnHorizontalHeader(p);
}

void GTreeView::slotResetColorLineFilter()
{
    m_lineFilter->setStyleSheet("QLineEdit{background: white;}");
}

void GTreeView::slotItemDataChanged(QStandardItem *item)
{
    MyQStandardItem *myItem = (MyQStandardItem*)item;
    CT_AbstractItemDrawable *itemDrawable = myItem->itemDrawable();

    if(itemDrawable != NULL)
    {
        if(myItem->columnType() == MyQStandardItem::ItemDrawableSelection)
        {
            bool selected = (item->checkState() == Qt::Checked);

            if(itemDrawable->isSelected() != selected)
            {
                itemDrawable->setSelected(selected);
                GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
            }
        }
    }
}

void GTreeView::slotShowColorOptions()
{
    GItemDrawableManagerOptionsColor dialog(this);

    dialog.setOptions(m_options);

    if(dialog.exec())
        m_options.setColorList(dialog.getColorList());
}

void GTreeView::slotSelect()
{
    QList<CT_AbstractItemDrawable*> sel = itemDrawableFromRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
        it.next()->setSelected(true);

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GTreeView::slotDeSelect()
{
    QList<CT_AbstractItemDrawable*> sel = itemDrawableFromRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
        it.next()->setSelected(false);

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GTreeView::slotInverseSelection()
{
    QList<CT_AbstractItemDrawable*> sel = itemDrawableFromRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *ii = it.next();
        ii->setSelected(!ii->isSelected());
    }

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GTreeView::slotColorAuto()
{
    QList<CT_AbstractItemDrawable *> l = itemDrawableFromRowSelected();

    if(!l.isEmpty())
    {
        QListIterator<CT_AbstractItemDrawable*> it(l);

        while(it.hasNext())
            it.next()->setColor(m_options.getNextColor());

        construct();
    }
}

void GTreeView::slotColorSolid()
{
    QtColorPicker colorPicker(0, -1, true, false);
    colorPicker.showColorDialog();

    if(!colorPicker.isDialogCanceled())
    {
        QColor color = colorPicker.currentColor();

        QList<CT_AbstractItemDrawable *> l = itemDrawableFromRowSelected();
        QListIterator<CT_AbstractItemDrawable*> it(l);

        while(it.hasNext())
            it.next()->setColor(color);

        construct();
    }
}

void GTreeView::slotAddSelectedToDocument()
{
    QAction *act = (QAction*)sender();
    int number = act->data().toInt();

    QList<DocumentInterface*> documents = GUI_MANAGER->getDocumentManagerView()->documents();
    QListIterator<DocumentInterface*> it(documents);

    while(it.hasNext())
    {
        DM_DocumentView *doc = (DM_DocumentView*)it.next();

        if(doc->getNumber() == number)
        {
            QList<CT_AbstractItemDrawable *> items = itemDrawableFromRowSelected();

            GUI_MANAGER->asyncAddAllItemDrawableOfListOnView(items, doc, NULL);

            return;
        }
    }
}

DM_ActionsHandler* GTreeView::actionsHandler() const
{
    return m_actionsHandler;
}

QStandardItem* GTreeView::itemFromIndex(const QModelIndex &proxyIndex) const
{
    return m_model->itemFromIndex(((DM_SortFilterMathProxyModel*)m_treeView->model())->mapToSource(proxyIndex));
}

CT_AbstractItemDrawable* GTreeView::itemDrawableFromItem(const QStandardItem *item) const
{
    return (CT_AbstractItemDrawable*)item->data().value<void*>();
}

QModelIndex GTreeView::indexAt(const QPoint &point) const
{
    return m_treeView->indexAt(point);
}

QWidget* GTreeView::treeViewport() const
{
    return m_treeView->viewport();
}

void GTreeView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    m_treeView->setSelectionMode(mode);
}

QAbstractItemView::SelectionMode GTreeView::selectionMode() const
{
    return m_treeView->selectionMode();
}

QModelIndexList GTreeView::selectedIndexes() const
{
    return m_treeView->selectionModel()->selectedIndexes();
}

QItemSelectionModel* GTreeView::selectionModel() const
{
    return m_treeView->selectionModel();
}

void GTreeView::refreshAll()
{
    construct();
}

void GTreeView::refreshItems(const QList<QStandardItem*> &items)
{
    QList<QPair<QStandardItem *, CT_AbstractItemDrawable *> > list;

    QListIterator<QStandardItem*> it(items);

    while(it.hasNext())
    {
        QStandardItem *i = it.next();
        list << qMakePair(i, itemDrawableFromItem(i));
    }

    m_treeViewController.refresh(list);
}

void GTreeView::refreshItems(const QList<CT_AbstractItemDrawable*> &items)
{
    QList<QPair<QStandardItem *, CT_AbstractItemDrawable *> > list;

    QListIterator<CT_AbstractItemDrawable*> it(items);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *i = it.next();
        list << qMakePair(itemFromItemDrawable(i), i);
    }

    m_treeViewController.refresh(list);
}

QList<QStandardItem *> GTreeView::createItems(const CT_AbstractItemDrawable &item, const int &level) const
{
    QList<QStandardItem *> l;

    // if we are synchronized with other documents
    if((level == 0)
            && (syncGroup() != NULL)
            && !syncGroup()->documentViews().isEmpty())
    {
        if(!item.isSelected())
            return l;

        QList<DocumentInterface*> docs = item.document();
        QListIterator<DM_DocumentView*> it(syncGroup()->documentViews());

        bool continueLoop = true;

        while(it.hasNext()
                && continueLoop)
            continueLoop = !docs.contains(it.next());

        if(continueLoop)
            return l;
    }

    // selectionné
    MyQStandardItem *itemDisplay = new MyQStandardItem((CT_AbstractItemDrawable*)&item, MyQStandardItem::ItemDrawableSelection, QString(""));
    itemDisplay->setCheckable(true);
    itemDisplay->setCheckState(item.isSelected() ? Qt::Checked : Qt::Unchecked);
    itemDisplay->setData(qVariantFromValue((void*)&item), Qt::UserRole + 1);
    QObject::connect(&item, SIGNAL(selectChange(bool)), itemDisplay, SLOT(setBoolData(bool)), Qt::DirectConnection);
    QObject::connect(itemDisplay, SIGNAL(dataChanged(QStandardItem*)), this, SLOT(slotItemDataChanged(QStandardItem*)), Qt::QueuedConnection);
    l << itemDisplay;

    int s = m_dataReferencesToUse.size();

    for(int i=0; i<s; ++i)
    {
        QStandardItem *ii = new QStandardItem();
        ii->setData(qVariantFromValue((void*)&item));
        ii->setEditable(false);
        l << ii;
    }

    if(dynamic_cast<const CT_AbstractSingularItemDrawable*>(&item) != NULL)
    {
        QList<CT_AbstractItemAttribute*> lIA = ((const CT_AbstractSingularItemDrawable&)item).itemAttributes();
        QListIterator<CT_AbstractItemAttribute*> itIA(lIA);

        while(itIA.hasNext())
        {
            CT_AbstractItemAttribute *att = itIA.next();
            int index = m_dataReferencesToUse.indexOf(att->model());

            if(index != -1)
            {
                QStandardItem *ii = l.at(index + GTreeView::COLUMN_FIRST_DATA_VALUE);
                ii->setText(att->toString((const CT_AbstractSingularItemDrawable*)&item, NULL));
            }
        }
    }
    else
    {
        itemDisplay->setText(tr("Groupe"));
    }

    return l;
}

bool GTreeView::canConstructTheModel() const
{
    return !m_typeBuilder.isEmpty();
}

QList<CT_AbstractItemDrawable *> GTreeView::itemDrawableForTreeView() const
{
    return document()->getItemDrawable();
}

int GTreeView::nLevelToConstruct() const
{
    return m_options.nLevelFiltering();
}

QList<CT_AbstractItemDrawable *> GTreeView::expandedItem() const
{
    return m_expandedItems;
}

QStandardItem* GTreeView::itemFromItemDrawable(const CT_AbstractItemDrawable *item) const
{
    QStandardItem *root = m_model->invisibleRootItem();

    int size = root->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *it = root->child(i, 0);

        if(itemDrawableFromItem(it) == item)
            return it;
    }

    return NULL;
}

void GTreeView::refreshHeaders()
{
    QStringList head;

    // if you change head order change => COLUMN_FIRST_DATA_VALUE

    head << tr("Sel.");

    QListIterator<CT_OutAbstractModel*> it(m_dataReferencesToUse);

    while(it.hasNext())
        head << it.next()->displayableName();

    m_model->setHorizontalHeaderLabels(head);

    #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    m_treeView->header()->setResizeMode(QHeaderView::ResizeToContents);
    #else
    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    #endif
}

DM_ActionsHandlerForTreeView* GTreeView::actionsHandlerTreeView() const
{
    return m_actionsHandler;
}

QList<CT_AbstractItemDrawable *> GTreeView::itemDrawableFromRowSelected() const
{
    QList<CT_AbstractItemDrawable*> listItem;

    QModelIndexList listSelected = m_treeView->selectionModel()->selectedRows();
    QListIterator<QModelIndex> it(listSelected);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *id = itemDrawableFromItem(itemFromIndex(it.next()));

        if(id != NULL)
            listItem.append(id);
    }

    return listItem;
}

bool GTreeView::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_treeView)
    {
        if(event->type() == QEvent::DragEnter)
        {
            if(actionsHandlerTreeView()->itemDragEnterEvent((QDragEnterEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::DragMove)
        {
            if(actionsHandlerTreeView()->itemDragMoveEvent((QDragMoveEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::DragLeave)
        {
            if(actionsHandlerTreeView()->itemDragLeaveEvent((QDragLeaveEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::Drop)
        {
            if(actionsHandlerTreeView()->itemDropEvent((QDropEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::KeyPress)
        {
            if(actionsHandlerTreeView()->keyPressEvent((QKeyEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::KeyRelease)
        {
            if(actionsHandlerTreeView()->keyReleaseEvent((QKeyEvent*)event))
                return true;
        }
    }
    else if(obj == m_treeView->viewport())
    {
        if(event->type() == QEvent::MouseMove)
        {
            if(actionsHandlerTreeView()->mouseMoveEvent((QMouseEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::MouseButtonPress)
        {
            if(actionsHandlerTreeView()->mousePressEvent((QMouseEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            if(actionsHandlerTreeView()->mouseReleaseEvent((QMouseEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::MouseButtonDblClick)
        {
            if(actionsHandlerTreeView()->mouseDoubleClickEvent((QMouseEvent*)event))
                return true;
        }
        else if(event->type() == QEvent::Wheel)
        {
            if(actionsHandlerTreeView()->wheelEvent((QWheelEvent*)event))
                return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void GTreeView::initContextMenu()
{
    m_contextMenu->clear();

    QAction *action = m_contextMenu->addAction(tr("Sélectionner"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(slotSelect()));

    action = m_contextMenu->addAction(tr("Dé-Sélectionner"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(slotDeSelect()));

    action = m_contextMenu->addAction(tr("Inverser la sélection"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(slotInverseSelection()));

    m_contextMenu->addSeparator();

    action = m_contextMenu->addAction(tr("Couleur automatique"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(slotColorAuto()));

    action = m_contextMenu->addAction(tr("Couleur unie"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(slotColorSolid()));

    m_contextMenu->addSeparator();

    QList<DocumentInterface*> documents = GUI_MANAGER->getDocumentManagerView()->documents();
    QListIterator<DocumentInterface*> it(documents);

    while(it.hasNext())
    {
        DM_DocumentView *doc = (DM_DocumentView*)it.next();

        if(doc->isVisible()
                && (doc->getNumber() != document()->getNumber()))
        {
            action = m_contextMenu->addAction(tr("Ajouter au ") + doc->getTitle());
            action->setData(doc->getNumber());
            connect(action, SIGNAL(triggered()), this, SLOT(slotAddSelectedToDocument()));
        }
    }
}

void GTreeView::setValidColorForLineFilter(bool valid)
{
    m_lineFilter->setStyleSheet(QString("QLineEdit{background: ") + (valid ? "green" : "red") + ";}");

    QTimer::singleShot(500, this, SLOT(slotResetColorLineFilter()));
}

void GTreeView::reconstructReferencesToUse()
{
    QList<CT_OutAbstractModel *> uniqueReferences;

    QList<DM_ItemDrawableType<CT_OutAbstractItemModel*, CT_AbstractItemDrawable> > types = m_typeBuilder.types();
    QListIterator<DM_ItemDrawableType<CT_OutAbstractItemModel*, CT_AbstractItemDrawable> > it(types);

    while(it.hasNext())
    {
        const DM_ItemDrawableType<CT_OutAbstractItemModel*, CT_AbstractItemDrawable>  &type = it.next();

        if(!type.isEmpty())
            recursiveAddToReferencesToUseForModel(uniqueReferences, type.type());
    }

    m_dataReferencesToUse = uniqueReferences;
}

void GTreeView::recursiveAddToReferencesToUseForModel(QList<CT_OutAbstractModel *> &uniqueReferences, const CT_OutAbstractModel *model)
{
    if(dynamic_cast<const CT_OutAbstractSingularItemModel*>(model) != NULL)
    {
        const CT_OutAbstractSingularItemModel *sModel = (const CT_OutAbstractSingularItemModel*)model;
        QListIterator<CT_OutAbstractItemAttributeModel*> itR(sModel->itemAttributes());

        while(itR.hasNext())
        {
            CT_OutAbstractModel *ref = itR.next()->originalModel();

            if(!uniqueReferences.contains(ref))
                uniqueReferences.append(ref);
        }
    }
    else
    {
        QList<CT_AbstractModel*> child = model->childrens();
        QListIterator<CT_AbstractModel*> it(child);

        while(it.hasNext())
            recursiveAddToReferencesToUseForModel(uniqueReferences, (CT_OutAbstractModel*)it.next());
    }
}

void GTreeView::reconstructCompleter()
{
    QStringList comp;

    QListIterator<CT_OutAbstractModel*> itH(m_dataReferencesToUse);

    while(itH.hasNext())
    {
        CT_OutAbstractModel *ref = itH.next();
        comp << (ref->displayableName() + " : ");
        comp << (ref->displayableName() + " : " + ((DM_SortFilterMathProxyModel*)m_treeView->model())->variableInMathExpression());
    }

    QCompleter *lastCompleter = m_lineFilter->completer();
    QCompleter *completer = NULL;

    if(!comp.isEmpty())
    {
        completer = new QCompleter(comp);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    }

    m_lineFilter->setCompleter(completer);

    delete lastCompleter;
}

void GTreeView::slotNewItemTypeDetected()
{
    reconstructReferencesToUse();
    reconstructCompleter();
    construct();
}

void GTreeView::slotItemTypeRemoved()
{
    reconstructReferencesToUse();
    reconstructCompleter();
    construct();
}

void GTreeView::construct()
{
    m_treeViewController.constructModel();
}
