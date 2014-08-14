#ifndef CT_STANDARDSTANDARDITEMGROUPDRAWMANAGER_H
#define CT_STANDARDSTANDARDITEMGROUPDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class CT_AbstractStandardItemGroup;
class CT_Container;
class CT_AbstractSingularItemDrawable;
class CT_GroupItemDrawableContainer;

class PLUGINSHAREDSHARED_EXPORT CT_StandardStandardItemGroupDrawManager : public CT_AbstractItemDrawableDrawManager
{
public:
    CT_StandardStandardItemGroupDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardStandardItemGroupDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_ITEMS_BACKUP_VISIBLE;
    const static QString INDEX_CONFIG_ITEMS_NEW_VISIBLE;

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

    void drawGroups(const QHash<QString, CT_Container*> &map, GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractStandardItemGroup &sig) const;
    void drawItems(const QHash<QString, CT_GroupItemDrawableContainer*> &map, GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractStandardItemGroup &sig) const;
};

#endif // CT_STANDARDSTANDARDITEMGROUPDRAWMANAGER_H
