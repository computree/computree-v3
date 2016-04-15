#include "ct_defaultitemattributemanager.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_abstractstepplugin.h"

#include <QDebug>

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
    bool ok = false;

    foreach (const QString &t, types) {

        if(ok || (t == "CT_AbstractSingularItemDrawable"))
        {
            ok = true;

            CT_DefaultItemAttributeManagerContainerList *container = m_collection.value(t, NULL);

            if(container != NULL)
            {
                QListIterator<CT_DefaultItemAttributeManagerContainer*> itC(container->list());

                while(itC.hasNext())
                    l.append(itC.next()->m_model->itemAttribute());
            }
        }
    }

    return l;
}

CT_AbstractItemAttribute *CT_DefaultItemAttributeManager::itemAttributeFromUniqueKey(const QString &uniqueKey, const QString &itemType) const
{
    QStringList types = itemType.split("/");

    CT_DefaultItemAttributeManagerContainerList *container = m_collection.value(types.last());

    if(container != NULL)
    {
        QListIterator<CT_DefaultItemAttributeManagerContainer*> itC(container->list());

        while(itC.hasNext()) {
            CT_DefaultItemAttributeManagerContainer *cc = itC.next();
            if(cc->m_userKey == uniqueKey)
                return cc->m_model->itemAttribute();
        }
    }

    return NULL;
}

CT_AbstractItemAttribute* CT_DefaultItemAttributeManager::itemAttributeFromModel(const CT_OutAbstractItemAttributeModel *outModel, const QString &itemType) const
{
    if(!outModel->isADefaultItemAttributeModel())
        return NULL;

    CT_OutAbstractModel *orModel = outModel->originalModel();

    // TODO : test performance en utilisant le type et sans utiliser le type (juste parcourir toute la map)
    QStringList types = itemType.split("/");
    bool ok = false;

    foreach (const QString &t, types) {

        if(ok || (t == "CT_AbstractSingularItemDrawable"))
        {
            ok = true;

            CT_DefaultItemAttributeManagerContainerList *container = m_collection.value(t);

            if(container != NULL)
            {
                QListIterator<CT_DefaultItemAttributeManagerContainer*> itC(container->list());

                while(itC.hasNext()) {
                    CT_DefaultItemAttributeManagerContainer *cc = itC.next();
                    if(cc->m_model == orModel)
                        return cc->m_model->itemAttribute();
                }
            }
        }
    }

    return NULL;
}

QList<CT_AbstractItemAttribute*> CT_DefaultItemAttributeManager::itemAttributesFromModel(const CT_InAbstractItemAttributeModel *inModel, const QString &itemType) const
{
    QStringList types = itemType.split("/");

    QSet<CT_AbstractItemAttribute*> l;

    QList<CT_InStdModelPossibility*> p = inModel->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> itP(p);

    while(itP.hasNext())
    {
        CT_OutAbstractModel *orModel = itP.next()->outModel()->originalModel();
        bool ok = false;

        foreach (const QString &t, types) {

            if(ok || (t == "CT_AbstractSingularItemDrawable"))
            {
                ok = true;

                CT_DefaultItemAttributeManagerContainerList *container = m_collection.value(t);

                if(container != NULL)
                {
                    QListIterator<CT_DefaultItemAttributeManagerContainer*> itC(container->list());

                    while(itC.hasNext()) {
                        CT_DefaultItemAttributeManagerContainer *cc = itC.next();
                        if(cc->m_model == orModel)
                            l.insert(cc->m_model->itemAttribute());
                    }
                }
            }
        }
    }

    return l.toList();
}

CT_AbstractItemAttribute* CT_DefaultItemAttributeManager::firstItemAttributeFromModel(const CT_InAbstractItemAttributeModel *inModel, const QString &itemType) const
{
    QStringList types = itemType.split("/");

    QList<CT_InStdModelPossibility*> p = inModel->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> itP(p);

    while(itP.hasNext())
    {
        CT_OutAbstractModel *orModel = itP.next()->outModel()->originalModel();
        bool ok = false;

        foreach (const QString &t, types) {

            if(ok || (t == "CT_AbstractSingularItemDrawable"))
            {
                ok = true;

                CT_DefaultItemAttributeManagerContainerList *container = m_collection.value(t);

                if(container != NULL)
                {
                    QListIterator<CT_DefaultItemAttributeManagerContainer*> itC(container->list());

                    while(itC.hasNext()) {
                        CT_DefaultItemAttributeManagerContainer *cc = itC.next();
                        if(cc->m_model == orModel)
                            return cc->m_model->itemAttribute();
                    }
                }
            }
        }
    }

    return NULL;
}

void CT_DefaultItemAttributeManager::clearDefaultAttributesFromPlugin(CT_AbstractStepPlugin *p)
{
    QList<CT_AbstractItemDrawable*> items = p->getItemDrawablesAvailable();

    foreach (CT_AbstractItemDrawable *item, items) {

        QStringList types = item->getType().split("/");

        foreach (QString type, types) {

            if(!type.startsWith("CT_") && (type != "ItemDrawable")) {
                delete m_collection.take(type);
            }
        }
    }
}

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainerList::CT_DefaultItemAttributeManagerContainerList()
{
}

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainerList::~CT_DefaultItemAttributeManagerContainerList()
{
    qDeleteAll(m_list.begin(), m_list.end());
}

void CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainerList::add(const QString &className, CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainer *c)
{
    Q_UNUSED(className)

    m_list.append(c);
}

const QList<CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainer *> &CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainerList::list() const
{
    return m_list;
}

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainer::CT_DefaultItemAttributeManagerContainer()
{
    m_model = NULL;
}

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManagerContainer::~CT_DefaultItemAttributeManagerContainer()
{
    delete m_model;
}
