#ifndef PB_CSVEXPORTERCONFIGURATION_H
#define PB_CSVEXPORTERCONFIGURATION_H

#include <QList>
#include <QPair>

class CT_OutAbstractSingularItemModel;
class CT_OutAbstractItemAttributeModel;

class PB_CSVExporterConfiguration
{
public:
    PB_CSVExporterConfiguration(const QList<CT_OutAbstractSingularItemModel*> &list);

    void setList(const QList<CT_OutAbstractSingularItemModel*> &list);
    const QList<CT_OutAbstractSingularItemModel*>& list() const;

    void addColumn(const CT_OutAbstractSingularItemModel *sItem, const CT_OutAbstractItemAttributeModel *ia);

    const QList< QPair<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*> >& getColumns() const;

    void clearColumns();

private:

    QList<CT_OutAbstractSingularItemModel*>                                                 _list;
    QList< QPair<CT_OutAbstractSingularItemModel*, CT_OutAbstractItemAttributeModel*> >     _columns;
};

#endif // PB_CSVEXPORTERCONFIGURATION_H
