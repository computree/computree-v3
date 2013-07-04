#ifndef INRESULTMODEL_H
#define INRESULTMODEL_H

#include "models/abtractmodel.h"

class INResultModel : public AbtractModel
{
public:
    INResultModel();
    ~INResultModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Result_IN;}
    virtual AbstractWidget* getWidget() {return _widget;}
    virtual QString getName();
    virtual bool isValid();

    virtual QString getInModelsDefinition();
    virtual QString getInComputeContent();

private:

    INResultWidget* _widget;

};

#endif // INRESULTMODEL_H
