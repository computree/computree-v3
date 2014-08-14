#ifndef CT_STANDARDITEMDATAREF_H
#define CT_STANDARDITEMDATAREF_H

#include "ct_itemdrawable/tools/ct_abstractitemdataref.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardItemDataRef : public CT_AbstractItemDataRef
{
public:
    CT_StandardItemDataRef(int userType,
                           IItemDataValue::ItemDataValueType dataType,
                           QString name,
                           QString displayableName,
                           CT_AbstractItemDataValueDelegate *delegate = NULL);

    // if you use this constructor the delegate must not be NULL !!!
    CT_StandardItemDataRef(int userType,
                           QString name,
                           QString displayableName,
                           CT_AbstractItemDataValueDelegate *delegate);

    bool configure() { return true; }

    SettingsNodeGroup* saveConfiguration() const;
    bool loadConfiguration(const SettingsNodeGroup *root) const;

    CT_AbstractItemDataRef* copy() const;

};

#endif // CT_STANDARDITEMDATAREF_H
