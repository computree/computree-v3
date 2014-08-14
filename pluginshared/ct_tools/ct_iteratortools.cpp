#include "ct_iteratortools.h"

CT_IteratorTools::CT_IteratorTools()
{
    _list = NULL;
    _iterator = NULL;
    _active = false;
    _autoDelete = false;
}

CT_IteratorTools::CT_IteratorTools(QList<CT_AbstractItemDrawable*> &list,
                                   bool deleteItemDrawableFromMemoryWhenRemove)
{
    _list = NULL;
    _iterator = NULL;
    _active = false;

    setDeleteItemDrawableFromMemoryWhenRemove(deleteItemDrawableFromMemoryWhenRemove);

    setContainer(list);
}

CT_IteratorTools::~CT_IteratorTools()
{
    delete _iterator;
}

void CT_IteratorTools::setContainer(QList<CT_AbstractItemDrawable*> &list)
{
    _list = &list;

    if(_iterator == NULL)
        _iterator = new MyMutableItemDrawableListIterator(list);

    _active = false;
}

void CT_IteratorTools::setDeleteItemDrawableFromMemoryWhenRemove(bool value)
{
    _autoDelete = value;
}

bool CT_IteratorTools::deleteItemDrawableFromMemoryWhenRemove() const
{
    return _autoDelete;
}

bool CT_IteratorTools::beginIterateItemDrawable()
{
    _active = false;

    if(_list == NULL)
        return false;

    (*_iterator) = (*_list);

    _active = true;

    return true;
}

bool CT_IteratorTools::hasNext() const
{
    if(isActive())
        return _iterator->hasNext();

    return false;
}

CT_AbstractItemDrawable* CT_IteratorTools::nextItemDrawable()
{
    if(isActive())
    {
        if(_iterator->hasNext())
            return _iterator->next();
        else
            _active = false;

        return NULL;
    }

    return NULL;
}

CT_AbstractItemDrawable* CT_IteratorTools::currentItemDrawable()
{
    if(isActive())
        return _iterator->value();

    return NULL;
}

bool CT_IteratorTools::removeCurrentItemDrawable()
{
    if(isActive())
    {
        if(deleteItemDrawableFromMemoryWhenRemove())
            delete _iterator->value();

        _iterator->remove();

        return true;
    }

    return false;
}

bool CT_IteratorTools::removeItemDrawable(CT_AbstractItemDrawable *item)
{
    // si on est en train de parcourir les items
    if(isActive())
    {
        _iterator->remove(item);

        if(deleteItemDrawableFromMemoryWhenRemove())
            delete item;

        return true;
    }

    if(_list == NULL)
        return false;

    if(_list->removeOne(item))
    {
        if(deleteItemDrawableFromMemoryWhenRemove())
            delete item;

        return true;
    }

    return false;
}

bool CT_IteratorTools::removeItemDrawableWithoutAutoDelete(CT_AbstractItemDrawable *item)
{
    // si on est en train de parcourir les items
    if(isActive())
    {
        _iterator->remove(item);
        return true;
    }

    if(_list == NULL)
        return false;

    return _list->removeOne(item);
}

void CT_IteratorTools::itemDrawableWithThisIndexHasBeenRemoved(int index)
{
    if(isActive())
        _iterator->indexRemoved(index);
}

bool CT_IteratorTools::isActive() const
{
    return _active;
}
