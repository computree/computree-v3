#include "dm_osgscenemanager.h"

#include "view/DocumentView/GraphicsViews/3D/gosggraphicsview.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"
#include "view/DocumentView/GraphicsViews/3D/Converter/dm_multipleitemdrawabletoosgworker.h"
#include "view/DocumentView/GraphicsViews/3D/Visitor/dm_boundingsphereitemselectedvisitor.h"

#include <osg/ValueObject>
#include <QEventLoop>
#include <algorithm>

#define PICKING_BIT     0x2
#define VISIBILITY_BIT  0x1

#define HAS_ITEM_KEY            "DM_OsgSceneManager"
#define HAS_ITEM_VALUE_OK       1

DM_OsgSceneManager::DM_OsgSceneManager(GOsgGraphicsView &view, osg::ref_ptr<osg::Group> scene)
{
    m_mutex = new QMutex(QMutex::Recursive);

    m_view = &view;
    m_converter = new DM_MultipleItemDrawableToOsgWorker(view);
    m_scene = scene;
    m_currentAction = NULL;
    m_currentActionConverter = NULL;
    m_groupContainsDrawablesOfAction = new osg::Group;
    m_itemRoot = new osg::Group;
    m_scene->addChild(m_itemRoot.get());
    m_scene->addChild(m_groupContainsDrawablesOfAction.get());

    initCullMaskOfViews();

    connect(m_converter, SIGNAL(newResultAvailable()), this, SLOT(converterResultAvailable()), Qt::QueuedConnection);
    connect(view.signalEmitter(), SIGNAL(drawingFinished()), this, SLOT(updateToRemove()), Qt::DirectConnection);
}

DM_OsgSceneManager::~DM_OsgSceneManager()
{
    delete m_converter;
    delete m_mutex;
}

void DM_OsgSceneManager::addItemDrawable(CT_AbstractItemDrawable &item)
{
    m_converter->addItemDrawable(item, m_view->getDocumentView()->getColor(&item));
}

void DM_OsgSceneManager::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    m_converter->removeItemDrawable(item);

    QMutexLocker locker(m_mutex);

    DM_PainterToOsgElementsResult r = m_results.take(&item);

    if(r.m_rootGroup.valid())
        m_toRemove.push_back(r);

    locker.unlock();
}

void DM_OsgSceneManager::updateItemDrawable(CT_AbstractItemDrawable &item)
{
    m_converter->addItemDrawable(item, m_view->getDocumentView()->getColor(&item));
}

void DM_OsgSceneManager::setCurrentAction(CT_AbstractAction *newAction)
{
    cancelCurrentActionConversion();

    QMutexLocker lock(&m_actionMutex);

    if(m_currentAction != NULL)
        disconnect(m_currentAction, NULL, this, NULL);

    m_currentAction = dynamic_cast<CT_AbstractActionForGraphicsView*>(newAction);

    if(m_currentAction != NULL) {
        connect(m_currentAction, SIGNAL(destroyed()), this, SLOT(cancelCurrentActionConversion()), Qt::DirectConnection);
        connect(m_currentAction, SIGNAL(drawing3DChanged()), this, SLOT(updateDrawableForCurrentAction()), Qt::QueuedConnection);
    }

    lock.unlock();

    updateDrawableForCurrentAction();
}

void DM_OsgSceneManager::updateItemDrawablesThatColorWasModified()
{
    QMutexLocker locker(m_mutex);

    ResultCollectionIterator it(m_results);

    while(it.hasNext()) {
        it.next();

        const DM_PainterToOsgElementsResult &r = it.value();

        if(m_view->getDocumentView()->isColorModified(it.key()))
            DM_PainterToOsgElements::staticChangeColorOfItemDrawableInResult(it.key(), m_view, r.m_rootGroup.get(), m_view->getDocumentView()->getColor(it.key()));
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfGlobalElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        ResultCollectionIterator it(m_results);

        while(it.hasNext()) {
            it.next();

            const DM_PainterToOsgElementsResult &r = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfGlobalElements(r.m_rootGroup.get());
        }
    } else {
        const DM_PainterToOsgElementsResult &r = m_results.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfGlobalElements(r.m_rootGroup.get());
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfLocalElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        ResultCollectionIterator it(m_results);

        while(it.hasNext()) {
            it.next();

            const DM_PainterToOsgElementsResult &r = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfLocalElements(r.m_rootGroup.get());
        }
    } else {
        const DM_PainterToOsgElementsResult &r = m_results.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfLocalElements(r.m_rootGroup.get());
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfAllElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        ResultCollectionIterator it(m_results);

        while(it.hasNext()) {
            it.next();

            const DM_PainterToOsgElementsResult &r = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfAllElements(r.m_rootGroup.get());
        }
    } else {
        const DM_PainterToOsgElementsResult &r = m_results.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfAllElements(r.m_rootGroup.get());
    }
}

