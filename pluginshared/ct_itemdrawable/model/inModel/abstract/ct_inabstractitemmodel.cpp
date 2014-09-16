#include "ct_inabstractitemmodel.h"

CT_InAbstractItemModel::CT_InAbstractItemModel(const QString &uniqueName,
                                               const QString &description,
                                               const QString &displayableName) : CT_InAbstractModel(uniqueName,
                                                                                                    description,
                                                                                                    displayableName)
{
    m_itemType = "";
    m_itemShortType = "";
}

QString CT_InAbstractItemModel::itemType() const
{
    return m_itemType;
}

QString CT_InAbstractItemModel::itemShortType() const
{
    return m_itemShortType;
}

QList<SettingsNodeGroup *> CT_InAbstractItemModel::getAllValues() const
{
    QList<SettingsNodeGroup*> retList = CT_InAbstractModel::getAllValues();

    SettingsNodeGroup *root = retList.first();

    SettingsNodeGroup *rootGroupModel = new SettingsNodeGroup("CT_InAbstractItemModel");
    root->addGroup(rootGroupModel);

    rootGroupModel->addValue(new SettingsNodeValue("Version", 1));
    rootGroupModel->addValue(new SettingsNodeValue("ItemType", itemType()));

    return retList;
}

bool CT_InAbstractItemModel::setAllValues(const QList<SettingsNodeGroup *> &list)
{
    QList<SettingsNodeGroup*> rootItemModel = list.first()->groupsByTagName("CT_InAbstractItemModel");

    if(rootItemModel.isEmpty())
        return false;

    QList<SettingsNodeValue*> values = rootItemModel.first()->valuesByTagName("ItemType");

    if(values.isEmpty())
        return false;

    if(values.first()->value().toString() != itemType())
        return false;

    return CT_InAbstractModel::setAllValues(list);
}

void CT_InAbstractItemModel::setItemType(const QString &itemType)
{
    Q_ASSERT_X(!itemType.isEmpty(), "setItemType", "You pass a item type empty !");

    m_itemType = itemType;
    m_itemShortType = m_itemType.split("/").last();
}

