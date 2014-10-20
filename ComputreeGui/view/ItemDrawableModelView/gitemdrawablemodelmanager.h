#ifndef GITEMDRAWABLEMODELMANAGER_H
#define GITEMDRAWABLEMODELMANAGER_H

#include <QWidget>
#include <QStandardItemModel>

#include "dm_itemdrawablemodelmanager.h"
#include "dm_itemdrawablemanageroptions.h"

#include <QMenu>

class QtColorPicker;

class CT_AbstractModel;
class CT_OutAbstractModel;
class CT_OutAbstractItemModel;
class CT_OutAbstractItemAttributeModel;
class CT_OutAbstractResultModel;
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

    void setResult(const CT_AbstractResult *res);
    void setColorOptions(const DM_ItemDrawableManagerOptions &options);

    CT_AbstractResult* result() const;

    QList<const CT_AbstractModel*> getSelectedIModels() const;

    void reconstruct();

private:
    Ui::GItemDrawableModelManager   *ui;

    GDocumentManagerView            *m_docManagerView;

    CT_AbstractResult                          *_result;
    QStandardItemModel              _viewModel;

    QtColorPicker                   *_colorPicker;

    DM_ItemDrawableManagerOptions   _colorOptions;

    void clearModel();
    void constructModel();
    void constructHeader();
    QMenu* constructContextMenu();

    QList<QStandardItem*> recursiveCreateItemsForModel(const CT_OutAbstractModel *model);

    QList<QStandardItem*> createItemsForModel(const CT_OutAbstractModel *model);
    QList<QStandardItem*> createItemsForResultModel(const CT_OutAbstractResultModel *model);
    QList<QStandardItem*> createItemsForItemModel(const CT_OutAbstractItemModel *model);

    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, int column) const;
    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, const DocumentInterface *doc) const;

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
