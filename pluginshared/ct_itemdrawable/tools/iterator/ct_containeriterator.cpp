#include "ct_containeriterator.h"

#include "ct_itemdrawable/ct_container.h"

CT_ContainerIterator::CT_ContainerIterator(const CT_Container *container)
{
    m_iterator = new QListIterator<CT_AbstractItemDrawable*>(*container->getList());
    m_current = NULL;
}

CT_ContainerIterator::~CT_ContainerIterator()
{
    delete m_iterator;
}
