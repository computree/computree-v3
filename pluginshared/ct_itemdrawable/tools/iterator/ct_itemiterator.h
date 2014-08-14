#ifndef CT_ITEMITERATOR_H
#define CT_ITEMITERATOR_H

#include "pluginShared_global.h"

#include <QVector>
#include <QHash>

class CT_AbstractItemGroup;
class CT_AbstractSingularItemDrawable;

class CT_InAbstractSingularItemModel;
class CT_OutAbstractSingularItemModel;

class CT_ContainerIterator;

class CT_GroupItemDrawableContainer;

class CT_VirtualAbstractStep;

/**
 * @brief Class tools to iterate over items in a group
 *
 * @warning Be careful that if items of the group is changed the iterator can returns undefined results
 */
class PLUGINSHAREDSHARED_EXPORT CT_ItemIterator
{
public:
    /**
     * @brief Iterate over all itemdrawable contained in the group
     * @param parent : the group in which you want to iterate
     */
    CT_ItemIterator(const CT_AbstractItemGroup *parent);

    /**
     * @brief Iterate by a IN itemdrawable model
     * @param parent : the group in which you want to iterate
     * @param inModel : the IN itemdrawable model that contains all item model to search in the group (parent)
     */
    CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel);

    /**
     * @brief Iterate by a OUT itemdrawable model
     * @param parent : the group in which you want to iterate
     * @param inModel : the OUT itemdrawable model that help to search item to iterate
     */
    CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel);

    /**
     * @brief Iterate by a IN or OUT group model name.
     * @param parent : the group in which you want to iterate
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     * @param modelName : the IN or OUT group model name to search (typically a DEF_...)
     */
    CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_VirtualAbstractStep *step, const QString &modelName);

    ~CT_ItemIterator();

    /**
     * @brief Returns true if there is at least one group ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    const CT_AbstractSingularItemDrawable* next();

    /**
     * @brief Returns the current item.
     *
     * Calling this function without at least one call to next will leads to undefined results.
     */
    const CT_AbstractSingularItemDrawable* current() const;

private:
    mutable QList<QString>                              m_iterators;

    CT_AbstractItemGroup                                *m_parent;
    mutable CT_AbstractSingularItemDrawable             *m_current;

    QHash<QString, CT_GroupItemDrawableContainer*>      m_items;

    mutable bool                                        m_hasNextCalled;

    void initIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel);
    void initIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel);
};

#endif // CT_ITEMITERATOR_H
