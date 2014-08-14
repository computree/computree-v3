#ifndef CT_RESULTITERATORFORGUI_H
#define CT_RESULTITERATORFORGUI_H

#include "pluginShared_global.h"

#include <QList>

class CT_ResultGroup;
class CT_OutAbstractItemModel;
class CT_AbstractItemDrawable;

/**
 * @brief Q_DECL_DEPRECATED
 */
class PLUGINSHAREDSHARED_EXPORT CT_ResultIteratorForGui
{
public:
    /**
     * @brief Create a iterator for the result passed in parameter
     */
    Q_DECL_DEPRECATED CT_ResultIteratorForGui(const CT_ResultGroup *result);

    /**
     * @brief Call this method before iterate !
     *
     *        Init the ierator to iterate over groups or items (detected automatically by the type of model passed in parameter)
     *
     *        Iterate over all groups or items and backup them in a collection to know how many they are.
     */
    Q_DECL_DEPRECATED void init(const CT_OutAbstractItemModel *model);

    /**
     * @brief Return how many groups or items you must iterate (call the method "init" first)
     */
    Q_DECL_DEPRECATED int size() const;

    /**
     * @brief Return true they are already a group or item next
     */
    Q_DECL_DEPRECATED bool hasNext() const;

    /**
     * @brief Return the next group or item
     */
    Q_DECL_DEPRECATED CT_AbstractItemDrawable* next();

private:
    CT_ResultGroup                              *m_result;

    QList<CT_AbstractItemDrawable*>             m_collection;
};

#endif // CT_RESULTITERATORFORGUI_H
