#include "ct_resultgroup.h"

#include "ct_global/ct_context.h"

#include "ct_itemdrawable/tools/iterator/ct_mutablegroupiterator.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h"

CT_ResultGroup::CT_ResultGroup() : CT_AbstractResult()
{
    m_guiIterator = NULL;
    m_gIterator = NULL;
    m_gRootIterator = NULL;
    m_iIterator = NULL;
}

CT_ResultGroup::CT_ResultGroup(const CT_OutAbstractResultModelGroup &model,
                               CT_ResultInitializeData &data,
                               CT_VirtualAbstractStep &parentStep) : CT_AbstractResult(&model, data, parentStep)
{
    m_guiIterator = NULL;
    m_gIterator = NULL;
    m_gRootIterator = NULL;
    m_iIterator = NULL;
}

CT_ResultGroup::~CT_ResultGroup()
{
    clearFromMemory();
}

QString CT_ResultGroup::getType() const
{
    return staticGetType();
}

QString CT_ResultGroup::staticGetType()
{
    return CT_AbstractResult::staticGetType() + "/CT_ResultGroup";
}

bool CT_ResultGroup::addGroup(CT_AbstractItemGroup *group)
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    _groups.append(group);

    return true;
}

bool CT_ResultGroup::addGroups(const QList<CT_AbstractItemGroup *> &groups)
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    _groups.append(groups);

    return true;
}

const QList<CT_AbstractItemGroup *>& CT_ResultGroup::groups() const
{
    return _groups;
}


// Ajout AP 24/07/2013

CT_AbstractItemGroup* CT_ResultGroup::beginGroup(const CT_InAbstractGroupModel *model)
{
    delete m_gIterator;
    m_gIterator = new CT_ResultGroupIterator(this, model);

    return nextGroup();
}

CT_AbstractItemGroup* CT_ResultGroup::nextGroup()
{
    if(m_gIterator->hasNext())
        return (CT_AbstractItemGroup*)m_gIterator->next();

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_ResultGroup::beginItem(const CT_InAbstractSingularItemModel *model)
{
    delete m_iIterator;
    m_iIterator = new CT_ResultItemIterator(this, model);

    return nextItem();
}

CT_AbstractSingularItemDrawable* CT_ResultGroup::nextItem()
{
    if(m_iIterator->hasNext())
        return (CT_AbstractSingularItemDrawable*)m_iIterator->next();

    return NULL;
}

// Fin d'ajout AP 24/07/2013



bool CT_ResultGroup::recursiveBeginIterateItems(const CT_InAbstractSingularItemModel &model)
{
    delete m_iIterator;
    m_iIterator = new CT_ResultItemIterator(this, &model);

    return m_iIterator->hasNext();
}

CT_AbstractSingularItemDrawable *CT_ResultGroup::recursiveNextItem()
{
    return nextItem();
}

bool CT_ResultGroup::recursiveBeginIterateGroups(const CT_InAbstractGroupModel &model)
{
    delete m_gIterator;
    m_gIterator = new CT_ResultGroupIterator(this, &model);

    return m_gIterator->hasNext();
}

CT_AbstractItemGroup* CT_ResultGroup::recursiveNextGroup()
{
    return nextGroup();
}

bool CT_ResultGroup::setList(QList<CT_AbstractItemGroup*> &list)
{
    if(_groups.isEmpty())
    {
        _groups = list;
        return true;
    }

    return false;
}

const QList<CT_AbstractItemGroup*>& CT_ResultGroup::getList() const
{
    return _groups;
}

int CT_ResultGroup::beginIterateItemDrawableList()
{
    return beginIterateItemDrawableList(false);
}

int CT_ResultGroup::beginIterateItemDrawableList(bool mutableIterator)
{
    Q_UNUSED(mutableIterator)

    delete m_gRootIterator;
    m_gRootIterator = new CT_ResultGroupIterator(this);

    return _groups.size();
}

CT_AbstractItemDrawable* CT_ResultGroup::nextItemDrawable()
{
    if(m_gRootIterator->hasNext())
        return (CT_AbstractItemDrawable*)m_gRootIterator->next();

    return NULL;
}

int CT_ResultGroup::recursiveBeginIterateItemDrawableWithModel(const CT_AbstractModel &model)
{
    delete m_guiIterator;
    m_guiIterator = NULL;

    const CT_OutAbstractItemModel *itemModel = dynamic_cast<const CT_OutAbstractItemModel*>(&model);

    if(itemModel == NULL)
        return 0;

    m_guiIterator = new CT_ResultIterator(this, itemModel, true);

    return m_guiIterator->size();
}

CT_AbstractItemDrawable* CT_ResultGroup::recursiveNextItemDrawable()
{
    if(m_guiIterator->hasNext())
        return (CT_AbstractItemDrawable*)m_guiIterator->next();

    return NULL;
}

bool CT_ResultGroup::removeItemSomethingInStructure(CT_AbstractItemDrawable *parent,
                                                            CT_AbstractItemDrawable *item)
{
    CT_AbstractItemGroup *gParent = dynamic_cast<CT_AbstractItemGroup*>(parent);

    if(gParent == NULL)
        return false;

    CT_AbstractSingularItemDrawable *sItem = dynamic_cast<CT_AbstractSingularItemDrawable*>(item);

    if(sItem != NULL)
        return removeSingularItemDrawableSomethingInStructure(gParent, sItem);

    return removeGroupSomethingInStructure((CT_AbstractItemGroup*)item);
}

bool CT_ResultGroup::removeGroupSomethingInStructure(CT_AbstractItemGroup *group)
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    recursiveSetBeRemovedLater(group);

    return true;
}

