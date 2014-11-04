#include "ct_abstractpointsattributescontainer.h"

CT_AbstractPointsAttributesContainer::CT_AbstractPointsAttributesContainer() : CT_AbstractSingularItemDrawable()
{
}

CT_AbstractPointsAttributesContainer::CT_AbstractPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(model,
                                                                                                                                              result)
{
}

CT_AbstractPointsAttributesContainer::CT_AbstractPointsAttributesContainer(const QString &modelName,
                                                                           const CT_AbstractResult *result) : CT_AbstractSingularItemDrawable(modelName,
                                                                                                                                              result)
{
}

QString CT_AbstractPointsAttributesContainer::staticGetType()
{
    return CT_AbstractSingularItemDrawable::staticGetType() + "/CT_AbstractPointsAttributesContainer";
}

CT_AbstractPointsAttributes* CT_AbstractPointsAttributesContainer::pointsAttributesAt(const int &key) const
{
    return m_att.value(key, NULL);
}

void CT_AbstractPointsAttributesContainer::internalInsertAttributes(const int &key, const CT_AbstractPointsAttributes *att)
{
    m_att.insert(key, (CT_AbstractPointsAttributes*)att);
}
