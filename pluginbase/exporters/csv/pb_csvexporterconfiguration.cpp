#include "pb_csvexporterconfiguration.h"

#include "interfaces.h"

PB_CSVExporterConfiguration::PB_CSVExporterConfiguration(const QList<const IItemDataRefList *> &list)
{
    _list = list;
}

void PB_CSVExporterConfiguration::setList(const QList<const IItemDataRefList*> &list)
{
    _list = list;
}

void PB_CSVExporterConfiguration::addColumn(const IItemDataRefList *refList, IItemDataRef *ref)
{
    _columns.append(qMakePair(refList, (const IItemDataRef*)ref));
}

const QList< QPair<const IItemDataRefList*, const IItemDataRef*> >& PB_CSVExporterConfiguration::getColumns() const
{
    return _columns;
}

void PB_CSVExporterConfiguration::clearColumns()
{
    _columns.clear();
}
