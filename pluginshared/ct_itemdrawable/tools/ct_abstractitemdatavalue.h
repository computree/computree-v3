#ifndef CT_ABSTRACTITEMDATAVALUE_H
#define CT_ABSTRACTITEMDATAVALUE_H

#include "interfaces.h"

#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDataValue : public IItemDataValue
{
    Q_INTERFACES(IItemDataValue)

public:
    CT_AbstractItemDataValue();

    virtual bool memoryCopy() const = 0;
};

#endif // CT_ABSTRACTITEMDATAVALUE_H
