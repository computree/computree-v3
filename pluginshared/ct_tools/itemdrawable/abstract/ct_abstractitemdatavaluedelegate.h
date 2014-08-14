#ifndef CT_ABSTRACTITEMDATAVALUEDELEGATE_H
#define CT_ABSTRACTITEMDATAVALUEDELEGATE_H

#include "interfaces.h"
#include "pluginShared_global.h"

#include "ct_itemdrawable/tools/ct_standarditemdatavalue.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDataValueDelegate
{
public:
    CT_AbstractItemDataValueDelegate() {}
    virtual ~CT_AbstractItemDataValueDelegate() {}

    IItemDataValue* value(const CT_AbstractItemDrawable *item) const
    {
        CT_StandardItemDataValue *dv = new CT_StandardItemDataValue();

        value(item, dv);

        return dv;
    }

    virtual bool value(const CT_AbstractItemDrawable *item, IItemDataValue* valueToSet) const = 0;

    virtual IItemDataValue::ItemDataValueType dataType() const = 0;

    virtual CT_AbstractItemDataValueDelegate* copy() const = 0;
};

#endif // CT_ABSTRACTITEMDATAVALUEDELEGATE_H
