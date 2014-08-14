#ifndef CT_STANDARDITEMDATAREFLIST_H
#define CT_STANDARDITEMDATAREFLIST_H

#include "ct_itemdrawable/tools/ct_abstractitemdataref.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardItemDataRefList : public IItemDataRefList
{
    Q_INTERFACES(IItemDataRefList)

public:
    CT_StandardItemDataRefList();
    CT_StandardItemDataRefList(const CT_StandardItemDataRefList &list);
    ~CT_StandardItemDataRefList();

    void setName(QString name);
    void setDisplayableName(QString displayableName);

    void addReference(const CT_StandardItemDataRefList &list);
    void addReference(CT_AbstractItemDataRef *ref);

    bool valueByCommand(const int &command, const CT_AbstractItemDrawable *itemdrawable, IItemDataValue *valueToSet) const;

    QString name() const;
    QString displayableName() const;

    const QList<IItemDataRef*>& references() const;

private:
    QList<CT_AbstractItemDataRef*>      _references;
    QMap<int, CT_AbstractItemDataRef*>  m_referencesByCommand;
    QString                             _name;
    QString                             _displayableName;
};

#endif // CT_STANDARDITEMDATAREFLIST_H
