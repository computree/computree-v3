#include "ct_standardstandarditemgroupdrawmanager.h"

#include "ct_abstractstandarditemgroup.h"

const QString CT_StandardStandardItemGroupDrawManager::INDEX_CONFIG_ITEMS_BACKUP_VISIBLE = "IBV";
const QString CT_StandardStandardItemGroupDrawManager::INDEX_CONFIG_ITEMS_NEW_VISIBLE = "INV";

CT_StandardStandardItemGroupDrawManager::CT_StandardStandardItemGroupDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_StandardItemGroup" : drawConfigurationName)
{
    
}

CT_StandardStandardItemGroupDrawManager::~CT_StandardStandardItemGroupDrawManager()
{
}

void CT_StandardStandardItemGroupDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_AbstractStandardItemGroup &item = dynamic_cast<const CT_AbstractStandardItemGroup&>(itemDrawable);

    painter.enableSetColor(false);

    drawGroups(item.constGroupsNew(), view, painter, item);
    drawGroups(item.constGroupsBackup(), view, painter, item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_ITEMS_NEW_VISIBLE).toBool())
        drawItems(item.constItemsNew(), view, painter, item);

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_ITEMS_BACKUP_VISIBLE).toBool())
        drawItems(item.constItemsBackup(), view, painter, item);

    painter.enableSetColor(true);
}

CT_ItemDrawableConfiguration CT_StandardStandardItemGroupDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_AbstractItemDrawableDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(INDEX_CONFIG_ITEMS_BACKUP_VISIBLE,"Anciens Items", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(INDEX_CONFIG_ITEMS_NEW_VISIBLE,"Nouveaux Items", CT_ItemDrawableConfiguration::Bool, true);

    return item;
}

void CT_StandardStandardItemGroupDrawManager::drawGroups(const QHash<QString, CT_Container *> &map, GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractStandardItemGroup &sig) const
{
    Q_UNUSED(sig)

    QHashIterator<QString, CT_Container*> itG(map);

    while(itG.hasNext())
    {
        itG.next();

        itG.value()->draw(view, painter);
    }
}

void CT_StandardStandardItemGroupDrawManager::drawItems(const QHash<QString, CT_GroupItemDrawableContainer *> &map, GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractStandardItemGroup &sig) const
{
    Q_UNUSED(sig)

    QHashIterator<QString, CT_GroupItemDrawableContainer*> itI(map);

    while(itI.hasNext())
    {
        itI.next();

        itI.value()->item()->draw(view, painter);
    }
}
