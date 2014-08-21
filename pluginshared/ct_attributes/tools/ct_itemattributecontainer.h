#ifndef CT_ITEMATTRIBUTECONTAINER_H
#define CT_ITEMATTRIBUTECONTAINER_H

#include "ct_attributes/abstract/ct_abstractitemattribute.h"

#include <QHash>

class CT_InAbstractItemAttributeModel;

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
     * @return false if the attribute already exist (model uniqueName already exist) in this container
     */
    bool addItemAttribute(CT_AbstractItemAttribute *att);

    /**
     * @brief Remove an attribute
     */
    void removeItemAttribute(CT_AbstractItemAttribute *att);

    /**
     * @brief Remove all attributes contained in the result passed in parameter
     */
    void removeItemAttributeFromResult(const CT_AbstractResult *result);

    /**
     * @brief Returns all attributes
     */
    QList<CT_AbstractItemAttribute*> itemAttributes() const;

    /**
     * @brief Returns all attributes contained in the result passed in parameter
     */
    QList<CT_AbstractItemAttribute*> itemAttributesFromResult(const CT_AbstractResult *result) const;

    /**
     * @brief Returns the item attribute corresponding to the OUTPUT model passed in parameter
     * @return NULL if item attribute don't exist in this singular item
     */
    CT_AbstractItemAttribute* itemAttributeFromModel(const CT_OutAbstractItemAttributeModel *outModel) const;

    /**
     * @brief Returns a list of item attributes corresponding to the possibility selected of the INPUT model passed in parameter
     * @return NULL if item attribute don't exist in this singular item
     */
    QList<CT_AbstractItemAttribute*> itemAttributesFromModel(const CT_InAbstractItemAttributeModel *inModel) const;

    /**
     * @brief Clear all attributes from the container
     */
    void clear();

private:
    QHash<CT_AbstractResult*, QList<CT_AbstractItemAttribute*>*> m_attributes;
};

#endif // CT_ITEMATTRIBUTECONTAINER_H
