#ifndef PB_CSVEXPORTERCOLUMN_H
#define PB_CSVEXPORTERCOLUMN_H

#include "interfaces.h"

class PB_CSVExporterColumn
{
public:
    PB_CSVExporterColumn(const QList<ItemDrawable*> &items, const IItemDataRef &ref);
    ~PB_CSVExporterColumn();

    int currentItemIndex() const;

    bool hasNextValue() const;
    QString nextValueToString();

    size_t size() const;

private:
    QList<ItemDrawable*>            _items;
    const IItemDataRef              *_ref;

    QListIterator<ItemDrawable*>    *_iterator;

    ItemDrawable                    *_currentItem;
    int                             _currentItemIndex;
    size_t                          _currentArrayIndex;
    size_t                          _currentArraySize;
    size_t                          _currentArraySecondIndex;
    bool                            _hasNext;

    IItemDataValue                  *_currentValue;

    void nextItem();

    bool isDataValueType2DArray() const;
    bool isDataValueType3DArray() const;
};

#endif // PB_CSVEXPORTERCOLUMN_H
