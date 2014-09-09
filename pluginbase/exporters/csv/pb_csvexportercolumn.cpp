#include "pb_csvexportercolumn.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

PB_CSVExporterColumn::PB_CSVExporterColumn(const QList<CT_AbstractSingularItemDrawable *> &items, const CT_OutAbstractItemAttributeModel *iaModel)
{
    _items = items;
    _ref = (CT_OutAbstractItemAttributeModel*)iaModel;

    m_currentItemAttribut = NULL;
    _currentItem = NULL;
    _hasNext = false;
    _currentItemIndex = -1;

    _iterator = new QListIterator<CT_AbstractSingularItemDrawable*>(_items);

    nextItem();
}

PB_CSVExporterColumn::~PB_CSVExporterColumn()
{
    delete _iterator;
}

int PB_CSVExporterColumn::currentItemIndex() const
{
    return _currentItemIndex;
}

bool PB_CSVExporterColumn::hasNextValue() const
{
    return _hasNext;
}

QString PB_CSVExporterColumn::nextValueToString()
{
    QString next;

    next = m_currentItemAttribut->toString(_currentItem, NULL);

    // on passe à l'item suivant
    nextItem();

    return next;
}

size_t PB_CSVExporterColumn::size() const
{
    size_t s = 0;

    _iterator->toFront();

    while(_iterator->hasNext())
    {
        if(_iterator->next()->itemAttribute(_ref) != NULL)
            ++s;
    }

    if(_currentItem != NULL)
        _iterator->findNext(_currentItem);
    else
        _iterator->toFront();

    return s;
}

void PB_CSVExporterColumn::nextItem()
{
    if(_iterator->hasNext())
    {
        do
        {
            _currentItem = _iterator->next();
            m_currentItemAttribut = _currentItem->itemAttribute(_ref);

            ++_currentItemIndex;

            _hasNext = (m_currentItemAttribut != NULL);

        }while(!_hasNext
               && _iterator->hasNext());
    }
    else
    {
        m_currentItemAttribut = NULL;
        _currentItem = NULL;
        _hasNext = false;
        ++_currentItemIndex;
    }
}
