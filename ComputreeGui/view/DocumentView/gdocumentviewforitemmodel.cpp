#include "gdocumentviewforitemmodel.h"

#include <QLineEdit>

#include "view/ItemDrawableView/gitemdrawablemanageroptionscolor.h"
#include "view/StepResultTreeView/myqstandarditem.h"

#include "tools/treeview/dm_sortfiltermathproxymodel.h"

#include "dm_guimanager.h"
#include "dm_actionshandler.h"

#include "qtcolorpicker.h"

#include "ct_actions/abstract/ct_abstractactionfortreeview.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QPushButton>
#include <QMenu>
#include <QWhatsThis>

//#define USE_TYPES_IN_COMBOBOX

GDocumentViewForItemModel::GDocumentViewForItemModel(GDocumentManagerView &manager, QString title) : GDocumentView(manager, title)
{
    m_layoutViews= NULL;
}

GDocumentViewForItemModel::~GDocumentViewForItemModel()
{
}

void GDocumentViewForItemModel::init()
{
    GDocumentView::init();

    delete getCentralWidget()->layout();
    m_layoutViews = new QHBoxLayout(getCentralWidget());
}

QString GDocumentViewForItemModel::getType() const
{
    return "TAB";
}

void GDocumentViewForItemModel::addView(GItemModelView *view)
{
    view->setDocument(this);

    m_views.append(view);
    m_layoutViews->addWidget(view->getViewWidget());
}

const QList<GItemModelView *>& GDocumentViewForItemModel::getItemModelViewList() const
{
    return m_views;
}

void GDocumentViewForItemModel::beginAddMultipleItemDrawable()
{
    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        it.next()->beginAddMultipleItemDrawable();
}

void GDocumentViewForItemModel::endAddMultipleItemDrawable()
{
    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        it.next()->endAddMultipleItemDrawable();
}

void GDocumentViewForItemModel::beginRemoveMultipleItemDrawable()
{
    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        it.next()->beginRemoveMultipleItemDrawable();
}

void GDocumentViewForItemModel::endRemoveMultipleItemDrawable()
{
    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        it.next()->endRemoveMultipleItemDrawable();
}

bool GDocumentViewForItemModel::acceptAction(const CT_AbstractAction *action) const
{
    return (action == NULL) || (dynamic_cast<const CT_AbstractActionForTreeView*>(action) != NULL);
}

