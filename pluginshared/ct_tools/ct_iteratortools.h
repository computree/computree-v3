#ifndef CT_ITERATORTOOLS_H
#define CT_ITERATORTOOLS_H

#include "pluginShared_global.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

#include <QMutableListIterator>

class MyMutableItemDrawableListIterator
{

private:
    QList<CT_AbstractItemDrawable*> *c;
    QList<CT_AbstractItemDrawable*>::const_iterator i, n;

public:
    inline MyMutableItemDrawableListIterator(QList<CT_AbstractItemDrawable*> &container)
        : c(&container)
    {
        i = c->constBegin();
        n = c->constEnd();
    }

    inline MyMutableItemDrawableListIterator &operator=(QList<CT_AbstractItemDrawable*> &container)
    {
        c = &container;
        i = c->constBegin();
        n = c->constEnd();
        return *this;
    }

    inline bool hasNext() const { return c->constEnd() != i; }
    inline CT_AbstractItemDrawable* next() { n = i++; return *n; }

    inline CT_AbstractItemDrawable* value() { if(n == c->constEnd()) { return NULL; } return *n; }

    inline void remove()
    {
        QList<CT_AbstractItemDrawable*>::iterator tmp = c->begin() + (n-c->constBegin());
        i = QList<CT_AbstractItemDrawable*>::const_iterator(c->erase(tmp));
        n = c->constEnd();
    }

    inline void remove(CT_AbstractItemDrawable *item)
    {
        // si l'item a supprimer est celui ou l'on se trouve
        if(item == value())
        {
            // on appel simplement remove()
            remove();
            return;
        }

        // sinon

        // on récupère l'index de l'item
        int index = c->indexOf(item);

        // on calcul l'index courant
        int currentIndex = n-c->constBegin();

        // on supprime l'item
        c->removeAt(index);

        // si l'item qu'on a supprimé était avant l'index courant
        if(index < currentIndex)
            --currentIndex; // on se decale d'un item

        n = c->constBegin()+currentIndex;
        i = n+1;
    }

    inline void indexRemoved(int index)
    {
        // on calcul l'index courant
        int currentIndex = n-c->constBegin();

        // si l'index qui a été supprimé est l'index courant
        if(index == currentIndex)
        {
            // on recalcul l'itérateur
            n = c->constBegin()+index;
            i = n;
        }
    }
};

class PLUGINSHAREDSHARED_EXPORT CT_IteratorTools
{
public:
    CT_IteratorTools();
    CT_IteratorTools(QList<CT_AbstractItemDrawable*> &list,
                     bool deleteItemDrawableFromMemoryWhenRemove);
    ~CT_IteratorTools();

    void setContainer(QList<CT_AbstractItemDrawable*> &list);

    void setDeleteItemDrawableFromMemoryWhenRemove(bool value);
    inline bool deleteItemDrawableFromMemoryWhenRemove() const;

    bool beginIterateItemDrawable();
    bool hasNext() const;
    CT_AbstractItemDrawable* nextItemDrawable();
    CT_AbstractItemDrawable* currentItemDrawable();
    bool removeCurrentItemDrawable();

    bool removeItemDrawable(CT_AbstractItemDrawable *item);
    bool removeItemDrawableWithoutAutoDelete(CT_AbstractItemDrawable *item);
    void itemDrawableWithThisIndexHasBeenRemoved(int index);

    bool isActive() const;

private:

    QList<CT_AbstractItemDrawable*>         *_list;
    bool                                    _autoDelete;

    bool                                    _active;

    // Iterateur mutable
    MyMutableItemDrawableListIterator       *_iterator;
};

#endif // CT_ITERATORTOOLS_H
