#ifndef DM_ATTRIBUTESMANAGER_H
#define DM_ATTRIBUTESMANAGER_H

#include "tools/attributes/worker/abstract/dm_abstractattributes.h"

#include <QList>
#include <QThread>

class DM_AttributesManager
{
public:
    DM_AttributesManager();
    ~DM_AttributesManager();

    void addAttributes(DM_AbstractAttributes *att);
    void remove(const int &index);
    int size() const;
    void clearInvalid();

    DM_AbstractAttributes* getAttributesFromInterface(const CT_AbstractAttributes *ia) const;

    const QList<DM_AbstractAttributes*>& attributes() const;

private:
    QList<DM_AbstractAttributes*>   m_collection;
    QThread                         m_thread;
};

#endif // DM_ATTRIBUTESMANAGER_H
