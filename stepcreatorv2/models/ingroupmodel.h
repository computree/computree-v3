#ifndef INGROUPMODEL_H
#define INGROUPMODEL_H

#include "models/abtractmodel.h"
#include "widgets/ingroupwidget.h"

class INGroupModel : public AbtractModel
{
public:
    INGroupModel();
    ~INGroupModel();


    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Group_IN;}
    virtual AbstractWidget* getWidget() {return _widget;}

private:
    INGroupWidget* _widget;
};

#endif // INGROUPMODEL_H
