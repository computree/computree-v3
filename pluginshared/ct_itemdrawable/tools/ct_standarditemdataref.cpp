#include "ct_standarditemdataref.h"

CT_StandardItemDataRef::CT_StandardItemDataRef(int userType,
                                               IItemDataValue::ItemDataValueType dataType,
                                               QString name,
                                               QString displayableName,
                                               CT_AbstractItemDataValueDelegate *delegate) : CT_AbstractItemDataRef(userType,
                                                                                                                    dataType,
                                                                                                                    name,
                                                                                                                    displayableName,
                                                                                                                    delegate)
{
}

CT_StandardItemDataRef::CT_StandardItemDataRef(int userType,
                                               QString name,
                                               QString displayableName,
                                               CT_AbstractItemDataValueDelegate *delegate) : CT_AbstractItemDataRef(userType,
                                                                                                                    name,
                                                                                                                    displayableName,
                                                                                                                    delegate)
{

}

SettingsNodeGroup* CT_StandardItemDataRef::saveConfiguration() const
{
    SettingsNodeGroup *group = new SettingsNodeGroup("CT_StandardItemDataRef");
    group->addValue(new SettingsNodeValue("Version", "1"));

    return group;
}

bool CT_StandardItemDataRef::loadConfiguration(const SettingsNodeGroup *root) const
{
    Q_UNUSED(root)

    return true;
}

CT_AbstractItemDataRef* CT_StandardItemDataRef::copy() const
{
    return new CT_StandardItemDataRef(userType(), dataType(), name(), displayableName(), ((delegate() != NULL) ? delegate()->copy() : NULL));
}
