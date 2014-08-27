#ifndef CT_INABSTRACTSINGULARITEMMODEL_H
#define CT_INABSTRACTSINGULARITEMMODEL_H

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractitemmodel.h"

#include <QString>

class CT_InAbstractItemAttributeModel;

/**
 * @brief Represent a model for an INPUT singular item. A singular item has item attributes for children.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InAbstractSingularItemModel : public CT_InAbstractItemModel
{
public:

    /**
     * @brief Construct a singular item model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractSingularItemModel(const QString &uniqueName,
                                   const QString &description,
                                   const QString &displayableName);

    /**
     * @brief Returns all attributes model contained in this model of item
     */
    virtual const QList<CT_InAbstractItemAttributeModel*>& itemAttributes() const = 0;
};

#endif // CT_INABSTRACTSINGULARITEMMODEL_H
