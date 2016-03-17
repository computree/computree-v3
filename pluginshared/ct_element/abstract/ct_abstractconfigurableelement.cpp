#include "ct_abstractconfigurableelement.h"

CT_AbstractConfigurableElement::CT_AbstractConfigurableElement()
{
}

CT_AbstractConfigurableElement::~CT_AbstractConfigurableElement()
{
}

QString CT_AbstractConfigurableElement::getUniqueName() const
{
    return metaObject()->className();
}

QString CT_AbstractConfigurableElement::getShortDisplayableName() const
{
    return getUniqueName();
}

QString CT_AbstractConfigurableElement::getDetailledDisplayableName() const
{
    return getShortDisplayableName();
}

QString CT_AbstractConfigurableElement::getDetailledDescription() const
{
    return getShortDescription();
}
