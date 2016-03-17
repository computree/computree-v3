#include "ct_abstractmetric.h"
#include "ct_attributes/ct_stditemattributet.h"

CT_AbstractMetric::CT_AbstractMetric() : CT_AbstractConfigurableElement()
{
    m_result = NULL;
}

CT_AbstractMetric::CT_AbstractMetric(const CT_AbstractMetric &other)
{
    m_result = other.m_result;

    AttributesContainerIterator it(m_attributes);

    while(it.hasNext()) {
        it.next();
        m_attributes.insert(it.key(), it.value()->copy());
    }
}

CT_AbstractMetric::~CT_AbstractMetric()
{
    clearAttributes();
}

QString CT_AbstractMetric::getDetailledDisplayableName() const
{
    QString result = getShortDisplayableName();

    if (m_attributes.size() > 0)
    {
        result += tr(" : ");

        AttributesContainerIterator it(m_attributes);

        while(it.hasNext())
        {
            AbstractAttributeObject *attributeObject = it.next().value();
            result.append(attributeObject->_displayableName);

            if(it.hasNext())
                result += ", ";
        }
    }
    return result;
}

void CT_AbstractMetric::initAttributesModels(CT_OutResultModelGroupToCopyPossibilities* resPoss, const CT_AutoRenameModels &parentModel)
{
    AttributesContainerIterator it(m_attributes);

    while(it.hasNext())
    {
        AbstractAttributeObject *attributeObject = it.next().value();

        CT_AbstractItemAttribute *attributePrototype = attributeObject->createPrototypeAttribute();

        resPoss->addItemAttributeModel(parentModel,
                                       *(attributeObject->_autoRenameModel),
                                       attributePrototype,
                                       attributeObject->_displayableName);
    }
}

void CT_AbstractMetric::computeMetric(CT_AbstractSingularItemDrawable *item)
{
    if((item == NULL) || (item->result() == NULL))
        return;

    m_result = dynamic_cast<CT_ResultGroup*>(item->result());

    computeMetric();

    AttributesContainerIterator it(m_attributes);

    while (it.hasNext())
    {
        AbstractAttributeObject* attributeObject = it.next().value();

        if (attributeObject->_attributeInstance != NULL)
        {
            item->addItemAttribute(attributeObject->_attributeInstance);
            attributeObject->_attributeInstance = NULL;
        }
    }
}

void CT_AbstractMetric::clearAttributes()
{
    qDeleteAll(m_attributes.begin(), m_attributes.end());
    m_attributes.clear();
}

void CT_AbstractMetric::postConfigure()
{
    clearAttributes();
    createAttributes();
}

CT_AbstractMetric::AbstractAttributeObject* CT_AbstractMetric::getAttributeObject(void* classMember, CT_AbstractCategory::ValueType type)
{
    CT_AbstractMetric::AbstractAttributeObject *attributeObject = m_attributes.value(classMember, NULL);

    if(attributeObject == NULL)
    {
        PS_LOG->addErrorMessage(LogInterface::metric, tr("Impossible de trouver l'attribut recherché (métrique : %2) !").arg(getShortDisplayableName()));
        return NULL;
    }

    if(attributeObject->valueType() != type)
    {
        PS_LOG->addErrorMessage(LogInterface::metric, tr("Mauvais type pour l'attribut recherché (%1 au lieu de %2) !").arg(CT_AbstractCategory::valueTypeToString(type)).arg(attributeObject->valueType()));
        return NULL;
    }

    return attributeObject;
}

const CT_AbstractMetric::AttributesContainer &CT_AbstractMetric::attributes() const
{
    return m_attributes;
}

bool CT_AbstractMetric::internalInsertAttributeObject(void *key, CT_AbstractMetric::AbstractAttributeObject *obj)
{
    if(m_attributes.contains(key))
        return false;

    m_attributes.insert(key, obj);
    return true;
}

bool CT_AbstractMetric::removeAttribute(void *key)
{
    AttributesContainerMutableIterator it(m_attributes);

    while(it.hasNext()) {
        it.next();

        if(it.key() == key)
        {
            delete it.value();
            it.remove();

            return true;
        }
    }

    return false;
}
