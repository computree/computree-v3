#include "ct_inabstractsingularitemmodel.h"
#include <assert.h>

CT_InAbstractSingularItemModel::CT_InAbstractSingularItemModel(QString uniqueName,
                                                               QString description,
                                                               QString displayableName) : CT_InAbstractModel(uniqueName,
                                                                                                             description,
                                                                                                             displayableName)
{

    m_itemType = "";
    m_itemShortType = "";
}

QString CT_InAbstractSingularItemModel::itemType() const
{
    return m_itemType;
}

QString CT_InAbstractSingularItemModel::itemShortType() const
{
    return m_itemShortType;
}

void CT_InAbstractSingularItemModel::setItemType(const QString &itemType)
{
    Q_ASSERT_X(!itemType.isEmpty(), "setItemType", "You pass a item type empty !");

    m_itemType = itemType;
    m_itemShortType = m_itemType.split("/").last();
}
