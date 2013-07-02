#ifndef INRESULTMODEL_H
#define INRESULTMODEL_H

#include "models/abtractmodel.h"


class INResultModel : AbtractModel
{
public:
    INResultModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Result_IN;}

};

#endif // INRESULTMODEL_H
