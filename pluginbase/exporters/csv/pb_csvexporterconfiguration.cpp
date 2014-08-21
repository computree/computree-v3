#include "pb_csvexporterconfiguration.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

PB_CSVExporterConfiguration::PB_CSVExporterConfiguration(const QList<CT_OutAbstractSingularItemModel *> &list)
{
    _list = list;
}

void PB_CSVExporterConfiguration::setList(const QList<CT_OutAbstractSingularItemModel *> &list)
{
    _list = list;
}

const QList<CT_OutAbstractSingularItemModel *> &PB_CSVExporterConfiguration::list() const
{
    return _list;
}

void PB_CSVExporterConfiguration::addColumn(const CT_OutAbstractSingularItemModel *sItem, const CT_OutAbstractItemAttributeModel *ia)
{
    _columns.append(qMakePair((CT_OutAbstractSingularItemModel*)sItem, (CT_OutAbstractItemAttributeModel*)ia));
}

const QList<QPair<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> > &PB_CSVExporterConfiguration::getColumns() const
{
    return _columns;
}

void PB_CSVExporterConfiguration::clearColumns()
{
    _columns.clear();
}
