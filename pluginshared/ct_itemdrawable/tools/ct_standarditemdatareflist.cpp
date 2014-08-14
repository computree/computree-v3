#include "ct_standarditemdatareflist.h"

CT_StandardItemDataRefList::CT_StandardItemDataRefList()
{
    _name = "Default";
}

CT_StandardItemDataRefList::CT_StandardItemDataRefList(const CT_StandardItemDataRefList &list)
{
    _name = list.name();
    _displayableName = list.displayableName();
    addReference(list);
}

CT_StandardItemDataRefList::~CT_StandardItemDataRefList()
{
    qDeleteAll(_references.begin(), _references.end());
}

void CT_StandardItemDataRefList::addReference(const CT_StandardItemDataRefList &list)
{
    QListIterator<CT_AbstractItemDataRef*> it(list._references);

    while(it.hasNext())
        addReference(it.next()->copy());
}

void CT_StandardItemDataRefList::addReference(CT_AbstractItemDataRef *ref)
{
    _references.append(ref);
    m_referencesByCommand.insert(ref->userType(), ref);
}

bool CT_StandardItemDataRefList::valueByCommand(const int &command, const CT_AbstractItemDrawable *itemdrawable, IItemDataValue *valueToSet) const
{
    CT_AbstractItemDataRef *ref = m_referencesByCommand.value(command, NULL);

    if((ref == NULL) || (ref->delegate() == NULL))
        return false;

    ref->delegate()->value(itemdrawable, valueToSet);
    return true;
}

void CT_StandardItemDataRefList::setName(QString name)
{
    _name = name;

    if(_displayableName.isEmpty())
        _displayableName = name;
}

void CT_StandardItemDataRefList::setDisplayableName(QString displayableName)
{
    _displayableName = displayableName;

    if(_displayableName.isEmpty())
        _displayableName = name();
}

QString CT_StandardItemDataRefList::name() const
{
    return _name;
}

QString CT_StandardItemDataRefList::displayableName() const
{
    return _displayableName;
}

const QList<IItemDataRef*>& CT_StandardItemDataRefList::references() const
{
    return (const QList<IItemDataRef*>&)_references;
}
