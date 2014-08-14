#ifndef CT_OUTABSTRACTSINGULARITEMMODEL_H
#define CT_OUTABSTRACTSINGULARITEMMODEL_H

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"

class CT_OutAbstractItemAttributeModel;
class CT_AbstractSingularItemDrawable;

/**
 * @brief Represent a OUTPUT model for a singular item.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractSingularItemModel : public CT_OutAbstractItemModel
{
public:
    /**
     * @brief Construct a OUTPUT model for a singular item
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractSingularItemModel(const QString &uniqueName,
                                    const QString &description,
                                    const QString &displayableName);
    virtual ~CT_OutAbstractSingularItemModel();

    /**
     * @brief Returns the real singular item of this model
     */
    CT_AbstractSingularItemDrawable* singularItem() const;

    // INHERITED FROM CT_OutAbstractItemModel //
    CT_AbstractItemDrawable* item() const;

    /**
     * @brief Returns all item attributes model contained in this model of item
     */
    virtual const QList<CT_OutAbstractItemAttributeModel*>& itemAttributes() const = 0;

protected:

    /**
     * @brief Delete the item from this model
     */
    void clearItem();

    /**
     * @brief Set the item of this model
     */
    void setSingularItem(CT_AbstractSingularItemDrawable *item);

private:

    CT_AbstractSingularItemDrawable     *_item;
};

#endif // CT_OUTABSTRACTSINGULARITEMMODEL_H
