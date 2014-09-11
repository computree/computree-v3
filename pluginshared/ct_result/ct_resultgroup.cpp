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
