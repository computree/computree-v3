#ifndef PB_CSVEXPORTERCOLUMN_H
#define PB_CSVEXPORTERCOLUMN_H

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

class PB_CSVExporterColumn
{
public:
    PB_CSVExporterColumn(const QList<CT_AbstractSingularItemDrawable*> &items, const CT_OutAbstractItemAttributeModel *iaModel);
    ~PB_CSVExporterColumn();

    int currentItemIndex() const;

    bool hasNextValue() const;
    QString nextValueToString();

    size_t size() const;

private:
    QList<CT_AbstractSingularItemDrawable*>         _items;
    CT_OutAbstractItemAttributeModel                *_ref;

    QListIterator<CT_AbstractSingularItemDrawable*> *_iterator;

    CT_AbstractSingularItemDrawable         *_currentItem;
    CT_AbstractItemAttribute                *m_currentItemAttribut;

    int                                     _currentItemIndex;
    bool                                    _hasNext;

    void nextItem();
};

#endif // PB_CSVEXPORTERCOLUMN_H
