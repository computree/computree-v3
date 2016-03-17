#include "ct_abstractmetricgeneric.h"

CT_AbstractMetricGeneric::~CT_AbstractMetricGeneric()
{
    qDeleteAll(m_attributesVaB.begin(), m_attributesVaB.end());
}

SettingsNodeGroup *CT_AbstractMetricGeneric::getAllSettings() const
{
    SettingsNodeGroup *root = new SettingsNodeGroup("CT_AbstractMetricGeneric");

    QListIterator<CT_AbstractVaB*> it(m_vab);

    while(it.hasNext()) {
        CT_AbstractVaB *vab = it.next();

        SettingsNodeGroup *v = new SettingsNodeGroup("VaB");
        v->addValue(new SettingsNodeValue("used", vab->used));

        root->addGroup(v);
    }

    return root;
}

bool CT_AbstractMetricGeneric::setAllSettings(const SettingsNodeGroup *settings)
{
    if(settings == NULL)
        return false;

    if(settings->name() == "CT_AbstractMetricGeneric")
    {
        QList<SettingsNodeGroup*> groups = settings->groupsByTagName("VaB");

        if(groups.size() != m_vab.size())
            return false;

        int i = 0;
        QListIterator<CT_AbstractVaB*> it(m_vab);

        while(it.hasNext()) {
            SettingsNodeGroup *v = groups.at(i);
            CT_AbstractVaB *vab = it.next();

            SettingsNodeValue *usedV = v->firstValueByTagName("used");

            if(usedV == NULL)
                return false;

            vab->used = usedV->value().toBool();
        }

        return true;
    }

    return false;
}

CT_AbstractConfigurableWidget* CT_AbstractMetricGeneric::createConfigurationWidget()
{
    if(m_vab.isEmpty())
        return NULL;

    CT_GenericConfigurableWidget *wid = new CT_GenericConfigurableWidget();

    QListIterator<CT_AbstractVaB*> it(m_vab);

    while(it.hasNext()) {
        CT_AbstractVaB *vab = it.next();
        wid->addBool("", "", m_attributesVaB.value(vab, NULL)->_displayableName, vab->used);
    }

    return wid;
}

void CT_AbstractMetricGeneric::createAttributes()
{
    QListIterator<CT_AbstractVaB*> it(m_vab);

    while(it.hasNext()) {
        CT_AbstractVaB *vab = it.next();

        if(vab->used)
            internalInsertAttributeObject(vab->valueToVoidPtr(), m_attributesVaB.value(vab, NULL)->copy());
    }
}