void DM_OsgSceneManager::getBoundingBoxOfAllItemDrawablePresentInScene(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    const osg::BoundingSphere &bound = m_itemRoot->getBound();

    min[0] = bound.center().x()-bound.radius();
    min[1] = bound.center().y()-bound.radius();
    min[2] = bound.center().z()-bound.radius();

    max[0] = bound.center().x()+bound.radius();
    max[1] = bound.center().y()+bound.radius();
    max[2] = bound.center().z()+bound.radius();
}

void DM_OsgSceneManager::getBoundingBoxOfAllItemDrawableSelectedInScene(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    DM_BoundingSphereItemSelectedVisitor vi;
    m_itemRoot->accept(vi);

    const osg::BoundingBox &bbox = vi.getBound();

    min[0] = bbox.xMin();
    min[1] = bbox.yMin();
    min[2] = bbox.zMin();

    max[0] = bbox.xMax();
    max[1] = bbox.yMax();
    max[2] = bbox.zMax();
}

QList< DM_OsgSceneManager::ResultCollection::mapped_type > DM_OsgSceneManager::itemDrawableResults() const
{
    QMutexLocker locker(m_mutex);

    return m_results.values();
}

const DM_OsgSceneManager::ResultCollection &DM_OsgSceneManager::itemDrawableAndResults() const
{
    return m_results;
}

osg::Group* DM_OsgSceneManager::itemDrawableToGroup(CT_AbstractItemDrawable *item) const
{
    QMutexLocker locker(m_mutex);

    return m_results.value(item).m_rootGroup.get();
}

void DM_OsgSceneManager::staticSetEnablePicking(osg::Node *node, bool enable)
{
    osg::Node::NodeMask mask = node->getNodeMask();

    if(enable)
        mask |= PICKING_BIT;
    else
        mask &= ~PICKING_BIT;

    node->setNodeMask(mask);
}

void DM_OsgSceneManager::staticSetVisible(osg::Node *node, bool enable)
{
    osg::Node::NodeMask mask = node->getNodeMask();

    if(enable)
        mask |= VISIBILITY_BIT;
    else
        mask &= ~VISIBILITY_BIT;

    node->setNodeMask(mask);
}

bool DM_OsgSceneManager::staticIsVisible(osg::Node *node)
{
    return (node->getNodeMask() & VISIBILITY_BIT);
}

void DM_OsgSceneManager::staticSetPickerTraversalMask(osgUtil::IntersectionVisitor &visitor)
{
    visitor.setTraversalMask(PICKING_BIT);
}

CT_AbstractItemDrawable* DM_OsgSceneManager::staticConvertNodePathToItemDrawable(const osg::NodePath &nodePath)
{
    int n = nodePath.size();

    for(int i=0; i<n; ++i) {
        osg::Node *node = nodePath[i];

        int value;

        if(node->getUserValue<int>(HAS_ITEM_KEY, value) && (value == HAS_ITEM_VALUE_OK))
            return ((DM_CustomUserData*)node->getUserData())->itemDrawable();
    }

    return NULL;
}

CT_AbstractItemDrawable *DM_OsgSceneManager::staticConvertNodeToItemDrawable(const osg::Node &node)
{
    int value;

    if(node.getUserValue<int>(HAS_ITEM_KEY, value) && (value == HAS_ITEM_VALUE_OK))
        return ((DM_CustomUserData*)node.getUserData())->itemDrawable();

    return NULL;
}

void DM_OsgSceneManager::initCullMaskOfViews()
{
    int n = m_view->getCompositeViewer()->getNumViews();

    for(int i=0; i<n; ++i)
    {
        osgViewer::View *v = m_view->getCompositeViewer()->getView(i);
        v->getCamera()->setCullMask(VISIBILITY_BIT);
    }
}

