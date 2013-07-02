#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abtractmodel.h"
#include "widgets/initemwidget.h"


class INItemModel : public AbtractModel
{
public:
    INItemModel();
    ~INItemModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Item_IN;}
    virtual AbstractWidget* getWidget() {return _widget;}

private:
    INItemWidget* _widget;
};

#endif // INITEMMODEL_H