bool CT_ResultGroup::removeSingularItemDrawableSomethingInStructure(CT_AbstractItemGroup *parent,
                                                                    CT_AbstractSingularItemDrawable *item)
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    // un groupe est toujours une copie donc son résultat est
    // obligatoirement celui-ci. Si ce n'est pas le cas alors l'item ne fait pas partie de
    // ce résultat donc on ne peux pas le supprimer.
    if((parent == NULL)
            || (parent->result() != this))
        return false;

    return parent->removeItem(item);
}

CT_AbstractResult* CT_ResultGroup::createNewInstance(CT_OutAbstractResultModel *model,
                                                     CT_ResultInitializeData &data,
                                                     CT_VirtualAbstractStep &parentStep,
                                                     CT_ResultCopyModeList copyModeList)
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    CT_OutAbstractResultModelGroup *newModel = dynamic_cast<CT_OutAbstractResultModelGroup*>(model);

    if(newModel == NULL)
        return NULL;

    CT_ResultGroup *res = new CT_ResultGroup(*newModel, data, parentStep);
    res->setName(getName());

    QListIterator<CT_AbstractItemGroup*> it(_groups);

    while(it.hasNext())
    {
        CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)it.next()->copy(newModel->rootGroup(), res, copyModeList);

        if(group == NULL)
        {
            delete res;
            return NULL;
        }
        else
        {
            res->addGroup(group);
        }
    }

    return res;
}

void CT_ResultGroup::setComplete()
{
    QMutexLocker locker(&_mutexAccessItemDrawable);

    QMutableListIterator<CT_AbstractItemGroup*> it(_groups);

    while(it.hasNext())
    {
        CT_AbstractItemGroup *gr = it.next();

        if(gr->willBeRemovedLater())
        {
            it.remove();
            delete gr;
        }
        else if(gr->atLeastOneChildMustBeRemovedLater())
        {
            gr->recursiveSetComplete();
        }
    }

    CT_AbstractResult::setComplete();
}

void CT_ResultGroup::clearFromMemoryProtected()
{
    delete m_guiIterator;
    m_guiIterator = NULL;

    delete m_gIterator;
    m_gIterator = NULL;

    delete m_gRootIterator;
    m_gRootIterator = NULL;

    delete m_iIterator;
    m_iIterator = NULL;

    PS_REPOSITORY->begingDeleteMultiCloud();

    int i = 0;
    int size = _groups.size();

    while(!_groups.isEmpty())
    {
        CT_AbstractItemGroup *gr = _groups.takeFirst();
        gr->internalSetWillBeRemovedFromResult(this);

        delete gr;

        ++i;

        setClearFromMemoryProgress((i*100)/size);
    }

    PS_REPOSITORY->endDeleteMultiCloud();

    _groups.clear();
}

void CT_ResultGroup::recursiveSetBeRemovedLater(CT_AbstractItemGroup *group)
{
    if(!group->willBeRemovedLater())
    {
        group->setBeRemovedLater();

        if(!group->isEmptyOfGroups())
        {
            CT_MutableGroupIterator it(group);

            while(it.hasNext())
                recursiveSetBeRemovedLater(it.next());
        }
    }
}
