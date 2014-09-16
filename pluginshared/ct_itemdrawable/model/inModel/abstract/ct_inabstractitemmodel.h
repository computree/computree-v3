#ifndef CT_INABSTRACTITEMMODEL_H
#define CT_INABSTRACTITEMMODEL_H

#include "ct_model/inModel/abstract/ct_inabstractmodel.h"

/**
 * @brief Represent a model for an INPUT item.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InAbstractItemModel : public CT_InAbstractModel
{
public:
    /**
     * @brief Construct a item model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_InAbstractItemModel(const QString &uniqueName,
                           const QString &description,
                           const QString &displayableName);

    /**
     * @brief Return the type of the item researched (CT_ItemXXX::statiGetType)
     */
    QString itemType() const;

    /**
     * @brief Return the short type of the item (the string after the last '/' character in the type string)
     */
    QString itemShortType() const;

    /**
     * @brief Returns values to backup this model and its children
     */
    virtual QList<SettingsNodeGroup*> getAllValues() const;

    /**
     * @brief Restore this model and its children by values passed in parameter
     * @return false if it was an error in values
     */
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list);

protected:

    /**
     * @brief Set the item's type
     */
    void setItemType(const QString &itemType);

private:
    QString                     m_itemType;
    QString                     m_itemShortType;
};

#endif // CT_INABSTRACTITEMMODEL_H
