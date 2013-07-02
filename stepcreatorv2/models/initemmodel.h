#ifndef INITEMMODEL_H
#define INITEMMODEL_H

#include "models/abtractmodel.h"


class INItemModel : AbtractModel
{
public:
    INItemModel();

    virtual AbtractModel::ModelType getModelType() {return AbtractModel::M_Item_IN;}
};

#endif // INITEMMODEL_H
