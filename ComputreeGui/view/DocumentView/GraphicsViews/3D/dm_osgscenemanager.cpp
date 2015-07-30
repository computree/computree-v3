#include "dm_osgscenemanager.h"

#include "view/DocumentView/GraphicsViews/3D/gosggraphicsview.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"
#include "view/DocumentView/GraphicsViews/3D/Converter/dm_multipleitemdrawabletoosgworker.h"
#include "view/DocumentView/GraphicsViews/3D/Visitor/dm_boundingsphereitemselectedvisitor.h"

#include <osg/ValueObject>
#include <QEventLoop>

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
    m_timer.setSingleShot(true);
    m_timer.setInterval(50);

    initCullMaskOfViews();

    connect(m_converter, SIGNAL(newResultAvailable()), this, SLOT(converterResultAvailable()), Qt::QueuedConnection);
    connect(this, SIGNAL(mustStartRemoveTimer()), &m_timer, SLOT(start()), Qt::QueuedConnection);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateToRemove()), Qt::QueuedConnection);

    m_converter->start();
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
    m_toRemove.append(&item);
    locker.unlock();

    emit mustStartRemoveTimer();
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

    QHashIterator<CT_AbstractItemDrawable*, osg::ref_ptr<osg::Group> > it(m_hashGroup);

    while(it.hasNext()) {
        it.next();

        osg::ref_ptr<osg::Group> g = it.value();

        if(m_view->getDocumentView()->isColorModified(it.key()))
            DM_PainterToOsgElements::staticChangeColorOfItemDrawableInResult(g.get(), m_view->getDocumentView()->getColor(it.key()));
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfGlobalElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        QHashIterator<CT_AbstractItemDrawable*, osg::ref_ptr<osg::Group> > it(m_hashGroup);

        while(it.hasNext()) {
            it.next();

            osg::ref_ptr<osg::Group> g = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfGlobalElements(g.get());
        }
    } else {
        osg::ref_ptr<osg::Group> g = m_hashGroup.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfGlobalElements(g.get());
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfLocalElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        QHashIterator<CT_AbstractItemDrawable*, osg::ref_ptr<osg::Group> > it(m_hashGroup);

        while(it.hasNext()) {
            it.next();

            osg::ref_ptr<osg::Group> g = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfLocalElements(g.get());
        }
    } else {
        osg::ref_ptr<osg::Group> g = m_hashGroup.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfLocalElements(g.get());
    }
}

void DM_OsgSceneManager::dirtyDisplayListOfAllElementsOfItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(item == NULL) {

        QHashIterator<CT_AbstractItemDrawable*, osg::ref_ptr<osg::Group> > it(m_hashGroup);

        while(it.hasNext()) {
            it.next();

            osg::ref_ptr<osg::Group> g = it.value();

            DM_PainterToOsgElements::staticDirtyDisplayListOfAllElements(g.get());
        }
    } else {
        osg::ref_ptr<osg::Group> g = m_hashGroup.value(item);

        DM_PainterToOsgElements::staticDirtyDisplayListOfAllElements(g.get());
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

QList<osg::ref_ptr<osg::Group> > DM_OsgSceneManager::itemDrawableGroups() const
{
    return m_hashGroup.values();
}

QHash<CT_AbstractItemDrawable *, osg::ref_ptr<osg::Group> > DM_OsgSceneManager::itemDrawableAndGroup() const
{
    return m_hashGroup;
}

osg::Group* DM_OsgSceneManager::itemDrawableToGroup(CT_AbstractItemDrawable *item) const
{
    return m_hashGroup.value(item, osg::ref_ptr<osg::Group>(NULL)).get();
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

    while(it.hasNext()) {
        it.next();

        DM_PainterToOsgElementsResult previousResult = m_hashResult.take(it.key());
        m_hashGroup.remove(it.key());

        // if update
        if(previousResult.m_rootGroup.valid())
            m_itemRoot->removeChild(previousResult.m_rootGroup.release());

        m_hashGroup.insert(it.key(), it.value().m_rootGroup);
        m_hashResult.insert(it.key(), it.value());

        DM_OsgSceneManager::staticSetEnablePicking(it.value().m_rootGroup.get(), true);

        it.value().m_rootGroup->setUserValue<int>(HAS_ITEM_KEY, HAS_ITEM_VALUE_OK);
        it.value().m_rootGroup->setUserData(new DM_CustomUserData(it.key()));

        m_itemRoot->addChild(it.value().m_rootGroup.get());
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

void DM_OsgSceneManager::updateToRemove()
{
    QMutexLocker locker(m_mutex);

    m_timer.stop();

    while(!m_toRemove.isEmpty())
    {
        CT_AbstractItemDrawable *d = m_toRemove.takeFirst();
        DM_PainterToOsgElementsResult res = m_hashResult.take(d);
        m_hashGroup.remove(d);

        if(res.m_rootGroup.valid())
            m_itemRoot->removeChild(res.m_rootGroup.release()); // TODO : problem when remove child with large amount of vertex in local vertex array => crash in painter.drawText after ???!!
    }

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

        // create a converter
        /*m_currentActionConverter = new DM_ActionToOsgWorker();
        m_currentActionConverter->setGraphicsView(m_view);
        m_currentActionConverter->setActionToConvert(m_currentAction);
        m_currentActionConverter->apply();
        m_groupContainsDrawablesOfAction->removeChild(0, m_groupContainsDrawablesOfAction->getNumChildren());
        osg::ref_ptr<osg::Group> results = m_currentActionConverter->results();
        staticSetEnablePicking(results.get(), false);
        m_groupContainsDrawablesOfAction->addChild(results.get());

        delete m_currentActionConverter;
        m_currentActionConverter = NULL;

        m_view->update();*/
    }
}
