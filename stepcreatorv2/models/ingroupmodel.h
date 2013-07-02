#ifndef INGROUPMODEL_H
#define INGROUPMODEL_H

#include "models/abtractmodel.h"

class INGroupModel : AbtractModel
{
public:
    INGroupModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Group_IN;}

};

#endif // INGROUPMODEL_H
