#ifndef CT_MUTABLEITEMITERATOR_H
#define CT_MUTABLEITEMITERATOR_H

#include "pluginShared_global.h"

#include <QVector>
#include <QHash>

class CT_AbstractItemGroup;
class CT_AbstractSingularItemDrawable;

class CT_InAbstractSingularItemModel;
class CT_OutAbstractSingularItemModel;

class CT_ContainerIterator;

class CT_GroupItemDrawableContainer;

/**
 * @brief Class tools to iterate over items (that can be removed) in a group
 *
 * @warning Be careful that if items of the group is changed the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_MutableItemIterator
{
public:
    /**
     * @brief Iterate over all itemdrawable contained in the group
     * @param parent : the group in which you want to iterate
     */
    CT_MutableItemIterator(CT_AbstractItemGroup *parent);

    /**
     * @brief Iterate by a IN itemdrawable model
     * @param parent : the group in which you want to iterate
     * @param inModel : the IN itemdrawable model that contains all item model to search in the group (parent)
     */
    CT_MutableItemIterator(CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel);

    /**
     * @brief Iterate by a OUT itemdrawable model
     * @param parent : the group in which you want to iterate
     * @param inModel : the OUT itemdrawable model that help to search item to iterate
     */
    CT_MutableItemIterator(CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel);

    ~CT_MutableItemIterator();

    /**
     * @brief Returns true if there is at least one group ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    CT_AbstractSingularItemDrawable* next();

    /**
     * @brief Returns the current item.
     *
     * Calling this function without at least one call to next will leads to undefined results.
     */
    CT_AbstractSingularItemDrawable* current();

    /**
     * @brief Removes the last item that was jumped over using one of the traversal functions
     */
    void remove();

private:
    mutable QList<QString>                              m_iterators;

    CT_AbstractItemGroup                                *m_parent;
    mutable CT_AbstractSingularItemDrawable             *m_current;

    QHash<QString, CT_GroupItemDrawableContainer*>      m_items;

    mutable bool                                        m_hasNextCalled;
};

#endif // CT_MUTABLEITEMITERATOR_H
