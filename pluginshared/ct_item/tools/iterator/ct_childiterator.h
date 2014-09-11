#ifndef CT_CHILDITERATOR_H
#define CT_CHILDITERATOR_H

#include "pluginShared_global.h"

#include <QList>

class CT_AbstractItem;

/**
 * @brief Class tools to iterate over child in a CT_AbstractItem
 *
 * @warning Be careful that if child is changed when you iterate the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_ChildIterator
{
public:
    /**
     * @brief Iterate over all childrens contained in the item (parent) passed in parameter
     * @param parent : the item in which you want to iterate
     */
    CT_ChildIterator(const CT_AbstractItem *parent);
    ~CT_ChildIterator();

    /**
     * @brief Returns true if there is at least one group ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    const CT_AbstractItem* next();

    /**
     * @brief Returns the current item.
     *
     * Calling this function without at least one call to next will leads to undefined results.
     */
    const CT_AbstractItem* current() const;

private:
    QList<CT_AbstractItem*>     m_childrens;
};

#endif // CT_CHILDITERATOR_H
