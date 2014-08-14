#include "ct_itemattributecontainer.h"

CT_ItemAttributeContainer::CT_ItemAttributeContainer()
{
}

CT_ItemAttributeContainer::~CT_ItemAttributeContainer()
{
    clear();
}

void CT_ItemAttributeContainer::addItemAttribute(CT_AbstractItemAttribute *att)
{
    QList<CT_AbstractItemAttribute *> *l = m_attributes.value(att->result(), NULL);

    if(l == NULL)
    {
        l = new QList<CT_AbstractItemAttribute *>();
        m_attributes.insert(att->result(), l);
    }

    l->append(att);
}

void CT_ItemAttributeContainer::removeItemAttribute(CT_AbstractItemAttribute *att)
{
    CT_AbstractResult *res = att->result();
    QList<CT_AbstractItemAttribute *> *l = m_attributes.value(res, NULL);

    if(l != NULL)
    {
        if(l->removeOne(att))
            delete att;

        if(l->isEmpty())
        {
            m_attributes.remove(res);
            delete l;
        }
    }
}

void CT_ItemAttributeContainer::removeItemAttributeFromResult(CT_AbstractResult *result)
{
    if(m_attributes.contains(result))
    {
        QList<CT_AbstractItemAttribute *> *l = m_attributes.take(result);

        qDeleteAll(l->begin(), l->end());
        delete l;
    }
}

QList<CT_AbstractItemAttribute *> CT_ItemAttributeContainer::itemAttributes() const
{
    QList<CT_AbstractItemAttribute*> ret;

    QList< QList<CT_AbstractItemAttribute*>* > l = m_attributes.values();
    QListIterator< QList<CT_AbstractItemAttribute*>* > it(l);

    while(it.hasNext())
        ret.append(*it.next());

    return ret;
}

QList<CT_AbstractItemAttribute *> CT_ItemAttributeContainer::itemAttributesFromResult(const CT_AbstractResult *result) const
{
    QList<CT_AbstractItemAttribute *> *l = m_attributes.value((CT_AbstractResult*)result, NULL);

    if(l == NULL)
        return QList<CT_AbstractItemAttribute *>();

    return *l;
}

void CT_ItemAttributeContainer::clear()
{
    QHashIterator<CT_AbstractResult*, QList<CT_AbstractItemAttribute *>* > it(m_attributes);

    while(it.hasNext())
    {
        it.next();
        QList<CT_AbstractItemAttribute *> *l = it.value();
        qDeleteAll(l->begin(), l->end());
        delete l;
    }

    m_attributes.clear();
}
