#include "ct_defaultitemattributemanager.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_abstractstepplugin.h"

CT_DefaultItemAttributeManager::CT_DefaultItemAttributeManager()
{
}

CT_DefaultItemAttributeManager::~CT_DefaultItemAttributeManager()
{
    QHashIterator<QString, QList<CT_DefaultItemAttributeManagerContainer*>* > it(m_collection);

    while(it.hasNext())
    {
        it.next();

        QList<CT_DefaultItemAttributeManagerContainer*> *l = it.value();

        foreach (CT_DefaultItemAttributeManagerContainer *c, (*l)) {
            delete c;
        }

        delete l;
    }
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

            QList<CT_DefaultItemAttributeManagerContainer*> *container = m_collection.value(t);

            if(container != NULL)
            {
                foreach (CT_DefaultItemAttributeManagerContainer *c, (*container)) {
                    l.append(c->m_model->itemAttribute());
                }
            }
        }
    }

    return l;
}

CT_AbstractItemAttribute *CT_DefaultItemAttributeManager::itemAttributeFromUniqueKey(const QString &uniqueKey, const QString &itemType) const
{
    QStringList types = itemType.split("/");

    QList<CT_DefaultItemAttributeManagerContainer*> *container = m_collection.value(types.last());

    if(container != NULL)
    {
        foreach (CT_DefaultItemAttributeManagerContainer *c, (*container)) {
            if(c->m_userKey == uniqueKey)
                return c->m_model->itemAttribute();
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

            QList<CT_DefaultItemAttributeManagerContainer*> *container = m_collection.value(t);

            if(container != NULL)
            {
                foreach (CT_DefaultItemAttributeManagerContainer *c, (*container)) {
                    if(c->m_model == orModel)
                        return c->m_model->itemAttribute();
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

                QList<CT_DefaultItemAttributeManagerContainer*> *container = m_collection.value(t);

                if(container != NULL)
                {
                    foreach (CT_DefaultItemAttributeManagerContainer *c, (*container)) {
                        if(c->m_model == orModel)
                            l.insert(c->m_model->itemAttribute());
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

                QList<CT_DefaultItemAttributeManagerContainer*> *container = m_collection.value(t);

                if(container != NULL)
                {
                    foreach (CT_DefaultItemAttributeManagerContainer *c, (*container)) {
                        if(c->m_model == orModel)
                            return c->m_model->itemAttribute();
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
                QList<CT_DefaultItemAttributeManagerContainer*> *l = m_collection.take(type);

                if(l != NULL) {
                    foreach (CT_DefaultItemAttributeManagerContainer *c, (*l))
                        delete c;

                    delete l;
                }
            }
        }
    }
}
