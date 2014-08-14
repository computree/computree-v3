#ifndef GITEMDRAWABLEMODELMANAGER_H
#define GITEMDRAWABLEMODELMANAGER_H

#include <QWidget>
#include <QStandardItemModel>

#include "dm_itemdrawablemodelmanager.h"
#include "dm_itemdrawablemanageroptions.h"

#include <QMenu>

class QtColorPicker;

class IModel;
class IItemModel;
class IResultModel;
class DM_DocumentView;
class GDocumentManagerView;
class DocumentInterface;

namespace Ui {
class GItemDrawableModelManager;
}

class GItemDrawableModelManager : public QWidget, public DM_ItemDrawableModelManager
{
    Q_OBJECT
    
public:
    explicit GItemDrawableModelManager(QWidget *parent = 0);
    ~GItemDrawableModelManager();
    
    void setDocumentManagerView(const GDocumentManagerView *docManagerView);

    void setResult(const Result *res);
    void setColorOptions(const DM_ItemDrawableManagerOptions &options);

    Result* result() const;

    QList<const IModel*> getSelectedIModels() const;

private:
    Ui::GItemDrawableModelManager   *ui;

    GDocumentManagerView            *m_docManagerView;

    Result                          *_result;
    QStandardItemModel              _viewModel;

    QMenu                           *_contextMenu;
    QtColorPicker                   *_colorPicker;

    DM_ItemDrawableManagerOptions   _colorOptions;

    void clearModel();
    void constructModel();
    void constructHeader();
    QMenu* constructContextMenu();

    QList<QStandardItem*> recursiveCreateItemsForResultModel(const IResultModel *model);
    QList<QStandardItem*> recursiveCreateItemsForItemModel(const IItemModel *model);

    QList<QStandardItem*> createItemsForResultModel(const IResultModel *model);
    QList<QStandardItem*> createItemsForItemModel(const IItemModel *model);

    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, IItemModel *model, int column) const;
    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, IItemModel *model, const DocumentInterface *doc) const;

    void recursiveSetCheckBoxEnable(QStandardItem *parent, bool enable);

private slots:

    void documentAdded(DM_DocumentView *view);
    void documentRemoved();
    void refreshCheckbox();

    void modelVisibilityChanged(bool visible);
    void modelVisibilityInDocumentChanged(const DocumentInterface *doc, bool visible);
    void itemChanged(QStandardItem *item);

    void resultDestroyedDirect();

    void showContextMenu(const QPoint &point);

    void setUniqueColorForModelSelected();
    void setAutomaticColorForModelSelected();
};

#endif // GITEMDRAWABLEMODELMANAGER_H
