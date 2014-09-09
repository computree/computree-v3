#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>
#include <QMenu>

#include "view/DocumentView/ItemModelViews/gitemmodelview.h"

#include "tools/itemdrawable/dm_itemdrawablebuildert.h"

#include "tools/treeview/dm_itemdrawabletreeviewcontroller.h"
#include "tools/treeview/dm_iitemdrawablestandarditembuilder.h"

#include "tools/treeview/dm_actionshandlerfortreeview.h"

#include "dm_itemdrawablemanageroptions.h"

#include "ct_attributes/abstract/ct_abstractitemattribute.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

class GTreeView : public QWidget, public GItemModelView, public DM_IItemDrawableStandardItemBuilder, public DM_ITreeViewManager
{
    Q_OBJECT

public:
    explicit GTreeView(QWidget *parent = 0);
    ~GTreeView();

    // GItemModelView
    void init();
    QWidget* getViewWidget() const;
    void beginAddMultipleItemDrawable();
    void endAddMultipleItemDrawable();
    void beginRemoveMultipleItemDrawable();
    void endRemoveMultipleItemDrawable();
    DM_ActionsHandler* actionsHandler() const;

    // TreeViewInterface
    QStandardItem* itemFromIndex(const QModelIndex &proxyIndex) const;
    CT_AbstractItemDrawable* itemDrawableFromItem(const QStandardItem *item) const;
    QModelIndex indexAt(const QPoint &point) const;
    QWidget* treeViewport() const;
    void setSelectionMode(QAbstractItemView::SelectionMode mode);
    QAbstractItemView::SelectionMode selectionMode() const;
    QModelIndexList selectedIndexes () const;
    QItemSelectionModel* selectionModel() const;
    void refreshAll();
    void refreshItems(const QList<QStandardItem*> &items);
    void refreshItems(const QList<CT_AbstractItemDrawable*> &items);

    // DM_IItemDrawableStandardItemBuilder
    QList<QStandardItem*> createItems(const CT_AbstractItemDrawable &item, const int &level) const;

    // DM_ITreeViewManager
    bool canConstructTheModel() const;
    QList<CT_AbstractItemDrawable*> itemDrawableForTreeView() const;
    int nLevelToConstruct() const;
    QList<CT_AbstractItemDrawable*> expandedItem() const;
    QStandardItem* itemFromItemDrawable(const CT_AbstractItemDrawable *item) const;
    void refreshHeaders();

    // THIS
    DM_ActionsHandlerForTreeView* actionsHandlerTreeView() const;
    QList<CT_AbstractItemDrawable*> itemDrawableFromRowSelected() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    DM_ActionsHandlerForTreeView            *m_actionsHandler;

    QMenu                                   *m_contextMenu;

    QTreeView                               *m_treeView;
    QStandardItemModel                      *m_model;
    DM_ItemDrawableTreeViewController       m_treeViewController;

    DM_ItemDrawableBuilderT<CT_OutAbstractItemModel*, CT_AbstractItemDrawable>       m_typeBuilder;

    QList<CT_AbstractItemDrawable*>         m_expandedItems;

    QLineEdit                               *m_lineFilter;

    QList< CT_OutAbstractModel* >           m_dataReferencesToUse;

    DM_ItemDrawableManagerOptions           m_options;

    static int                              COLUMN_FIRST_DATA_VALUE;

    /**
     * @brief (re)Init the context menu
     */
    void initContextMenu();

    /**
     * @brief Call this method if you want to change color of the QlineEdit used for filter the treeview
     */
    void setValidColorForLineFilter(bool valid);

    /**
     * @brief Call this method when you want to reconstruct references to use
     */
    void reconstructReferencesToUse();

    /**
     * @brief Called by method reconstructReferencesToUse to add references from the model passed in parameter
     */
    void recursiveAddToReferencesToUseForModel(QList< CT_OutAbstractModel* > &uniqueReferences, const CT_OutAbstractModel *model);

    /**
     * @brief Call this method when you want to reconstruct the completer of the line edit
     */
    void reconstructCompleter();

private slots:

    /**
     * @brief Called when a new type (a new type of model) of itemdrawable is detected
     */
    void slotNewItemTypeDetected();

    /**
     * @brief Called when a type of itemdrawable (a type of model) is removed
     */
    void slotItemTypeRemoved();

    /**
     * @brief Called when a CT_AbstractItemDrawable is added to the document
     */
    void slotAddItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when a CT_AbstractItemDrawable is removed from the document
     */
    void slotRemoveItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Called when the button 'Sync with' is clicked
     */
    void slotButtonSyncWithClicked();

    /**
     * @brief Called when a index in the treeview is activated
     */
    void slotActivated(const QModelIndex &index);

    /**
     * @brief Called when a index in the treeview is clicked
     */
    void slotClicked(const QModelIndex &index);

    /**
     * @brief Called when the a index in the treeview is double clicked
     */
    void slotDoubleClicked(const QModelIndex &index);

    /**
     * @brief Called when a index in the treeview is expanded
     */
    void slotExpanded(const QModelIndex &index);

    /**
     * @brief Called when a index in the treeview is collapsed
     */
    void slotCollapsed(const QModelIndex &index);

    /**
     * @brief Called when text changed from line edit filter
     */
    void slotSetTextFilter();

    /**
     * @brief Called when we must show the context menu on the tree view
     */
    void slotShowContextMenu(const QPoint &p);

    /**
     * @brief Called when we must show the context menu on the tree view header
     */
    void slotShowHeaderContextMenu(const QPoint &p);

    /**
     * @brief Called when the timer is finish and we must reset the color of the QLineEdit that filter the treeview
     */
    void slotResetColorLineFilter();

    /**
     * @brief Called when a data of a QStandardItem in the model changed (per example when the itemDrawable is selected)
     */
    void slotItemDataChanged(QStandardItem *item);

    /**
     * @brief Called when we must show the colors options dialog
     */
    void slotShowColorOptions();

    /**
     * @brief Called when we must select CT_AbstractItemDrawable of QStandardItem selected
     */
    void slotSelect();

    /**
     * @brief Called when we must de-select CT_AbstractItemDrawable of QStandardItem selected
     */
    void slotDeSelect();

    /**
     * @brief Called when we must inverse the selection of QStandardItem selected
     */
    void slotInverseSelection();

    /**
     * @brief Called when we must set automatic color of CT_AbstractItemDrawable selected
     */
    void slotColorAuto();

    /**
     * @brief Called when we must set unique color of CT_AbstractItemDrawable selected
     */
    void slotColorSolid();

    /**
     * @brief Called from a QAction to add selected item to the document (data in QAction)
     */
    void slotAddSelectedToDocument();

public slots:
    /**
     * @brief Call to (re)construct the model of the TreeView
     */
    void construct();

signals:
    /**
     * @brief emit when the button Sync with is clicked
     */
    void syncWith(const GItemModelView *view);
};

#endif // GTREEVIEW_H
