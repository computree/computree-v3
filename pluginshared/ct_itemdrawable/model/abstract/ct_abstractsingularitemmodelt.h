#ifndef CT_ABSTRACTSINGULARITEMMODELT_H
#define CT_ABSTRACTSINGULARITEMMODELT_H

#include "ct_tools/model/ct_autorenamemodels.h"

#include <QList>
#include <QString>

/**
 * @brief Represent a model for an singular item (IN or OUT). A singular item can only have item attributes for childrens.
 *
 *        Use the template parameters to define if you want a IN or OUT model
 */
template <class GroupModelT, class AttributModelT, class InheritedT>
class CT_AbstractSingularItemModelT : public InheritedT
{
public:
    CT_AbstractSingularItemModelT(const QString &uniqueName,
                                  const QString &description,
                                  const QString &displayableName);

    virtual ~CT_AbstractSingularItemModelT();

    /**
     * @brief Add a model of item attribute
     * @return false and delete the model if the item attribute (modelName) already exist in hierarchy
     */
    virtual bool addItemAttribute(AttributModelT *attribute);

    /**
     * @brief Add a model of item attribute
     * @param autoName : a reference to an object of type CT_AutoRenameModels that is used to rename automatically
     *                   the model added. You must keep in your code the CT_AutoRenameModels to get the unique name
     *                   of the model if you want to find it in a list.
     *
     * @return true in all case
     */
    virtual bool addItemAttribute(AttributModelT *attribute, CT_AutoRenameModels &autoName);

    /**
     * @brief delete the item attribute from this item (and delete it from memory) (compare address)
     * @param attribute : item attribute to remove
     * @return true if the item attribute was found and deleted
     */
    virtual bool removeItemAttribute(AttributModelT *attribute);

    /**
     * @brief Returns the list of item attribute of the item
     */
    virtual const QList<AttributModelT*>& itemAttributes() const;

    /**
     * @brief Returns the item attribute corresponding with the modelName in the list. NULL if it was not founded.
     */
    virtual AttributModelT* findItemAttribute(const QString &uniqueModelName) const;

    /**
     * @brief Delete all item attributes from memory and removed them from the list
     */
    virtual void clearItemAttributes();

    /*!
     * @brief Returns the parent group
     */
    virtual GroupModelT* parentGroup() const;

    /*!
     * @brief Returns the root group
     */
    virtual GroupModelT* rootGroup() const;

    // INHERITED FROM CT_AbstractModel
    virtual QList<CT_AbstractModel*> childrens() const;
    virtual bool isEmpty() const;

    /**
     * @brief Returns a copy of the item and a copy of all children
     */
    //virtual CT_AbstractSingularItemModelT<GroupModelT, AttributModelT, InheritedT> copyItem() const = 0;

protected:
    /**
     * @brief Add the item attribute to list without verification
     */
    void internalAddItemAttribute(AttributModelT *attr);

    /**
     * @brief Called when a item attribute is added to the list
     */
    virtual void itemAttributeAdded(const AttributModelT *att) { Q_UNUSED(att) }

    /**
     * @brief Called when a item attribute is removed from the list
     */
    virtual void itemAttributeRemoved(const AttributModelT *att) { Q_UNUSED(att) }

private:
    QList<AttributModelT*>          m_attributes;
};

#include "ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.hpp"

#endif // CT_ABSTRACTSINGULARITEMMODELT_H
