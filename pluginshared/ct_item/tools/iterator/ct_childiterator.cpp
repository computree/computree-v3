#include "ct_childiterator.h"

#include "ct_item/abstract/ct_abstractitem.h"

CT_ChildIterator::CT_ChildIterator(const CT_AbstractItem *parent)
{
    m_childrens = parent->childrensForGui();
}

CT_ChildIterator::~CT_ChildIterator()
{
}

bool CT_ChildIterator::hasNext() const
{
    return !m_childrens.isEmpty();
}

const CT_AbstractItem* CT_ChildIterator::next()
{
    return m_childrens.takeFirst();
}

const CT_AbstractItem* CT_ChildIterator::current() const
{
    return m_childrens.first();
}
