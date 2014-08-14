#ifndef DM_ITREEVIEWMANAGER_H
#define DM_ITREEVIEWMANAGER_H

#include "interfaces.h"
#include <QStandardItem>

/**
 * @brief You must inherit this class if you want to use the DM_ItemDrawableTreeViewController
 */
class DM_ITreeViewManager
{
public:
    virtual ~DM_ITreeViewManager() {}

    /**
     * @brief Return true if we can (re)construct the model
     */
    virtual bool canConstructTheModel() const = 0;

    /**
     * @brief Return the complete list of ItemDrawable to add to the TreeView (only first level, childrens will be created automatically)
     */
    virtual QList<ItemDrawable*> itemDrawableForTreeView() const = 0;

    /**
     * @brief Return the number maximum of level to construct (how many times we can enter in the childrens method of a ItemDrawable in the 'itemDrawableForTreeView' list)
     */
    virtual int nLevelToConstruct() const = 0;

    /**
     * @brief Return the QStandardItem corresponding at the ItemDrawable passed in parameter
     */
    virtual QStandardItem* itemFromItemDrawable(const ItemDrawable *item) const = 0;

    /**
     * @brief Refresh headers of the view
     */
    virtual void refreshHeaders() = 0;
};

#endif // DM_ITREEVIEWMANAGER_H
