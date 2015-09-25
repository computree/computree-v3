#ifndef DM_OSGSCENEMANAGER_H
#define DM_OSGSCENEMANAGER_H

#include <QTimer>
#include <QHash>
#include <QHashIterator>

#include <osg/Group>
#include <osgUtil/IntersectionVisitor>

#include "view/DocumentView/GraphicsViews/3D/Converter/dm_actiontoosgworker.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

class DM_MultipleItemDrawableToOsgWorker;
class GOsgGraphicsView;

/**
 * @brief Used by DM_OsgSceneManager to keep a reference to an CT_AbstractItemDrawable in a node
 */
class DM_CustomUserData : public QObject, public osg::Referenced
{
    Q_OBJECT

public:
    DM_CustomUserData(CT_AbstractItemDrawable *item, size_t indexInParent) {
        m_item = item;
        m_indexInParent = indexInParent;
    }

    /*~DM_CustomUserData(){
        int a;
    }*/

    CT_AbstractItemDrawable* itemDrawable() const {
        return m_item;
    }

    size_t indexInParent() const {
        return m_indexInParent;
    }

private:
    CT_AbstractItemDrawable *m_item;
    size_t                  m_indexInParent;

public slots:
    /**
     * @brief Connect this slot to update the index of this object
     */
    void indexRemoved(size_t beginIndex, size_t endIndex, size_t count) {
        if(m_indexInParent > endIndex)
            m_indexInParent -= count;
    }
};

/**
 * @brief Use this class to manage itemdrawable that must be in the scene. ItemDrawable will be autoamtically
 *        converted to osg elements and added to the scene or removed if you want to remove one.
 */
class DM_OsgSceneManager : public QObject
{
    Q_OBJECT

public:
    DM_OsgSceneManager(GOsgGraphicsView &view, osg::ref_ptr<osg::Group> scene);
    ~DM_OsgSceneManager();

    typedef QHash<CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > ResultCollection;
    typedef QHashIterator<CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > ResultCollectionIterator;

    /**
     * @brief Returns true if a conversion is in progress or in wait to begin
     */
    bool isConversionInProgress() const;

public slots:

    /**
     * @brief Call to change the current action draw in 3D
     */
    void setCurrentAction(CT_AbstractAction *newAction);

    /**
     * @brief Call to add an itemDrawable to the scene
     */
    void addItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Call to remove a itemdrawable from the scene
     */
    void removeItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Call this method to update the 3d drawing of the itemdrawable passed in parameter
     */
    void updateItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Update colors of all item drawable (if it was modified)
     */
    void updateItemDrawablesThatColorWasModified();

    /**
     * @brief Dirty display list of all itemdrawable that was created with the global vertex array (points cloud, mesh, etc...)
     * @param item : item to dirty display list. If NULL the entire list of itemdrawable will be dirty.
     */
    void dirtyDisplayListOfGlobalElementsOfItemDrawable(CT_AbstractItemDrawable *item = NULL);

    /**
     * @brief Dirty display list of all item drawable that was created with a local vertex array (circle, quads, lines, etc...)
     * @param item : item to dirty display list. If NULL the entire list of itemdrawable will be dirty.
     */
    void dirtyDisplayListOfLocalElementsOfItemDrawable(CT_AbstractItemDrawable *item = NULL);

    /**
     * @brief Dirty display list of all item drawable that was created with a local or global vertex array
     * @param item : item to dirty display list. If NULL the entire list of itemdrawable will be dirty.
     */
    void dirtyDisplayListOfAllElementsOfItemDrawable(CT_AbstractItemDrawable *item = NULL);

    /**
     * @brief Returns the bounding box of all item drawable present in the scene
     */
    void getBoundingBoxOfAllItemDrawablePresentInScene(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    /**
     * @brief Returns the bounding box of all item drawable selected in the scene
     */
    void getBoundingBoxOfAllItemDrawableSelectedInScene(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    /**
     * @brief Return all results that represent itemdrawable (one result for one itemdrawable). It's results from DM_PainterToOsgElements
     */
    QList< ResultCollection::mapped_type > itemDrawableResults() const;

    /**
     * @brief Return a hashmap with the itemdrawable in key and it's group in value
     */
    const ResultCollection& itemDrawableAndResults() const;

    /**
     * @brief Return the osg::Group that correspond to the item passed in parameter
     */
    osg::Group* itemDrawableToGroup(CT_AbstractItemDrawable *item) const;

    /**
     * @brief Enable/Disable the group from the picking
     */
    static void staticSetEnablePicking(osg::Node *node, bool enable);

    /**
     * @brief Enable/Disable the visibility of the node
     */
    static void staticSetVisible(osg::Node *node, bool enable);

    /**
     * @brief Return true if the node is visible
     */
    static bool staticIsVisible(osg::Node *node);

    /**
     * @brief Set the picker visitor traversal mask to ignore elements that was not visible and must not be picked
     */
    static void staticSetPickerTraversalMask(osgUtil::IntersectionVisitor &visitor);

    /**
     * @brief Convert a nodepath to a itemdrawable. Return NULL if nodepath don't contains a itemdrawable
     */
    static CT_AbstractItemDrawable* staticConvertNodePathToItemDrawable(const osg::NodePath &nodePath);

    /**
     * @brief Convert a node to a itemdrawable. Return NULL if node don't contains a itemdrawable
     */
    static CT_AbstractItemDrawable* staticConvertNodeToItemDrawable(const osg::Node &node);

private:
    typedef std::vector<DM_PainterToOsgElementsResult> RemoveCollection;

    osg::ref_ptr<osg::Group>                                        m_scene;                        // the root scene
    osg::ref_ptr<osg::Group>                                        m_itemRoot;                     // the root groups that contains all itemdrawable converted to osg groups
    osg::ref_ptr<osg::Program>                                      m_shaderForGlobalGeometries;    // the sahder to use with the global points cloud
    DM_MultipleItemDrawableToOsgWorker                              *m_converter;                   // convert itemdrawable to osg structure
    DM_ActionToOsgWorker                                            *m_currentActionConverter;      // convert a action to osg structure
    ResultCollection                                                m_results;                      // a hash to backup result for item
    GOsgGraphicsView                                                *m_view;
    RemoveCollection                                                m_toRemove;
    QTimer                                                          m_timerUpdateRemove;
    QMutex                                                          m_actionMutex;
    DM_PainterToOsgElementsResult                                   m_actionResult;
    QMutex                                                          *m_mutex;
    CT_AbstractActionForGraphicsView                                *m_currentAction;
    osg::ref_ptr<osg::Group>                                        m_groupContainsDrawablesOfAction;

    /**
     * @brief Init the cull mask of all views of the GOsgGraphicsView
     */
    void initCullMaskOfViews();

private slots:
    /**
     * @brief Called when a new result is available from the converter
     */
    void converterResultAvailable();

    /**
     * @brief Called when a new result is available from the action converter
     */
    void actionConverterResultAvailable();

    /**
     * @brief Cancel the current action conversion
     */
    void cancelCurrentActionConversion();

    /**
     * @brief Called when we must update the removed item from the list
     */
    void updateToRemove();

    /**
     * @brief Called when the drawing of an action was changed and we must update the drawable in the scene
     */
    void updateDrawableForCurrentAction();

signals:

    /**
     * @brief Emitted when the time to remove item must be started.
     */
    void internalMustStartRemoveTimer();

    /**
     * @brief Emitted when one or more items was removed.
     */
    void internalItemRemoved(size_t beginIndex, size_t endIndex, size_t count);
};

#endif // DM_OSGSCENEMANAGER_H
