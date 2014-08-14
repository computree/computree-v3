#ifndef CT_ITEMATTRIBUTECONTAINER_H
#define CT_ITEMATTRIBUTECONTAINER_H

#include "ct_attributes/abstract/ct_abstractitemattribute.h"

#include <QHash>

/**
 * @brief Just a container that manage item attributes. It use a QHash
 *        with the key => the result that contains the item attribute
 *        with the value => a list of item attributes (contained in the result [KEY])
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemAttributeContainer
{
public:
    CT_ItemAttributeContainer();
    virtual ~CT_ItemAttributeContainer();

    /**
     * @brief Add an attribute
     */
    void addItemAttribute(CT_AbstractItemAttribute *att);

    /**
     * @brief Remove an attribute
     */
    void removeItemAttribute(CT_AbstractItemAttribute *att);

    /**
     * @brief Remove all attributes contained in the result passed in parameter
     */
    void removeItemAttributeFromResult(CT_AbstractResult *result);

    /**
     * @brief Returns all attributes
     */
    QList<CT_AbstractItemAttribute*> itemAttributes() const;

    /**
     * @brief Returns all attributes contained in the result passed in parameter
     */
    QList<CT_AbstractItemAttribute*> itemAttributesFromResult(const CT_AbstractResult *result) const;

    /**
     * @brief Clear all attributes from the container
     */
    void clear();

private:
    QHash<CT_AbstractResult*, QList<CT_AbstractItemAttribute*>*> m_attributes;
};

#endif // CT_ITEMATTRIBUTECONTAINER_H
