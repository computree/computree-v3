#ifndef DM_ITREEVIEWMANAGERT_H
#define DM_ITREEVIEWMANAGERT_H

#include "interfaces.h"

/**
 * @brief You must inherit this class if you want to use the DM_ItemDrawableTreeViewControllerT
 */
template<class Item>
class DM_ITreeViewManagerT
{
public:
    virtual ~DM_ITreeViewManagerT() {}

    /**
     * @brief Return true if we can (re)construct the model
     */
    virtual bool canConstructTheModel() const = 0;

    /**
     * @brief Return the complete list of CT_AbstractItemDrawable to add to the TreeView (only first level, childrens will be created automatically)
     */
    virtual QList<CT_AbstractItemDrawable*> itemDrawableForTreeView() const = 0;

    /**
     * @brief Return the number maximum of level to construct (how many times we can enter in the childrens method of a CT_AbstractItemDrawable in the 'itemDrawableForTreeView' list)
     */
    virtual int nLevelToConstruct() const = 0;

    /**
     * @brief Return the Item corresponding to the CT_AbstractItemDrawable passed in parameter
     */
    virtual Item* itemFromItemDrawable(const CT_AbstractItemDrawable *item) const = 0;

    /**
     * @brief Refresh headers of the view
     */
    virtual void refreshHeaders() = 0;

    /**
     * @brief Refresh expanded items
     */
    virtual void refreshExpanded() = 0;
};

#endif // DM_ITREEVIEWMANAGERT_H
