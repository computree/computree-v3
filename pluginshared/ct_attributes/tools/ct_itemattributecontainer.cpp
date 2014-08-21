#include "ct_itemattributecontainer.h"

#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

CT_ItemAttributeContainer::CT_ItemAttributeContainer()
{
}

CT_ItemAttributeContainer::~CT_ItemAttributeContainer()
{
    clear();
}

bool CT_ItemAttributeContainer::addItemAttribute(CT_AbstractItemAttribute *att)
{
    QList<CT_AbstractItemAttribute *> *l = m_attributes.value(att->result(), NULL);

    if(l == NULL)
    {
        l = new QList<CT_AbstractItemAttribute *>();
        m_attributes.insert(att->result(), l);
    }

    l->append(att);
    return true;
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

void CT_ItemAttributeContainer::removeItemAttributeFromResult(const CT_AbstractResult *result)
{
    QList<CT_AbstractItemAttribute *> *l = m_attributes.take((CT_AbstractResult*)result);

    if(l != NULL)
    {
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

CT_AbstractItemAttribute* CT_ItemAttributeContainer::itemAttributeFromModel(const CT_OutAbstractItemAttributeModel *outModel) const
{
    QString uName = outModel->uniqueName();
    QHashIterator<CT_AbstractResult*, QList<CT_AbstractItemAttribute*>*> it(m_attributes);

    while(it.hasNext())
    {
        it.next();

        QListIterator<CT_AbstractItemAttribute*> itL(*it.value());

        while(itL.hasNext())
        {
            CT_AbstractItemAttribute *att = itL.next();

            if(att->model()->uniqueName() == uName)
                return att;
        }
    }

    return NULL;
}

QList<CT_AbstractItemAttribute*> CT_ItemAttributeContainer::itemAttributesFromModel(const CT_InAbstractItemAttributeModel *inModel) const
{
    QList<CT_AbstractItemAttribute*> l;
    QHashIterator<CT_AbstractResult*, QList<CT_AbstractItemAttribute*>*> it(m_attributes);

    QList<CT_InStdModelPossibility*> p = inModel->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> itP(p);

    while(itP.hasNext())
    {
        QString uName = itP.next()->outModel()->uniqueName();

        it.toFront();

        while(it.hasNext())
        {
            it.next();

            QListIterator<CT_AbstractItemAttribute*> itL(*it.value());

            while(itL.hasNext())
            {
                CT_AbstractItemAttribute *att = itL.next();

                if(att->model()->uniqueName() == uName)
                    l.append(att);
            }
        }
    }

    return l;
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
