#include "ct_instdmodelpossibilitygroup.h"

CT_InStdModelPossibilityGroup::CT_InStdModelPossibilityGroup()
{
    m_exclusive = false;
    m_enable = true;
    m_minMustBeSelected = 1;
    m_maxCanBeSelected = 9999;
}

CT_InStdModelPossibilityGroup::~CT_InStdModelPossibilityGroup()
{
    clearPossibilities();
}

void CT_InStdModelPossibilityGroup::addPossibility(const CT_InStdModelPossibility *p)
{
    CT_InStdModelPossibility* pp = (CT_InStdModelPossibility*)p;

    if(!m_possibilities.contains(pp))
    {
        m_possibilities.append(pp);

        pp->setGroup(this);
    }
}

bool CT_InStdModelPossibilityGroup::removePossibility(const CT_InStdModelPossibility *p)
{
    CT_InStdModelPossibility* pp = (CT_InStdModelPossibility*)p;

    if(m_possibilities.removeOne(pp))
    {
        pp->setGroup(NULL);
        return true;
    }

    return false;
}

bool CT_InStdModelPossibilityGroup::isSelected(const CT_InStdModelPossibility *p) const
{
    return m_possibilitiesSelected.contains((CT_InStdModelPossibility*)p);
}

void CT_InStdModelPossibilityGroup::select(CT_InStdModelPossibility *p)
{
    if(!isSelected(p)
            && isEnable())
    {
        if(isExclusive())
        {
            while(!m_possibilitiesSelected.isEmpty())
            {
                CT_InStdModelPossibility *oP = m_possibilitiesSelected.first();
                oP->setSelected(false);
                unSelect(oP);
            }
        }

        m_possibilitiesSelected.append(p);

        while(m_possibilitiesSelected.size() > maximumNumberOfPossibilityThatCanBeSelected())
            m_possibilitiesSelected.first()->setSelected(false);
    }
}

void CT_InStdModelPossibilityGroup::unSelect(CT_InStdModelPossibility *p)
{
    m_possibilitiesSelected.removeOne(p);
}

void CT_InStdModelPossibilityGroup::setExclusive(bool ex)
{
    m_exclusive = ex;

    if(m_exclusive)
    {
        while(m_possibilitiesSelected.size() > 1)
            m_possibilitiesSelected.removeLast();
    }
}

bool CT_InStdModelPossibilityGroup::isExclusive() const
{
    return m_exclusive;
}

void CT_InStdModelPossibilityGroup::setEnable(bool en)
{
    m_enable = en;

    if(!m_enable)
        m_possibilitiesSelected.clear();
}

bool CT_InStdModelPossibilityGroup::setMinimumNumberOfPossibilityThatMustBeSelected(int n)
{
    if((n > maximumNumberOfPossibilityThatCanBeSelected()) || (n <= 0))
        return false;

    m_minMustBeSelected = n;

    return true;
}

int CT_InStdModelPossibilityGroup::minimumNumberOfPossibilityThatMustBeSelected() const
{
    return m_minMustBeSelected;
}

bool CT_InStdModelPossibilityGroup::setMaximumNumberOfPossibilityThatCanBeSelected(int n)
{
    if((n < minimumNumberOfPossibilityThatMustBeSelected()) || (n <= 0))
        return false;

    m_maxCanBeSelected = n;

    while(m_possibilitiesSelected.size() > maximumNumberOfPossibilityThatCanBeSelected())
        m_possibilitiesSelected.first()->setSelected(false);

    return true;
}

int CT_InStdModelPossibilityGroup::maximumNumberOfPossibilityThatCanBeSelected() const
{
    return m_maxCanBeSelected;
}

bool CT_InStdModelPossibilityGroup::isValid() const
{
    return (m_minMustBeSelected <= m_possibilitiesSelected.size()) && (m_possibilitiesSelected.size() <= m_maxCanBeSelected);
}

bool CT_InStdModelPossibilityGroup::isEnable() const
{
    return m_enable;
}

const QList<CT_InStdModelPossibility*>& CT_InStdModelPossibilityGroup::getPossibilities() const
{
    return m_possibilities;
}

int CT_InStdModelPossibilityGroup::nPossibilities() const
{
    return m_possibilities.size();
}

void CT_InStdModelPossibilityGroup::clearPossibilities()
{
    m_possibilities.clear();
    m_possibilitiesSelected.clear();
}

