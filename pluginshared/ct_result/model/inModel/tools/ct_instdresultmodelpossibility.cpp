#include "ct_instdresultmodelpossibility.h"

#include "ct_model/inModel/abstract/ct_inabstractmodel.h"

CT_InStdResultModelPossibility::CT_InStdResultModelPossibility() : CT_InStdModelPossibility()
{
    m_inModel = NULL;
}

CT_InStdResultModelPossibility::~CT_InStdResultModelPossibility()
{
    delete m_inModel;
}

CT_InAbstractModel *CT_InStdResultModelPossibility::inModel() const
{
    return m_inModel;
}

void CT_InStdResultModelPossibility::setInModel(CT_InAbstractModel *model)
{
    delete m_inModel;
    m_inModel = model;
}
