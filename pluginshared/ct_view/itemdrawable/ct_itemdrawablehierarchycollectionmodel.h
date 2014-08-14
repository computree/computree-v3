#ifndef CT_ITEMDRAWABLEHIERARCHYCOLLECTIONMODEL_H
#define CT_ITEMDRAWABLEHIERARCHYCOLLECTIONMODEL_H

#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionselectionmodel.h"

#include <QList>

class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableHierarchyCollectionModel
{
public:
    CT_ItemDrawableHierarchyCollectionModel();
    ~CT_ItemDrawableHierarchyCollectionModel();

    void addModel(CT_ItemDrawableHierarchyCollectionSelectionModel *model);

    const QList<CT_ItemDrawableHierarchyCollectionSelectionModel*>& models() const;

private:
    QList<CT_ItemDrawableHierarchyCollectionSelectionModel*> m_models;
};

#endif // CT_ITEMDRAWABLEHIERARCHYCOLLECTIONMODEL_H
