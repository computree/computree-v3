#include "inresultmodel.h"

INResultModel::INResultModel()
{
    _widget = new INResultWidget();
    setText("Result");
}

INResultModel::~INResultModel()
{
    delete _widget;
}
