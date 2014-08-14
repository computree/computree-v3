#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>
#include <QMenu>

#include "view/DocumentView/ItemModelViews/gitemmodelview.h"

#include "tools/itemdrawable/dm_itemdrawabletypebuilder.h"

#include "tools/treeview/dm_itemdrawabletreeviewcontroller.h"
#include "tools/treeview/dm_iitemdrawablestandarditembuilder.h"

#include "tools/treeview/dm_actionshandlerfortreeview.h"

#include "dm_itemdrawablemanageroptions.h"

class DataRefPairCompare : public QPair<QString, IItemDataRef*>
{
public:
    DataRefPairCompare(QString name, IItemDataRef *ref) : QPair<QString, IItemDataRef*>(name, ref) {}

    bool operator ==(const DataRefPairCompare &other) const
    {
        return other.first == this->first;
    }
};

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
    ItemDrawable* itemDrawableFromItem(const QStandardItem *item) const;
    QModelIndex indexAt(const QPoint &point) const;
    QWidget* treeViewport() const;
    void setSelectionMode(QAbstractItemView::SelectionMode mode);
    QAbstractItemView::SelectionMode selectionMode() const;
    QModelIndexList selectedIndexes () const;
    QItemSelectionModel* selectionModel() const;
    void refreshAll();
    void refreshItems(const QList<QStandardItem*> &items);
    void refreshItems(const QList<ItemDrawable*> &items);

    // DM_IItemDrawableStandardItemBuilder
    QList<QStandardItem*> createItems(const ItemDrawable &item, const int &level) const;

    // DM_ITreeViewManager
    bool canConstructTheModel() const;
    QList<ItemDrawable*> itemDrawableForTreeView() const;
    int nLevelToConstruct() const;
    QList<ItemDrawable*> expandedItem() const;
    QStandardItem* itemFromItemDrawable(const ItemDrawable *item) const;
    void refreshHeaders();

    // THIS
    DM_ActionsHandlerForTreeView* actionsHandlerTreeView() const;
    QList<ItemDrawable*> itemDrawableFromRowSelected() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    DM_ActionsHandlerForTreeView            *m_actionsHandler;

    QMenu                                   *m_contextMenu;

    QTreeView                               *m_treeView;
    QStandardItemModel                      *m_model;
    DM_ItemDrawableTreeViewController       m_treeViewController;

    DM_ItemDrawableTypeBuilder              m_typeBuilder;

    QList<ItemDrawable*>                    m_expandedItems;

    QLineEdit                               *m_lineFilter;

    mutable IItemDataValue                  *m_dataValue;

    QList<DataRefPairCompare>               m_dataReferencesToUse;

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
     * @brief Call this method when you want to reconstruct the completer of the line edit
     */
    void reconstructCompleter();

    /**
     * @brief Return true if 'ref' exist in m_dataReferencesToUse (compare unique name)
     */
    bool checkIfReferenceExistInReferences(const IItemDataRef *ref, const QList<IItemDataRef*> &refs) const;

private slots:

    /**
     * @brief Called when a new type of itemdrawable is detected
     */
    void slotNewItemTypeDetected(DM_ItemDrawableType type);

    /**
     * @brief Called when a type of itemdrawable is removed
     */
    void slotItemTypeRemoved(DM_ItemDrawableType type);

    /**
     * @brief Called when a ItemDrawable is added to the document
     */
    void slotAddItemDrawable(ItemDrawable &item);

    /**
     * @brief Called when a ItemDrawable is removed from the document
     */
    void slotRemoveItemDrawable(ItemDrawable &item);

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
     * @brief Called when we must select ItemDrawable of QStandardItem selected
     */
    void slotSelect();

    /**
     * @brief Called when we must de-select ItemDrawable of QStandardItem selected
     */
    void slotDeSelect();

    /**
     * @brief Called when we must inverse the selection of QStandardItem selected
     */
    void slotInverseSelection();

    /**
     * @brief Called when we must set automatic color of ItemDrawable selected
     */
    void slotColorAuto();

    /**
     * @brief Called when we must set unique color of ItemDrawable selected
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
