#ifndef CT_CONTAINERITERATOR_H
#define CT_CONTAINERITERATOR_H

#include "pluginShared_global.h"

#include <QListIterator>

class CT_Container;
class CT_AbstractItemDrawable;

class PLUGINSHAREDSHARED_EXPORT CT_ContainerIterator
{
public:
    CT_ContainerIterator(const CT_Container *container);
    ~CT_ContainerIterator();

    /**
     * @brief Returns true if there is at least one item ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    inline bool hasNext() const { return m_iterator->hasNext(); }

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    const CT_AbstractItemDrawable* next() { m_current = m_iterator->next(); return m_current;}

    /**
     * @brief Returns the current item.
     *
     * Calling this function without at least one call to next will leads to undefined results.
     */
    inline const CT_AbstractItemDrawable* current() const { return m_current; }

private:
    QListIterator<CT_AbstractItemDrawable*> *m_iterator;
    CT_AbstractItemDrawable                 *m_current;
};

#endif // CT_CONTAINERITERATOR_H
