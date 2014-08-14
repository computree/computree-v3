#include "ct_defaultitemattributemanager.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManager()
{
}

CT_DefaultItemAttributeManager::~CT_DefaultItemAttributeManager()
{
    qDeleteAll(m_collection.begin(), m_collection.end());
}

QList<CT_AbstractItemAttribute *> CT_DefaultItemAttributeManager::itemAttributes(const QString &itemType) const
{
    QList<CT_AbstractItemAttribute *> l;

    QStringList types = itemType.split("/");

    foreach (const QString &t, types) {
        QList<CT_DefaultItemAttributeManagerContainer*> container = m_collection.values(t);

        foreach (CT_DefaultItemAttributeManagerContainer *c, container) {
            l.append(c->m_model->itemAttribute());
        }
    }

    return l;
}
