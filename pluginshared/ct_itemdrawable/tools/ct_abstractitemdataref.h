#ifndef CT_ABSTRACTITEMDATAREF_H
#define CT_ABSTRACTITEMDATAREF_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdatavaluedelegate.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDataRef : public IItemDataRef
{
    Q_INTERFACES(IItemDataRef)

public:
    CT_AbstractItemDataRef(int userType,
                           IItemDataValue::ItemDataValueType dataType,
                           QString name,
                           QString displayableName,
                           CT_AbstractItemDataValueDelegate *delegate = NULL);

    // if you use this constructor the delegate must not be NULL !!!
    CT_AbstractItemDataRef(int userType,
                           QString name,
                           QString displayableName,
                           CT_AbstractItemDataValueDelegate *delegate);

    ~CT_AbstractItemDataRef();

    int userType() const;

    QString name() const;
    QString displayableName() const;
    IItemDataValue::ItemDataValueType dataType() const;
    CT_AbstractItemDataValueDelegate* delegate() const;

    virtual CT_AbstractItemDataRef* copy() const = 0;

private:

    int                                 _userType;
    QString                             _name;
    QString                             _displayableName;
    IItemDataValue::ItemDataValueType   _dataType;
    CT_AbstractItemDataValueDelegate    *m_delegate;
};

#endif // CT_ABSTRACTITEMDATAREF_H
