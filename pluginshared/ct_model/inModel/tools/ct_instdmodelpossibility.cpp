#include "ct_instdmodelpossibility.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

CT_InStdModelPossibility::CT_InStdModelPossibility()
{
    m_model = NULL;
    m_group = NULL;
    m_autoDeleteOutModel = false;
}

CT_InStdModelPossibility::~CT_InStdModelPossibility()
{
    clearOutModel();

    if(m_group != NULL)
        m_group->removePossibility(this);
}

void CT_InStdModelPossibility::setOutModel(const CT_OutAbstractModel &model)
{
    clearOutModel();

    m_model = (CT_OutAbstractModel*)&model;
}

void CT_InStdModelPossibility::setAutoDeleteOutModel(bool e)
{
    m_autoDeleteOutModel = e;
}

bool CT_InStdModelPossibility::mustAutoDeleteOutModel() const
{
    return m_autoDeleteOutModel;
}

CT_OutAbstractModel* CT_InStdModelPossibility::outModel() const
{
    return m_model;
}

bool CT_InStdModelPossibility::isSelected() const
{
    if(m_group == NULL)
        return false;

    return m_group->isSelected((CT_InStdModelPossibility*)this);
}

bool CT_InStdModelPossibility::setSelected(bool check)
{
    if(m_group == NULL)
        return false;

    bool lastValue = isSelected();

    if(check)
        m_group->select(this);
    else
        m_group->unSelect(this);

    bool newValue = isSelected();

    if(lastValue != newValue)
        emit selectStateChange(newValue);

    return (newValue == check);
}

void CT_InStdModelPossibility::clearOutModel()
{
    if(m_autoDeleteOutModel)
        delete m_model;

    m_model = NULL;
}

void CT_InStdModelPossibility::setGroup(const CT_InStdModelPossibilityGroup *group)
{
    m_group = (CT_InStdModelPossibilityGroup*)group;
}

CT_InStdModelPossibilityGroup* CT_InStdModelPossibility::group() const
{
    return m_group;
}

CT_InStdModelPossibility* CT_InStdModelPossibility::copy() const
{
    CT_InStdModelPossibility *p = new CT_InStdModelPossibility();
    p->setOutModel(*outModel());
    p->setSelected(isSelected());

    return p;
}

