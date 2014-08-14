#include "ct_abstractitemdataref.h"

CT_AbstractItemDataRef::CT_AbstractItemDataRef(int userType,
                                               IItemDataValue::ItemDataValueType dataType,
                                               QString name,
                                               QString displayableName,
                                               CT_AbstractItemDataValueDelegate *delegate)
{
    _userType = userType;
    _dataType = dataType;
    _name = name;
    _displayableName = displayableName.isEmpty() ? name : displayableName;
    m_delegate = delegate;
}

CT_AbstractItemDataRef::CT_AbstractItemDataRef(int userType,
                                               QString name,
                                               QString displayableName,
                                               CT_AbstractItemDataValueDelegate *delegate)
{
    _userType = userType;
    _name = name;
    _displayableName = displayableName.isEmpty() ? name : displayableName;
    m_delegate = delegate;
    _dataType = m_delegate->dataType();
}

CT_AbstractItemDataRef::~CT_AbstractItemDataRef()
{
    delete m_delegate;
}

int CT_AbstractItemDataRef::userType() const
{
    return _userType;
}

QString CT_AbstractItemDataRef::name() const
{
    return _name;
}

QString CT_AbstractItemDataRef::displayableName() const
{
    return _displayableName;
}

IItemDataValue::ItemDataValueType CT_AbstractItemDataRef::dataType() const
{
    return _dataType;
}

CT_AbstractItemDataValueDelegate* CT_AbstractItemDataRef::delegate() const
{
    return m_delegate;
}
