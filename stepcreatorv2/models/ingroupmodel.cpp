#include "ingroupmodel.h"

INGroupModel::INGroupModel()
{
    _widget = new INGroupWidget();
    setText("Group");
}

INGroupModel::~INGroupModel()
{
    delete _widget;
}
