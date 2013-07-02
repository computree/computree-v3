#include "initemmodel.h"

INItemModel::INItemModel()
{
    _widget = new INItemWidget();
    setText("Item");
}

INItemModel::~INItemModel()
{
    delete _widget;
}
