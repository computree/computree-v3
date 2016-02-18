#ifndef GITEMDRAWABLEMODELMANAGER_H
#define GITEMDRAWABLEMODELMANAGER_H

#include <QWidget>
#include <QStandardItemModel>

#include "dm_itemdrawablemodelmanager.h"
#include "dm_itemdrawablemanageroptions.h"
#include "tools/itemdrawable/dm_contextmenucolouristadder.h"

#include <QMenu>

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

struct GActionSetColorByInfo {
    GActionSetColorByInfo(): m_model(NULL), m_docIndex(-1) {}
    GActionSetColorByInfo(CT_OutAbstractItemAttributeModel *m, int docI) : m_model(m), m_docIndex(docI) {}
    CT_OutAbstractItemAttributeModel    *m_model;
    int                                 m_docIndex;
};

class GItemDrawableModelManager : public QWidget, public DM_ItemDrawableModelManager, public IColouristContextMenuAccess
{
    Q_OBJECT
    
public:
    explicit GItemDrawableModelManager(QWidget *parent = 0);
    ~GItemDrawableModelManager();
    
    void setDocumentManagerView(const GDocumentManagerView *docManagerView);

    void setResult(const CT_AbstractResult *res);
    void setColorOptions(const DM_ItemDrawableManagerOptions &options);

    DM_ContextMenuColouristAdder* contextMenuColouristAdder() const;

    CT_AbstractResult* result() const;

    QList<const CT_AbstractModel*> getSelectedIModels() const;

    void reconstruct();

private:
    Ui::GItemDrawableModelManager   *ui;

    GDocumentManagerView            *m_docManagerView;

    CT_AbstractResult               *_result;
    QStandardItemModel              _viewModel;

    DM_ItemDrawableManagerOptions   _colorOptions;

    QMenu                           *m_contextMenu;
    DM_ContextMenuColouristAdder    *m_contextMenuColorAdder;

    void clearModel();
    void constructModel();
    void constructHeader();
    void reConstructContextMenu();

    QList<QStandardItem*> recursiveCreateItemsForModel(const CT_OutAbstractModel *model);

    QList<QStandardItem*> createItemsForModel(const CT_OutAbstractModel *model);
    QList<QStandardItem*> createItemsForResultModel(const CT_OutAbstractResultModel *model);
    QList<QStandardItem*> createItemsForItemModel(const CT_OutAbstractItemModel *model);

    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, int column) const;
    QStandardItem* recursiveGetStandardItemForModel(QStandardItem *pItem, CT_OutAbstractModel *model, const DocumentInterface *doc) const;

    void recursiveSetCheckBoxEnable(QStandardItem *parent, bool enable);

    // IColouristContextMenuAccess
    QList<CT_AbstractItemDrawable*> getItemDrawableToColorize() const;
    QList<CT_AbstractModel*> getSelectedModelsToUseInColorizerMenu() const;

private slots:

    void documentAdded(DM_DocumentView *view);
    void documentRemoved();
    void refreshCheckbox();

    void modelVisibilityChanged(bool visible);
    void modelVisibilityInDocumentChanged(const DocumentInterface *doc, bool visible);
    void itemChanged(QStandardItem *item);

    void resultDestroyedDirect();

    void showContextMenu(const QPoint &point);
};

Q_DECLARE_METATYPE(GActionSetColorByInfo)

#endif // GITEMDRAWABLEMODELMANAGER_H
