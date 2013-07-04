#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abtractmodel.h"

class INItemModel : public AbtractModel
{
public:
    INItemModel();
    ~INItemModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Item_IN;}
    virtual AbstractWidget* getWidget() {return _widget;}
    virtual QString getName();

    virtual QString getInModelsDefinition();
    virtual QString getInComputeContent();

private:
    INItemWidget* _widget;
};

#endif // INITEMMODEL_H