void DM_OsgSceneManager::converterResultAvailable()
{
    QMutexLocker locker(m_mutex);

    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > l = m_converter->takeResults();
    QHashIterator< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > it(l);

    size_t index = m_itemRoot->getNumChildren();

    while(it.hasNext()) {
        it.next();

        CT_AbstractItemDrawable *item = it.key();
        const DM_PainterToOsgElementsResult &result = it.value();

        DM_PainterToOsgElementsResult previousResult = m_results.value(item);

        m_results.insert(item, result);

        DM_OsgSceneManager::staticSetEnablePicking(result.m_rootGroup.get(), true);

        result.m_rootGroup->setUserValue<int>(HAS_ITEM_KEY, HAS_ITEM_VALUE_OK);

        DM_CustomUserData *userData = new DM_CustomUserData(item, index);
        result.m_rootGroup->setUserData(userData);

        connect(this, SIGNAL(internalItemRemoved(size_t,size_t,size_t)), userData, SLOT(indexRemoved(size_t,size_t,size_t)), Qt::DirectConnection);

        // if update
        if(previousResult.m_rootGroup.valid()) {
            m_itemRoot->setChild(((DM_CustomUserData*)previousResult.m_rootGroup->getUserData())->indexInParent(), result.m_rootGroup.get());
            previousResult.m_rootGroup.release();
        } else {
            m_itemRoot->addChild(result.m_rootGroup.get());
            ++index;
        }
    }

    m_view->update();
}

void DM_OsgSceneManager::actionConverterResultAvailable()
{
    QMutexLocker lock(&m_actionMutex);

    m_groupContainsDrawablesOfAction->removeChild(0, m_groupContainsDrawablesOfAction->getNumChildren());

    if(m_currentActionConverter != NULL) {
        m_actionResult = m_currentActionConverter->results();
        staticSetEnablePicking(m_actionResult.m_rootGroup.get(), false);
        m_groupContainsDrawablesOfAction->addChild(m_actionResult.m_rootGroup.get());

        delete m_currentActionConverter;
        m_currentActionConverter = NULL;
    }

    m_view->update();
}

void DM_OsgSceneManager::cancelCurrentActionConversion()
{
    QMutexLocker lock(&m_actionMutex);

    if(m_currentActionConverter != NULL) {
        m_currentActionConverter->cancel();

        while(!m_currentActionConverter->isFinished());

        delete m_currentActionConverter;
        m_currentActionConverter = NULL;
    }
}

bool sortResultByIndex(const DM_PainterToOsgElementsResult &r1, const DM_PainterToOsgElementsResult &r2) {
    return ((DM_CustomUserData*)r1.m_rootGroup->getUserData())->indexInParent() < ((DM_CustomUserData*)r2.m_rootGroup->getUserData())->indexInParent();
}

void DM_OsgSceneManager::updateToRemove()
{
    QMutexLocker locker(m_mutex);

    std::sort(m_toRemove.begin(), m_toRemove.end(), sortResultByIndex);

    RemoveCollection::iterator it = m_toRemove.begin();
    RemoveCollection::iterator end = m_toRemove.end();

    if(it != end) {

        DM_PainterToOsgElementsResult rFirst = *it;
        staticSetVisible(rFirst.m_rootGroup.get(), false);
        size_t beginIndex = ((DM_CustomUserData*)rFirst.m_rootGroup->getUserData())->indexInParent();
        size_t endIndex = beginIndex;

        DM_CustomUserData *userData = (DM_CustomUserData*)rFirst.m_rootGroup->getUserData();

        ++it;

        while(it != end) {
            DM_PainterToOsgElementsResult r = *it;
            staticSetVisible(r.m_rootGroup.get(), false);
            userData = (DM_CustomUserData*)r.m_rootGroup->getUserData();

            size_t index = userData->indexInParent();

            if(index == endIndex+1)
                ++endIndex;
            else {
                size_t count = (endIndex-beginIndex)+1;
                m_itemRoot->removeChildren(beginIndex, count);

                beginIndex = index;
                endIndex = beginIndex;

                emit internalItemRemoved(beginIndex, endIndex, count);
            }

            ++it;
        }

        size_t count = (endIndex-beginIndex)+1;
        m_itemRoot->removeChildren(beginIndex, count);

        emit internalItemRemoved(beginIndex, endIndex, count);
    }

    m_toRemove.clear();

    m_view->update();
}

void DM_OsgSceneManager::updateDrawableForCurrentAction()
{
    QMutexLocker lock(&m_actionMutex);

    // m_currentActionConverter will be null when it's conversion was finished
    if((m_currentActionConverter == NULL)
            && (m_currentAction != NULL)) {

        // create a thread for the builder
        QThread *thread = new QThread();

        // create a converter
        m_currentActionConverter = new DM_ActionToOsgWorker();
        m_currentActionConverter->moveToThread(thread);
        m_currentActionConverter->setGraphicsView(m_view);
        m_currentActionConverter->setActionToConvert(m_currentAction);

        // don't delete the converter when finished but only the thread
        DM_AbstractWorker::staticConnectWorkerToThread(m_currentActionConverter, false, true, true);

        connect(m_currentActionConverter, SIGNAL(finished()), this, SLOT(actionConverterResultAvailable()), Qt::QueuedConnection);

        thread->start();
    }
}
