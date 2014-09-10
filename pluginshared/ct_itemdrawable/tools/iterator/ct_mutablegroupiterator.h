#ifndef CT_MUTABLEGROUPITERATOR_H
#define CT_MUTABLEGROUPITERATOR_H

#include "pluginShared_global.h"

#include <QVector>

class CT_AbstractItemGroup;

class CT_InAbstractGroupModel;
class CT_OutAbstractGroupModel;

class CT_ContainerIterator;

class CT_VirtualAbstractStep;

/**
 * @brief Class tools to iterate over groups (that can be removed) in a group
 */
class PLUGINSHAREDSHARED_EXPORT CT_MutableGroupIterator
{
public:
    /**
     * @brief Iterate over all groups contained in the parent
     * @param parent : the group in which you want to iterate
     */
    CT_MutableGroupIterator(CT_AbstractItemGroup *parent);

    /**
     * @brief Iterate by a IN group model
     * @param parent : the group in which you want to iterate
     * @param inModel : the IN group model that contains all groups to search in the group (parent)
     */
    CT_MutableGroupIterator(CT_AbstractItemGroup *parent, const CT_InAbstractGroupModel *inModel);

    /**
     * @brief Iterate by a OUT group model
     * @param parent : the group in which you want to iterate
     * @param inModel : the OUT group model that help to search group to iterate
     */
    CT_MutableGroupIterator(CT_AbstractItemGroup *parent, const CT_OutAbstractGroupModel *outModel);

    /**
     * @brief Iterate by a IN or OUT group model name.
     * @param parent : the group in which you want to iterate
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     * @param modelName : the IN or OUT group model name to search (typically a DEF_...)
     */
    CT_MutableGroupIterator(const CT_AbstractItemGroup *parent, const CT_VirtualAbstractStep *step, const QString &modelName);

    ~CT_MutableGroupIterator();

    /**
     * @brief Returns true if there is at least one group ahead of the iterator, i.e. the iterator is not at the back of the container; otherwise returns false.
     */
    bool hasNext() const;

    /**
     * @brief Returns the next item and advances the iterator by one position.
     *
     * Calling this function on an iterator located at the back of the container leads to undefined results.
     */
    CT_AbstractItemGroup* next();

    /**
     * @brief Returns the current item.
     *
     * Calling this function without at least one call to next will leads to undefined results.
     */
    CT_AbstractItemGroup* current();

    /**
     * @brief Removes the last item that was jumped over using one of the traversal functions
     */
    void remove();

private:
    mutable QList<CT_ContainerIterator*>    m_iterators;
    mutable bool                            m_hasNextCalled;

    void initIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractGroupModel *inModel);
    void initIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractGroupModel *outModel);
};

#endif // CT_MUTABLEGROUPITERATOR_H
