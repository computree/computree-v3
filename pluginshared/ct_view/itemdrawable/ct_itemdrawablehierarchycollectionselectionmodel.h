#ifndef CT_ITEMDRAWABLEHIERARCHYCOLLECTIONSELECTIONMODEL_H
#define CT_ITEMDRAWABLEHIERARCHYCOLLECTIONSELECTIONMODEL_H

#include "ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchystep.h"

class PLUGINSHAREDSHARED_EXPORT CT_ItemDrawableHierarchyCollectionSelectionModel
{
public:
    CT_ItemDrawableHierarchyCollectionSelectionModel(const QString &name);

    QString name() const;

    void setCollection(const QList<CT_ItemDrawableCollectionHierarchyStep> &collection);
    const QList<CT_ItemDrawableCollectionHierarchyStep>& collection() const;

    void addExcludeModel(const int &index);
    void clearExcludeModel();
    bool mustExcludeModel(const int &index) const;

private:
    QList<CT_ItemDrawableCollectionHierarchyStep>   m_collection;
    QList<int>                                      m_exclude;
    QString                                         m_name;
};

#endif // CT_ITEMDRAWABLEHIERARCHYCOLLECTIONSELECTIONMODEL_H
