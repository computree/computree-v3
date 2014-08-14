#ifndef PB_CSVEXPORTERCONFIGURATION_H
#define PB_CSVEXPORTERCONFIGURATION_H

#include <QList>
#include <QPair>

class IItemDataRefList;
class IItemDataRef;

class PB_CSVExporterConfiguration
{
public:
    PB_CSVExporterConfiguration(const QList<const IItemDataRefList*> &list);

    void setList(const QList<const IItemDataRefList*> &list);

    void addColumn(const IItemDataRefList *refList, IItemDataRef *ref);

    const QList< QPair<const IItemDataRefList*, const IItemDataRef*> >& getColumns() const;

    void clearColumns();

private:

    QList<const IItemDataRefList *>                                 _list;
    QList< QPair<const IItemDataRefList*, const IItemDataRef*> >    _columns;
};

#endif // PB_CSVEXPORTERCONFIGURATION_H
