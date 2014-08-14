#include "pb_csvexportercolumn.h"

#include "ct_global/ct_context.h"

PB_CSVExporterColumn::PB_CSVExporterColumn(const QList<ItemDrawable*> &items, const IItemDataRef &ref)
{
    _items = items;
    _ref = &ref;

    _currentItem = NULL;
    _hasNext = false;
    _currentArrayIndex = 0;
    _currentArraySize = 0;
    _currentItemIndex = -1;
    _currentArraySecondIndex = 0;

    _currentValue = PS_REPOSITORY->createNewEmptyDataValue();

    _iterator = new QListIterator<ItemDrawable*>(_items);

    nextItem();
}

PB_CSVExporterColumn::~PB_CSVExporterColumn()
{
    delete _currentValue;
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

    // si c'est un tableau 2D
    if(isDataValueType2DArray())
    {
        // on retourne comme valeur suivant tous les éléments de la ligne Y

        // on récupère la taille d'une ligne du tableau
        size_t col = _currentValue->otherSize(1);

        for(size_t i=0; i<col; ++i)
        {
            next += _currentValue->valueAtIndexToString(_currentArrayIndex, NULL);

            if(i < (col-1))
                next += ";";

            ++_currentArrayIndex;
        }
    }
    // si c'est un tableau 3D
    else if(isDataValueType3DArray())
    {
        // on retourne comme valeur suivant tous les éléments de la ligne Y

        // on récupère la taille X du tableau
        size_t x = _currentValue->otherSize(1);
        // on récupère la taille Y du tableau
        size_t y = _currentValue->otherSize(2)-1;

        // si on a ecrit toutes les lignes et qu'on passe au Y suivant
        if(_currentArraySecondIndex == y)
        {
            _currentArraySecondIndex = 0;

            // on ne renvoi rien car on saute une ligne
        }
        // sinon
        else
        {
            // on écrit une ligne
            for(size_t i=0; i<x; ++i)
            {
                next += _currentValue->valueAtIndexToString(_currentArrayIndex, NULL);

                if(i < (x-1))
                    next += ";";

                ++_currentArrayIndex;
            }

            ++_currentArraySecondIndex;
        }
    }
    else
    {
        next = _currentValue->valueAtIndexToString(_currentArrayIndex, NULL);
        ++_currentArrayIndex;
    }

    // si il n'y a plus d'élément suivant dans la liste
    if(_currentArrayIndex >= _currentArraySize)
    {
        // on passe à l'item suivant
        nextItem();
    }

    return next;
}

size_t PB_CSVExporterColumn::size() const
{
    size_t s = 0;

    _iterator->toFront();

    while(_iterator->hasNext())
    {
        _iterator->next()->dataValueFromRef(*_ref, _currentValue);

        s += _currentValue->size();
    }

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
            _currentItem->dataValueFromRef(*_ref, _currentValue);

            ++_currentItemIndex;

            _currentArrayIndex = 0;
            _currentArraySecondIndex = 0;
            _currentArraySize = _currentValue->size();

            _hasNext = (_currentArrayIndex < _currentArraySize);

        }while(!_hasNext
               && _iterator->hasNext());
    }
    else
    {
        _currentItem = NULL;
        _currentArrayIndex = 0;
        _currentArraySecondIndex = 0;
        _hasNext = false;
        ++_currentItemIndex;
    }
}

bool PB_CSVExporterColumn::isDataValueType2DArray() const
{
    return (_currentValue->type() >= IItemDataValue::IDVT_BOOL_2D_ARRAY)
            && (_currentValue->type() < IItemDataValue::IDVT_UNKNOWN_2D_ARRAY);
}

bool PB_CSVExporterColumn::isDataValueType3DArray() const
{
    return (_currentValue->type() >= IItemDataValue::IDVT_BOOL_3D_ARRAY)
            && (_currentValue->type() < IItemDataValue::IDVT_UNKNOWN_3D_ARRAY);
}
