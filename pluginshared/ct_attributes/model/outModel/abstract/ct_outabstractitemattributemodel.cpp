#include "ct_outabstractitemattributemodel.h"

CT_OutAbstractItemAttributeModel::CT_OutAbstractItemAttributeModel(const QString &uniqueName,
                                                                   const QString &description,
                                                                   const QString &displayableName,
                                                                   CT_AbstractItemAttribute *attribute) : DEF_CT_OutAbstractIAModel(uniqueName,
                                                                                                                                    description,
                                                                                                                                    displayableName)
{
    m_attribute = attribute;

    if(m_attribute != NULL)
        m_attribute->setModel(this);
}

CT_OutAbstractItemAttributeModel::~CT_OutAbstractItemAttributeModel()
{
    clearItemAttribute();
}

CT_AbstractItemAttribute* CT_OutAbstractItemAttributeModel::itemAttribute() const
{
    return m_attribute;
}

void CT_OutAbstractItemAttributeModel::clearItemAttribute()
{
    delete m_attribute;
    m_attribute = NULL;
}

void CT_OutAbstractItemAttributeModel::setItemAttribute(CT_AbstractItemAttribute *attribute)
{
    clearItemAttribute();
    m_attribute = attribute;

    if(m_attribute != NULL)
        m_attribute->setModel(this);
}
