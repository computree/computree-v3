#ifndef CT_INABSTRACTSINGULARITEMMODEL_H
#define CT_INABSTRACTSINGULARITEMMODEL_H

#include "ct_model/inModel/abstract/ct_inabstractmodel.h"

#include <QString>

class CT_InAbstractItemAttributeModel;

/**
 * @brief Represent a model for an INPUT singular item. A singular item has item attributes for children.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InAbstractSingularItemModel : public CT_InAbstractModel
{
public:

    /**
     * @brief Construct a singular item model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractSingularItemModel(QString uniqueName,
                                   QString description,
                                   QString displayableName);

    /**
     * @brief Return the type of the singular item researched (CT_ItemXXX::statiGetType)
     */
    QString itemType() const;

    /**
     * @brief Return the short type of the singular item (the string after the last '/' character in the type string)
     */
    QString itemShortType() const;

    /**
     * @brief Returns all attributes model contained in this model of item
     */
    virtual const QList<CT_InAbstractItemAttributeModel*>& itemAttributes() const = 0;

protected:

    /**
     * @brief Set the item's type
     */
    void setItemType(const QString &itemType);

private:

    QString                     m_itemType;
    QString                     m_itemShortType;
};

#endif // CT_INABSTRACTSINGULARITEMMODEL_H
