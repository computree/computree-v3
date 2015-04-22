#ifndef CT_OUTABSTRACTITEMATTRIBUTEMODEL_H
#define CT_OUTABSTRACTITEMATTRIBUTEMODEL_H

#include "ct_attributes/model/outModel/abstract/def_ct_outabstractitemattributemodel.h"
#include "ct_attributes/abstract/ct_abstractitemattribute.h"

class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractItemAttributeModel : public DEF_CT_OutAbstractIAModel
{
public:
    CT_OutAbstractItemAttributeModel(const QString &uniqueName,
                                     const QString &description,
                                     const QString &displayableName,
                                     CT_AbstractItemAttribute *attribute);
    ~CT_OutAbstractItemAttributeModel();

    /**
     * @brief Returns the item attribute of the model
     */
    CT_AbstractItemAttribute* itemAttribute() const;

    /**
     * @brief Returns true if this model or is originalModel() is a default item attribute model (default = created by the itemdrawable statically = not added by a step)
     */
    bool isADefaultItemAttributeModel() const;

protected:

    /**
     * @brief Delete the item attribute from this model
     */
    void clearItemAttribute();

    /**
     * @brief Set the item attribute of this model
     */
    void setItemAttribute(CT_AbstractItemAttribute *attribute);

private:
    CT_AbstractItemAttribute    *m_attribute;
    bool                        m_default;

protected:
    friend class CT_DefaultItemAttributeManager;

    /**
     * @brief Called from CT_DefaultItemAttributeManager to set this attribute model as default
     */
    void setAsDefaultItemAttributeModel();
};

#endif // CT_OUTABSTRACTITEMATTRIBUTEMODEL_H
