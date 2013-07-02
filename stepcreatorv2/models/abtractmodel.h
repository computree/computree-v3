#ifndef ABTRACTMODEL_H
#define ABTRACTMODEL_H

#include "qstandarditemmodel.h"

class AbtractModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_IN,
        M_Group_IN,
        M_Item_IN,
        M_Result_OUT,
        M_Group_OUT,
        M_Item_OUT
    };

    AbtractModel();

    virtual AbtractModel::ModelType getModelType() = 0;
};

#endif // ABTRACTMODEL_H
