#ifndef CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H
#define CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.h"

namespace Ui {
class CT_ItemDrawableHierarchyCollectionWidget;
}

class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableHierarchyCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CT_ItemDrawableHierarchyCollectionWidget(QWidget *parent = 0);
    ~CT_ItemDrawableHierarchyCollectionWidget();

    void setDocumentManager(const DocumentManagerInterface *dm);

    void setModel(CT_ItemDrawableHierarchyCollectionModel *model);

    bool hasChoice() const;
    QSharedPointer<CT_StandardColorCloudRegistered> colorCloudSelected() const;
    QList<CT_AbstractSingularItemDrawable*> itemDrawableSelected() const;

private:
    Ui::CT_ItemDrawableHierarchyCollectionWidget            *ui;
    DocumentManagerInterface                                *m_dm;
    CT_ItemDrawableHierarchyCollectionModel                 *m_modelCollection;
    QStandardItemModel                                      m_model;
    bool                                                    m_choice;
    CT_ItemDrawableHierarchyCollectionSelectionModel        *m_selectionModelForCloud;

    void constructHeader();
    QList<QStandardItem*> createItems(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm, const int &index) const;
    QList<QStandardItem*> createItemsForItemDrawable(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm, const CT_AbstractSingularItemDrawable *itemd, const int &index) const;
    QList<QStandardItem*> createItemsForCloud(QSharedPointer<CT_StandardColorCloudRegistered> colors) const;

    void recursiveUncheckAllItemsExcept(QStandardItem *root, const QStandardItem *item, const CT_ItemDrawableHierarchyCollectionSelectionModel *sm);
    QList<QStandardItem*> recursiveItemsSelected(QStandardItem *root) const;

private slots:
    void itemChanged(QStandardItem *item);
};

#endif // CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H
