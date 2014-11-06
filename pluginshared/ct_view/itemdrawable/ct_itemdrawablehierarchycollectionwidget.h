#ifndef CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H
#define CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.h"

namespace Ui {
class CT_ItemDrawableHierarchyCollectionWidget;
}

/**
 * @brief A widget that can show attributes that can be selected to use it in export
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableHierarchyCollectionWidget : public QWidget
{
    Q_OBJECT

public:

    enum CloudType {
        POINTS,
        FACES,
        EDGES,
        NONE
    };

    explicit CT_ItemDrawableHierarchyCollectionWidget(CloudType cloudType, QWidget *parent = 0);
    ~CT_ItemDrawableHierarchyCollectionWidget();

    /**
     * @brief Call this method before 'setModel' to enable/disable the use of document's colors)
     */
    void setCanSelectColors(bool enable);

    /**
     * @brief Call this method before 'setModel' to enable/disable the use of document's normals
     */
    void setCanSelectNormals(bool enable);

    /**
     * @brief Set the document manager (if exist)
     */
    void setDocumentManager(const DocumentManagerInterface *dm);

    /**
     * @brief Set the collection of attributes that can be selected to use it in export
     */
    void setModel(CT_ItemDrawableHierarchyCollectionModel *model);

    /**
     * @brief Returns true if at least one element can be selected (if at least one element is present in the selection model)
     */
    bool hasChoice() const;

    /**
     * @brief Returns true if choice can be selected by default
     */
    bool canChoiceBeSelectedByDefault() const;

    /**
     * @brief Select choice by default
     */
    bool selectChoiceByDefault();

    /**
     * @brief Returns the color cloud selected (NULL if it was not selected)
     */
    QSharedPointer<CT_StandardColorCloudRegistered> colorCloudSelected() const;

    /**
     * @brief Returns the normal cloud selected (NULL if it was not selected)
     */
    QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudSelected() const;

    /**
     * @brief Returns itemdrawable selected
     */
    QList<CT_AbstractSingularItemDrawable*> itemDrawableSelected() const;

private:
    Ui::CT_ItemDrawableHierarchyCollectionWidget            *ui;
    CloudType                                               m_cloudType;
    GraphicsViewInterface::ColorCloudType                   m_colorCloudType;
    GraphicsViewInterface::NormalCloudType                  m_normalCloudType;
    DocumentManagerInterface                                *m_dm;
    CT_ItemDrawableHierarchyCollectionModel                 *m_modelCollection;
    QStandardItemModel                                      m_model;
    int                                                     m_nChoice;
    CT_ItemDrawableHierarchyCollectionSelectionModel        *m_selectionModelForPointCloud;
    CT_ItemDrawableHierarchyCollectionSelectionModel        *m_selectionModelForNormalCloud;
    bool                                                    m_canSelectColors;
    bool                                                    m_canSelectNormals;

    void constructHeader();
    QList<QStandardItem*> createItems(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm, const int &index) const;
    QList<QStandardItem*> createItemsForItemDrawable(const CT_ItemDrawableHierarchyCollectionSelectionModel *sm, const CT_AbstractSingularItemDrawable *itemd, const int &index) const;
    QList<QStandardItem*> createItemsForColorCloud(QSharedPointer<CT_StandardColorCloudRegistered> colors) const;
    QList<QStandardItem*> createItemsForNormalCloud(QSharedPointer<CT_StandardNormalCloudRegistered> normals) const;

    void recursiveUncheckAllItemsExcept(QStandardItem *root, const QStandardItem *item, const CT_ItemDrawableHierarchyCollectionSelectionModel *sm);
    QList<QStandardItem*> recursiveItemsSelected(QStandardItem *root) const;

    /**
     * @brief Returns all checkable QStandardItem
     */
    QList<QStandardItem*> checkableItems(QStandardItem *root) const;

private slots:
    void itemChanged(QStandardItem *item);
};

#endif // CT_ITEMDRAWABLEHIERARCHYCOLLECTIONWIDGET_H
