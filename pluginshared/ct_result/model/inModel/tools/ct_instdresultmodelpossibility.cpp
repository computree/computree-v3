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

CT_InStdModelPossibility* CT_InStdResultModelPossibility::copy() const
{
    CT_InStdResultModelPossibility *p = new CT_InStdResultModelPossibility();
    p->setOutModel(*outModel());
    p->setSelected(isSelected());
    p->setAutoDeleteOutModel(false);

    if(m_inModel != NULL)
        p->setInModel(m_inModel->copy(true));

    return p;
}

void CT_InStdResultModelPossibility::setInModel(CT_InAbstractModel *model)
{
    delete m_inModel;
    m_inModel = model;
}
