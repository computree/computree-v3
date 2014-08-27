#include "ct_inabstractgroupmodel.h"

#include "ct_itemdrawable/model/inModel/abstract/def_ct_abstractgroupmodelin.h"

CT_InAbstractGroupModel::CT_InAbstractGroupModel(const QString &uniqueName,
                                                 const QString &description,
                                                 const QString &displayableName) : DEF_CT_AbstractGroupModelIn(uniqueName,
                                                                                                               description,
                                                                                                               displayableName)
{
}

QString CT_InAbstractGroupModel::groupShortType() const
{
    return itemShortType();
}

QString CT_InAbstractGroupModel::groupType() const
{
    return itemType();
}

QList<SettingsNodeGroup*> CT_InAbstractGroupModel::getAllValues() const
{
    QList<SettingsNodeGroup*> retList = DEF_CT_AbstractGroupModelIn::getAllValues();

    SettingsNodeGroup *root = retList.first();

    SettingsNodeGroup *rootGroupModel = new SettingsNodeGroup("CT_InAbstractGroupModel");
    root->addGroup(rootGroupModel);

    rootGroupModel->addValue(new SettingsNodeValue("Version", 1));

    QListIterator< DEF_CT_AbstractGroupModelIn* > itG(groups());

    while(itG.hasNext())
    {
        SettingsNodeGroup *gModel = new SettingsNodeGroup("Group");
        QList<SettingsNodeGroup*> values = itG.next()->getAllValues();

        while(!values.isEmpty())
            gModel->addGroup(values.takeFirst());

        rootGroupModel->addGroup(gModel);
    }

    QListIterator<CT_InAbstractSingularItemModel*> itI(items());

    while(itI.hasNext())
    {
        SettingsNodeGroup *iModel = new SettingsNodeGroup("Item");
        QList<SettingsNodeGroup*> values = itI.next()->getAllValues();

        while(!values.isEmpty())
            iModel->addGroup(values.takeFirst());

        rootGroupModel->addGroup(iModel);
    }

    return retList;
}

bool CT_InAbstractGroupModel::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if(!DEF_CT_AbstractGroupModelIn::setAllValues(list))
        return false;

    QList<SettingsNodeGroup*> rootGroupModel = list.first()->groupsByTagName("CT_InAbstractGroupModel");

    if(rootGroupModel.isEmpty())
        return false;

    QList<SettingsNodeGroup*> groupsS = rootGroupModel.first()->groupsByTagName("Group");

    if(groupsS.size() != groups().size())
        return false;

    QList<SettingsNodeGroup*> itemsS = rootGroupModel.first()->groupsByTagName("Item");

    if(itemsS.size() != items().size())
        return false;

    int i=0;
    QListIterator< DEF_CT_AbstractGroupModelIn* > itG(groups());

    while(itG.hasNext())
    {
        if(!itG.next()->setAllValues(groupsS.at(i)->groups()))
            return false;

        ++i;
    }

    i=0;

    QListIterator<CT_InAbstractSingularItemModel*> itI(items());

    while(itI.hasNext())
    {
        if(!itI.next()->setAllValues(itemsS.at(i)->groups()))
            return false;

        ++i;
    }

    return true;
}

void CT_InAbstractGroupModel::setGroupType(const QString &groupType)
{
    setItemType(groupType);
}
