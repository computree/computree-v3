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

void CT_InAbstractItemModel::setItemType(const QString &itemType)
{
    Q_ASSERT_X(!itemType.isEmpty(), "setItemType", "You pass a item type empty !");

    m_itemType = itemType;
    m_itemShortType = m_itemType.split("/").last();
}

