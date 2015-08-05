#include "ct_abstractmetric.h"
#include "ct_attributes/ct_stditemattributet.h"

CT_AbstractMetric::CT_AbstractMetric() : CT_AbstractConfigurableElement()
{
    _result = NULL;
}

CT_AbstractMetric::CT_AbstractMetric(const CT_AbstractMetric *other)
{
    _attributes = other->_attributes;
    _result = other->_result;
}

CT_AbstractMetric::~CT_AbstractMetric()
{
    QMutableMapIterator<QString, AttributeObject*> it(_attributes);
    while (it.hasNext())
    {
        AttributeObject* attributeObject = it.next().value();

        if (attributeObject != NULL)
        {
            if (attributeObject->_instance != NULL)
            {
                delete attributeObject->_instance;
            }
            delete attributeObject;
        }
    }
    _attributes.clear();
}

void CT_AbstractMetric::initAttributesModels(CT_OutResultModelGroupToCopyPossibilities* resPoss, const CT_AutoRenameModels &parentModel)
{
    createAttributes();

    QMutableMapIterator<QString, AttributeObject*> it(_attributes);
    while (it.hasNext())
    {
        AttributeObject *attributeObject = it.next().value();

        CT_AbstractItemAttribute *attributePrototype = NULL;

        switch (attributeObject->_type)
        {
            case AT_bool:    attributePrototype = new CT_StdItemAttributeT<bool>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            case AT_double:  attributePrototype = new CT_StdItemAttributeT<double>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            case AT_float:   attributePrototype = new CT_StdItemAttributeT<float>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            case AT_int:     attributePrototype = new CT_StdItemAttributeT<int>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            case AT_size_t:  attributePrototype = new CT_StdItemAttributeT<size_t>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            case AT_QString: attributePrototype = new CT_StdItemAttributeT<QString>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
            default :        attributePrototype = new CT_StdItemAttributeT<double>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(attributeObject->_category), NULL, 0); break;
        }

        resPoss->addItemAttributeModel(parentModel,
                                       *(attributeObject->_autoRenameModel),
                                       attributePrototype,
                                       attributeObject->_name);
    }
}

void CT_AbstractMetric::computeMetric(CT_AbstractSingularItemDrawable *item)
{
    computeMetric();

    QMutableMapIterator<QString, AttributeObject*> it(_attributes);
    while (it.hasNext())
    {
        AttributeObject* attributeObject = it.next().value();

        if (attributeObject->_instance != NULL)
        {
            item->addItemAttribute(attributeObject->_instance);
        }
    }

    _attributes.clear();
}


void CT_AbstractMetric::addAttribute(QString name, CT_AbstractMetric::AttributeType type, QString category)
{
    _attributes.insert(name, new AttributeObject(QString("%1_%2").arg(getName()).arg(name), type, category, new CT_AutoRenameModels()));
}


CT_AbstractMetric::AttributeObject *CT_AbstractMetric::getAttributeObject(QString attributeName, CT_AbstractMetric::AttributeType type)
{
    if (!_attributes.contains(attributeName))
    {
        qDebug() << tr("Impossible de trouver l'attribut %1 (métrique : %2) !").arg(attributeName).arg(getName());
        return NULL;
    }
    CT_AbstractMetric::AttributeObject *attributeObject = (CT_AbstractMetric::AttributeObject*) _attributes.value(attributeName);

    if (attributeObject->_type != type)
    {
        qDebug() << tr("Mauvais type pour l'attribut %1 !").arg(attributeName);
        return NULL;
    }

    if (attributeObject->_instance != NULL) {delete attributeObject->_instance; attributeObject->_instance = NULL;}
    return attributeObject;
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, bool &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_bool);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<bool>(attributeObject->_autoRenameModel->completeName(),
                                                                    attributeObject->_category,
                                                                    _result,
                                                                    value);
    }
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, double &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_double);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<double>(attributeObject->_autoRenameModel->completeName(),
                                                                      attributeObject->_category,
                                                                      _result,
                                                                      value);
    }
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, float &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_float);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<float>(attributeObject->_autoRenameModel->completeName(),
                                                                     attributeObject->_category,
                                                                     _result,
                                                                     value);
    }
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, int &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_int);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<int>(attributeObject->_autoRenameModel->completeName(),
                                                                   attributeObject->_category,
                                                                   _result,
                                                                   value);
    }
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, size_t &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_size_t);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<size_t>(attributeObject->_autoRenameModel->completeName(),
                                                                      attributeObject->_category,
                                                                      _result,
                                                                      value);
    }
}

void CT_AbstractMetric::setAttributeValue(QString attributeName, QString &value)
{
    AttributeObject *attributeObject = getAttributeObject(attributeName, AT_QString);
    if (attributeObject != NULL)
    {
        attributeObject->_instance = new CT_StdItemAttributeT<QString>(attributeObject->_autoRenameModel->completeName(),
                                                                       attributeObject->_category,
                                                                       _result,
                                                                       value);
    }
}