bool GDocumentViewForItemModel::setCurrentAction(CT_AbstractAction *action, bool deleteAction)
{
    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    emit currentActionChanged(action);

    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
    {
        if(action == NULL)
            it.next()->setCurrentAction(NULL);
        else
            it.next()->setCurrentAction(dynamic_cast<CT_AbstractActionForTreeView*>(action->copy()));
    }

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

bool GDocumentViewForItemModel::setDefaultAction(CT_AbstractAction *action, bool deleteAction)
{
    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    emit defaultActionChanged(action);

    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
    {
        if(action == NULL)
            it.next()->setDefaultAction(NULL);
        else
            it.next()->setDefaultAction(dynamic_cast<CT_AbstractActionForTreeView*>(action->copy()));
    }

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    return true;
}

CT_AbstractAction* GDocumentViewForItemModel::currentAction() const
{
    if(m_views.isEmpty())
        return NULL;

    return m_views.first()->actionsHandler()->currentAction();
}

CT_AbstractAction* GDocumentViewForItemModel::defaultAction() const
{
    if(m_views.isEmpty())
        return NULL;

    return m_views.first()->actionsHandler()->defaultAction();
}

void GDocumentViewForItemModel::removeActions(const QString &uniqueName) const
{
    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        it.next()->actionsHandler()->removeActions(uniqueName);
}

// TODO in default action
/*void GDocumentViewForItemModel::construct()
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("GDocumentViewForItemModel::construct()"));

    m_timerToRefresh.stop();
    refreshReferences();
    m_treeViewController.constructModel();
}

void GDocumentViewForItemModel::addSelectedItemToDocument(const int &number)
{
    QList<DocumentInterface*> documents = GUI_MANAGER->getDocumentManagerView()->documents();
    QListIterator<DocumentInterface*> it(documents);

    while(it.hasNext())
    {
        DM_DocumentView *doc = (DM_DocumentView*)it.next();

        if(doc->getNumber() == number)
        {
            QList<CT_AbstractItemDrawable *> items = getItemDrawableCorrespondingToRowSelected();

            GUI_MANAGER->asyncAddAllItemDrawableOfListOnView(items, doc, NULL);

            return;
        }
    }
}

QList<CT_AbstractItemDrawable*> GDocumentViewForItemModel::getItemDrawableCorrespondingToRowSelected()
{
    QList<CT_AbstractItemDrawable*> listItem;

    QModelIndexList listSelected = m_treeView->selectionModel()->selectedRows();
    QListIterator<QModelIndex> it(listSelected);

    while(it.hasNext())
    {
        QStandardItem *item = m_model->itemFromIndex(((QSortFilterProxyModel*)m_treeView->model())->mapToSource(it.next()));
        CT_AbstractItemDrawable *id = (CT_AbstractItemDrawable*)item->data().value<void*>();

        if(id != NULL)
            listItem.append(id);
    }

    return listItem;
}*/

/*void GDocumentViewForItemModel::setReferencesToUseOfItemDrawable(const CT_AbstractItemDrawable *item)
{
    if(item != NULL)
    {
        m_currentType = m_typeBuilder.getTypeForItemDrawable(*item);
        m_dataReferencesOfModel = item->getModel()->displayableName();
        m_dataReferencesToUse = item->dataReferencesListStatic()->references();

        QStringList comp;

        QListIterator<IItemDataRef*> it(m_dataReferencesToUse);

        while(it.hasNext())
        {
            IItemDataRef *ref = it.next();
            comp << (ref->displayableName() + " : ");

            if(ref->dataType() > IItemDataValue::IDVT_BOOL && ref->dataType() < IItemDataValue::IDVT_STRING)
                comp << (ref->displayableName() + " : " + ((DM_SortFilterMathProxyModel*)m_treeView->model())->variableInMathExpression());
        }

        QCompleter *lastC = m_lineFilter->completer();
        QCompleter *completer = new QCompleter(comp);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        m_lineFilter->setCompleter(completer);

        delete lastC;
    }
    else
    {
        m_dataReferencesOfModel.clear();
        m_dataReferencesToUse.clear();

        QCompleter *lastC = m_lineFilter->completer();
        m_lineFilter->setCompleter(NULL);

        delete lastC;
    }

    refreshLabelReferences();
}

void GDocumentViewForItemModel::refreshReferences()
{
    if(m_dataReferencesToUse.isEmpty())
    {
        m_dataReferencesOfModel.clear();

        #ifdef USE_TYPES_IN_COMBOBOX
        DM_ItemDrawableType type = m_typeBuilder.getTypeWithName(m_comboBox->currentItemText());

        if(!type.isEmpty())
            setReferencesToUseOfItemDrawable(type.itemDrawableCollection().first());
        #else
        if(nItemDrawable() > 0)
            setReferencesToUseOfItemDrawable(getItemDrawable(0));
        #endif
    }
}

void GDocumentViewForItemModel::refreshLabelReferences()
{
    if(m_dataReferencesOfModel.isEmpty())
        m_labelUseReferencesOf->setText(tr("Aucun type de données défini"));
    else
        m_labelUseReferencesOf->setText(tr("Affiche les données du type : ") + m_dataReferencesOfModel);
}

bool GDocumentViewForItemModel::checkIfReferenceExistInReferences(const IItemDataRef *ref, const QList<IItemDataRef *> &refs) const
{
    QListIterator<IItemDataRef*> it(refs);

    while(it.hasNext())
    {
        if(it.next()->name() == ref->name())
            return true;
    }

    return false;
}

void GDocumentViewForItemModel::initContextMenu()
{
    m_contextMenu->clear();

    QAction *action = m_contextMenu->addAction(tr("Modifier les titres des colonnes pour ce type d'CT_AbstractItemDrawable"));
    action->setEnabled(m_treeView->selectionModel()->selectedRows().size() == 1);
    connect(action, SIGNAL(triggered()), this, SLOT(slotUseReferences()));

    m_contextMenu->addSeparator();

    action = m_contextMenu->addAction(tr("Sélectionner"));
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
                && (doc->getNumber() != getNumber()))
        {
            action = m_contextMenu->addAction(tr("Ajouter au ") + doc->getTitle());
            action->setData(doc->getNumber());
            connect(action, SIGNAL(triggered()), this, SLOT(slotAddSelectedToDocument()));
        }
    }
}*/

/*void GDocumentViewForItemModel::slotMustAddItem(DM_ItemDrawableType type)
{
    #ifdef USE_TYPES_IN_COMBOBOX
    if(!m_comboBox->isRemoveEmptyTextWhenAddNewItem()
            && (m_comboBox->comboBox()->count() == 1))
        m_comboBox->setEmptyText(tr("........... Choisir un élément à afficher ..........."));

    m_comboBox->addItem(type.type(), type.type().right(type.type().size()-(type.type().lastIndexOf('/')+1)));
    #else
    if(m_currentType == "")
    {
        m_currentType = type.type();
        slotComboBoxItemActivated(m_currentType);
    }
    #endif
}

void GDocumentViewForItemModel::slotMustRemoveItem(DM_ItemDrawableType type)
{
    #ifdef USE_TYPES_IN_COMBOBOX
    m_comboBox->removeItem(type.type());

    if(m_comboBox->comboBox()->count() == 1)
    {
        m_comboBox->setEmptyText(tr("........... Aucun element visualisable dans la table ..........."));
        setReferencesToUseOfItemDrawable(NULL);
    }
    #else
    if(type.type() == m_currentType)
        m_currentType = "";

    if(m_typeBuilder.isEmpty())
    {
        setReferencesToUseOfItemDrawable(NULL);
        slotComboBoxItemActivated(m_currentType);
    }
    else if(m_currentType.isEmpty())
    {
        m_currentType = m_typeBuilder.types().first().type();
        setReferencesToUseOfItemDrawable(NULL);
        slotComboBoxItemActivated(m_currentType);
    }
    #endif
}*/

/*void GDocumentViewForItemModel::slotAddItemDrawable(CT_AbstractItemDrawable &item)
{
    m_typeBuilder.addItemDrawable(item);

    #ifdef USE_TYPES_IN_COMBOBOX
    // if the selected text in the combobox is the type of this itemdrawable
    if(m_comboBox->currentItemText() == m_typeBuilder.getTypeForItemDrawable(item))
        m_treeViewController.addItemDrawable(item);
    #else
    m_treeViewController.addItemDrawable(item);
    #endif
}

void GDocumentViewForItemModel::slotRemoveItemDrawable(CT_AbstractItemDrawable &item)
{
    m_typeBuilder.removeItemDrawable(item);

    #ifdef USE_TYPES_IN_COMBOBOX
    // if the selected text in the combobox is the type of this itemdrawable
    if(m_comboBox->currentItemText() == m_typeBuilder.getTypeForItemDrawable(item))
        m_treeViewController.removeItemDrawable(item);
    #else
    m_treeViewController.removeItemDrawable(item);
    #endif
}*/

/*void GDocumentViewForItemModel::slotSyncWithClicked()
{
    emit syncWith(this);
}*/

/*void GDocumentViewForItemModel::slotComboBoxItemActivated(const QString &text)
{
    setReferencesToUseOfItemDrawable(NULL);

    if(m_typeBuilder.existTypeWithName(text))
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotComboBoxItemActivated : ") + text);

        construct();
    }
    else
    {
        //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotComboBoxItemActivated = clearModel ") + text);

        m_model->clear();

        //create headers
        refreshHeaders();
    }
}

void GDocumentViewForItemModel::slotComboBoxItemChanged(const QString &text)
{
    //GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, QString("slotComboBoxItemChanged : ") + text);

    if(m_typeBuilder.existTypeWithName(text))
        m_timerToRefresh.start();
    else
        m_timerToRefresh.stop();
}*/

/*void GDocumentViewForItemModel::slotExpanded(const QModelIndex &i)
{
    m_expanded.append((CT_AbstractItemDrawable*)m_model->itemFromIndex(((QSortFilterProxyModel*)m_treeView->model())->mapToSource(i))->data().value<void*>());
}

void GDocumentViewForItemModel::slotCollapsed(const QModelIndex &i)
{
    m_expanded.removeOne((CT_AbstractItemDrawable*)m_model->itemFromIndex(((QSortFilterProxyModel*)m_treeView->model())->mapToSource(i))->data().value<void*>());
}*/

/*void GDocumentViewForItemModel::slotTimerRefreshTimeOut()
{
    setReferencesToUseOfItemDrawable(NULL);
    construct();
}*/

/*void GDocumentViewForItemModel::slotOptionsUpdated(DM_ItemDrawableManagerOptions options)
{
    m_options = options;
    m_typeBuilder.setNLevel(m_options.nLevelFiltering());
    construct();
}*/

/*void GDocumentViewForItemModel::slotSetTextFilter()
{
    QString textToUse = m_lineFilter->text();
    int columnToUse = -1;
    bool continueLoop = true;
    QListIterator<IItemDataRef*> it(m_dataReferencesToUse);

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
}*/

/*void GDocumentViewForItemModel::slotShowViewContextMenu(const QPoint &p)
{
    initContextMenu();
    m_contextMenu->exec(m_treeView->viewport()->mapToGlobal(p));
}*/

/*void GDocumentViewForItemModel::slotUseReferences()
{
    QList<CT_AbstractItemDrawable *> l = getItemDrawableCorrespondingToRowSelected();

    if(!l.isEmpty())
    {
        setReferencesToUseOfItemDrawable(l.first());
        construct();
    }
}*/

/*void GDocumentViewForItemModel::slotSelect()
{
    QList<CT_AbstractItemDrawable*> sel = getItemDrawableCorrespondingToRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
        it.next()->getItemDrawableSignalSlotManager()->select(true);

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GDocumentViewForItemModel::slotDeSelect()
{
    QList<CT_AbstractItemDrawable*> sel = getItemDrawableCorrespondingToRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
        it.next()->getItemDrawableSignalSlotManager()->select(false);

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GDocumentViewForItemModel::slotInverseSelection()
{
    QList<CT_AbstractItemDrawable*> sel = getItemDrawableCorrespondingToRowSelected();
    QListIterator<CT_AbstractItemDrawable*> it(sel);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *ii = it.next();
        ii->getItemDrawableSignalSlotManager()->select(!ii->isSelected());
    }

    GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
}

void GDocumentViewForItemModel::slotColorAuto()
{
    QList<CT_AbstractItemDrawable *> l = getItemDrawableCorrespondingToRowSelected();

    if(!l.isEmpty())
    {
        QListIterator<CT_AbstractItemDrawable*> it(l);

        while(it.hasNext())
            it.next()->getItemDrawableSignalSlotManager()->setColor(m_options.getNextColor());

        construct();
    }
}

void GDocumentViewForItemModel::slotColorSolid()
{
    QtColorPicker colorPicker(0, -1, true, false);
    colorPicker.showColorDialog();

    if(!colorPicker.isDialogCanceled())
    {
        QColor color = colorPicker.currentColor();

        QList<CT_AbstractItemDrawable *> l = getItemDrawableCorrespondingToRowSelected();
        QListIterator<CT_AbstractItemDrawable*> it(l);

        while(it.hasNext())
            it.next()->getItemDrawableSignalSlotManager()->setColor(color);

        construct();
    }
}*/

/*void GDocumentViewForItemModel::slotResetColorLineFilter()
{
    m_lineFilter->setStyleSheet("QLineEdit{background: white;}");
}*/

/*void GDocumentViewForItemModel::slotAddSelectedToDocument()
{
    QAction *act = (QAction*)sender();
    addSelectedItemToDocument(act->data().toInt());
}*/

/*void GDocumentViewForItemModel::slotItemDataChanged(QStandardItem *item)
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
                itemDrawable->getItemDrawableSignalSlotManager()->select(selected);
                GUI_MANAGER->getDocumentManagerView()->redrawAllDocument();
            }
        }
    }
}*/

/*void GDocumentViewForItemModel::slotShowColorOptions()
{
    GItemDrawableManagerOptionsColor dialog(getCentralWidget());

    dialog.setOptions(m_options);

    if(dialog.exec())
        m_options.setColorList(dialog.getColorList());
}*/

QList<InDocumentViewInterface *> GDocumentViewForItemModel::views() const
{
    QList<InDocumentViewInterface *> l;

    QListIterator<GItemModelView*> it(m_views);

    while(it.hasNext())
        l.append(it.next());

    return l;
}
