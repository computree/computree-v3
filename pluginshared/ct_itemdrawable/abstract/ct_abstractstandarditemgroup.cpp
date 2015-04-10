#include "ct_abstractstandarditemgroup.h"
#include <assert.h>

#include "ct_global/ct_context.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_itemdrawable/tools/iterator/ct_containeriterator.h"

const CT_StandardStandardItemGroupDrawManager CT_AbstractStandardItemGroup::SIG_DRAW_MANAGER;

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup() : CT_AbstractItemGroup()
{
    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup(const CT_OutAbstractGroupModel *model,
                                                           const CT_AbstractResult *result) : CT_AbstractItemGroup(model, result)
{
    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup(const QString &modelName,
                                                           const CT_AbstractResult *result) : CT_AbstractItemGroup(modelName, result)
{
    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::~CT_AbstractStandardItemGroup()
{
    delete _mutexAccessGroup;
    delete _mutexAccessItem;

    qDeleteAll(_groupsNew.begin(), _groupsNew.end());
    qDeleteAll(_groupsBackup.begin(), _groupsBackup.end());

    qDeleteAll(_itemsNew.begin(), _itemsNew.end());
    qDeleteAll(_itemsBackup.begin(), _itemsBackup.end());
}

QString CT_AbstractStandardItemGroup::getType() const
{
    return staticGetType();
}

QString CT_AbstractStandardItemGroup::staticGetType()
{
    return CT_AbstractItemGroup::staticGetType() + "/CT_AbstractStandardItemGroup";
}

bool CT_AbstractStandardItemGroup::containsItemDrawable(const CT_InAbstractSingularItemModel *model) const
{
    assert(model != NULL);

    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_InStdModelPossibility*> list = model->getPossibilitiesSaved();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = it.next();

        if(possibility->isSelected())
        {
            CT_GroupItemDrawableContainer *ct = _itemsNew.value(possibility->outModel()->uniqueName(), NULL);

            if(ct == NULL)
            {
                ct = _itemsBackup.value(possibility->outModel()->uniqueName(), NULL);

                if((ct != NULL) && !ct->willBeRemovedLater())
                    return true;
            }
            else if(!ct->willBeRemovedLater())
            {
                return true;
            }
        }
    }

    return false;
}

bool CT_AbstractStandardItemGroup::containsItemDrawable(const CT_OutAbstractSingularItemModel *model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    CT_GroupItemDrawableContainer *ct = _itemsNew.value(model->uniqueName(), NULL);

    if(ct == NULL)
    {
        ct = _itemsBackup.value(model->uniqueName(), NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            return true;
    }
    else if(!ct->willBeRemovedLater())
    {
        return true;
    }

    return false;
}

int CT_AbstractStandardItemGroup::nItems() const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    int n = 0;

    QHashIterator<QString, CT_GroupItemDrawableContainer*> it(_itemsNew);

    while(it.hasNext())
    {
        if(!it.next().value()->willBeRemovedLater())
            ++n;
    }

    it = _itemsBackup;

    while(it.hasNext())
    {
        if(!it.next().value()->willBeRemovedLater())
            ++n;
    }

    return n;
}

QHash<QString, CT_GroupItemDrawableContainer*> CT_AbstractStandardItemGroup::items() const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QHash<QString, CT_GroupItemDrawableContainer*> l;
    QHashIterator<QString, CT_GroupItemDrawableContainer*> it(itemsNew());

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    it = itemsBackup();

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    return l;
}

bool CT_AbstractStandardItemGroup::removeItem(CT_AbstractSingularItemDrawable *item)
{
    QMutexLocker locker(_mutexAccessItem);

    if(item->model() == NULL)
        return false;

    QString modelName = item->model()->uniqueName();

    CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

    if(ct != NULL)
    {
        ct->setWillBeRemovedLater();
        return true;
    }

    ct = _itemsBackup.value(modelName, NULL);

    if(ct != NULL)
    {
        ct->setWillBeRemovedLater();
        return true;
    }

    return false;
}

int CT_AbstractStandardItemGroup::nGroups() const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    int n = 0;

    QHashIterator<QString, CT_Container*> it(_groupsNew);

    while(it.hasNext())
    {
        CT_Container *ct = it.next().value();

        n += (ct->getList()->size() - ct->itemThatWillBeRemovedLater().size());
    }

    it = _groupsBackup;

    while(it.hasNext())
    {
        CT_Container *ct = it.next().value();

        n += (ct->getList()->size() - ct->itemThatWillBeRemovedLater().size());
    }

    return n;
}

QHash<QString, CT_Container*> CT_AbstractStandardItemGroup::groups() const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    QHash<QString, CT_Container*> l;
    QHashIterator<QString, CT_Container*> it(groupsNew());

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    it = groupsBackup();

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    return l;
}

bool CT_AbstractStandardItemGroup::removeGroup(CT_AbstractItemGroup *group)
{
    return ((CT_ResultGroup*)result())->removeGroupSomethingInStructure(group);
}

bool CT_AbstractStandardItemGroup::containsGroup(const CT_OutAbstractGroupModel *model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    CT_Container *container = _groupsNew.value(model->uniqueName(), NULL);

    if(container == NULL)
        container = _groupsBackup.value(model->uniqueName(), NULL);

    if(container == NULL)
        return false;

    return ((container->getList()->size() - container->itemThatWillBeRemovedLater().size()) != 0);
}

bool CT_AbstractStandardItemGroup::isEmpty() const
{
    return ((nGroups() == 0) && (nItems() == 0));
}

bool CT_AbstractStandardItemGroup::isEmptyOfGroups() const
{
    return (nGroups() == 0);
}

bool CT_AbstractStandardItemGroup::isEmptyOfSingularItems() const
{
    return (nItems() == 0);
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::item(const CT_OutAbstractSingularItemModel *model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    CT_GroupItemDrawableContainer *ct = _itemsNew.value(model->uniqueName(), NULL);

    if((ct != NULL) && !ct->willBeRemovedLater())
        return ct->item();

    ct = _itemsBackup.value(model->uniqueName(), NULL);

    if((ct != NULL) && !ct->willBeRemovedLater())
        return ct->item();

    return NULL;
}

QList<CT_AbstractSingularItemDrawable *> CT_AbstractStandardItemGroup::items(const CT_InAbstractSingularItemModel *model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_AbstractSingularItemDrawable*> retList;

    QList<CT_InStdModelPossibility*> list = model->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        QString modelName = it.next()->outModel()->uniqueName();

        CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            retList.append(ct->item());

        ct = _itemsBackup.value(modelName, NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            retList.append(ct->item());
    }

    return retList;
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::firstItem(const CT_InAbstractSingularItemModel *model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_InStdModelPossibility*> list = model->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        QString modelName = it.next()->outModel()->uniqueName();

        CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            return ct->item();

        ct = _itemsBackup.value(modelName, NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            return ct->item();
    }

    return NULL;
}

const QHash<QString, CT_GroupItemDrawableContainer*>& CT_AbstractStandardItemGroup::itemsNew() const
{
    return _itemsNew;
}

const QHash<QString, CT_GroupItemDrawableContainer*>& CT_AbstractStandardItemGroup::itemsBackup() const
{
    return _itemsBackup;
}

const QHash<QString, CT_Container*>& CT_AbstractStandardItemGroup::groupsNew() const
{
    return _groupsNew;
}

const QHash<QString, CT_Container*>& CT_AbstractStandardItemGroup::groupsBackup() const
{
    return _groupsBackup;
}

QList<CT_ItemDrawableConfiguration*> CT_AbstractStandardItemGroup::getDependantViewConfigurations()
{
    QList<CT_ItemDrawableConfiguration*> list;

    if(model() != NULL)
        recursiveGetDependantViewConfigurations(list, (CT_OutAbstractGroupModel*)model());

    return list;
}

void CT_AbstractStandardItemGroup::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    QMutexLocker locker(_mutexAccessGroup);

    if(getAlternativeDrawManager() != NULL)
        getAlternativeDrawManager()->draw(view, painter, *this);
    else if(getBaseDrawManager() != NULL)
        getBaseDrawManager()->draw(view, painter, *this);
}

/////////// PROTECTED //////////////

bool CT_AbstractStandardItemGroup::protectedAddGroup(CT_AbstractItemGroup *group)
{
    QMutexLocker locker(_mutexAccessGroup);

    CT_Container *container = _groupsNew.value(group->model()->uniqueName(), NULL);

    if(container == NULL)
    {
        container = new CT_Container(NULL, result(), group->getType());
        _groupsNew.insert(group->model()->uniqueName(), container);
    }

    bool checkExistInContainer = false;

    if(group->willBeRemovedLater()) {
        staticUndoWillBeRemovedLater(group);
        checkExistInContainer = true;
    }

    group->setParentGroup(this);
    group->setParentContainer(container);

    if(!checkExistInContainer || (checkExistInContainer && !container->contains(group)))
        container->addItemDrawable(group);

    return true;
}

bool CT_AbstractStandardItemGroup::protectedAddItemDrawable(CT_AbstractSingularItemDrawable *item)
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    if(_itemsNew.contains(item->model()->uniqueName()))
        return false;

    _itemsNew.insert(item->model()->uniqueName(), new CT_GroupItemDrawableContainer(item));

    return true;
}

bool CT_AbstractStandardItemGroup::copyStructure(CT_AbstractStandardItemGroup *itemGroup)
{
    QMutexLocker locker(_mutexAccessGroup);

    const CT_OutAbstractGroupModel *newModel = (const CT_OutAbstractGroupModel *)itemGroup->model();

    if(newModel != NULL)
    {
        CT_AbstractResult *result = itemGroup->result();

        // copie des groupes backup de ce groupe vers les groupes backup du nouveau groupe
        if(!copyGroups(result, *newModel, _groupsBackup, itemGroup->_groupsBackup, *itemGroup))
        {
            delete itemGroup;
            return NULL;
        }

        // copie des groupes new de ce groupe vers les groupes backup du nouveau groupe
        if(!copyGroups(result, *newModel, _groupsNew, itemGroup->_groupsBackup, *itemGroup))
        {
            delete itemGroup;
            return NULL;
        }

        // copie des items backup de ce groupe vers les items backup du nouveau groupe
        if(!copyItems(_itemsBackup, *newModel, *itemGroup))
        {
            delete itemGroup;
            return NULL;
        }

        // copie des items new de ce groupe vers les items backup du nouveau groupe
        if(!copyItems(_itemsNew, *newModel, *itemGroup))
        {
            delete itemGroup;
            return NULL;
        }
    }

    return itemGroup;
}

bool CT_AbstractStandardItemGroup::copyGroups(CT_AbstractResult *result,
                                      const CT_OutAbstractGroupModel &modelCopy,
                                      const QHash<QString, CT_Container*> &mapSource,
                                      QHash<QString, CT_Container*> &mapDest,
                                      CT_AbstractStandardItemGroup &groupDest)
{
    CT_ResultCopyModeList copyMode;

    CT_AbstractItemGroup            *groupSource;
    CT_Container                    *containerSource;

    CT_OutAbstractGroupModel        *groupModelCopy;
    CT_AbstractItemGroup            *groupCopy;
    CT_Container                    *containerCopy;

    QHashIterator<QString, CT_Container*> itG(mapSource);

    while(itG.hasNext())
    {
        itG.next();

        // on recherche dans le nouveau modèle le groupe modèle fils correspondant au groupe modèle fils source
        groupModelCopy = (CT_OutAbstractGroupModel*)modelCopy.findGroup(itG.key());

        // si il existe dans le nouveau modèle
        if(groupModelCopy != NULL)
        {
            // on crée un nouveau container qui va contenir la copie des groupes du container source
            containerCopy = new CT_Container(NULL, result, groupModelCopy->group()->getType());
            // les groupes devront être supprimer de la mémoire
            containerCopy->setAutoDelete(true);

            // on l'insere dans la map de destination
            mapDest.insert(itG.key(), containerCopy);

            // on récupère le container source
            containerSource = itG.value();

            // on commence le parcours de la liste des groupes
            CT_ContainerIterator itC(containerSource);

            while(itC.hasNext())
            {
                groupSource = dynamic_cast<CT_AbstractItemGroup*>((CT_AbstractItemDrawable*)itC.next());
                groupCopy = (CT_AbstractItemGroup*)groupSource->copy(groupModelCopy, result, copyMode);

                // si la copie à échouer car le nouveau modèle n'est pas complet on s'arrête
                if(groupCopy == NULL)
                    return false;

                // sinon on l'ajoute au nouveau container
                containerCopy->addItemDrawable(groupCopy);
                groupCopy->setParentGroup(&groupDest);
                groupCopy->setParentContainer(containerCopy);
            }
        }
    }

    return true;
}

bool CT_AbstractStandardItemGroup::copyItems(const QHash<QString, CT_GroupItemDrawableContainer*> &mapSource,
                                     const CT_OutAbstractGroupModel &groupModelCopy,
                                     CT_AbstractStandardItemGroup &groupDest)
{
    QHashIterator<QString, CT_GroupItemDrawableContainer*> itI(mapSource);

    while(itI.hasNext())
    {
        itI.next();

        CT_OutAbstractSingularItemModel *itemModelCopy = groupModelCopy.findItem(itI.key());

        // on l'insere dans la map backup du nouveau groupe si le modèle de l'item existe dans le nouveau modèle du groupe
        if(itemModelCopy != NULL)
            groupDest._itemsBackup.insert(itI.key(), itI.value()->copy());
    }

    return true;
}

void CT_AbstractStandardItemGroup::recursiveGetDependantViewConfigurations(QList<CT_ItemDrawableConfiguration*> &list, CT_OutAbstractGroupModel* gModel)
{
    QListIterator<CT_OutAbstractSingularItemModel*> itI(gModel->items());

    while(itI.hasNext())
    {
        CT_OutAbstractSingularItemModel *iModel = itI.next();

        if(!list.contains(iModel->itemDrawable()->getViewConfiguration()))
            list.append(iModel->itemDrawable()->getViewConfiguration());
    }

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(gModel->groups());

    while(itG.hasNext())
        recursiveGetDependantViewConfigurations(list, (CT_OutAbstractGroupModel*)itG.next());
}

void CT_AbstractStandardItemGroup::recursiveSetComplete()
{
    QMutableListIterator<CT_AbstractItemDrawable*> *itI;

    // delete new groups that must be removed
    QMutableHashIterator<QString, CT_Container*> it(_groupsNew);

    while(it.hasNext())
    {
        it.next();

        CT_Container *c = it.value();

        itI = new QMutableListIterator<CT_AbstractItemDrawable*>(*c->getList());

        while(itI->hasNext())
        {
            CT_AbstractStandardItemGroup *gr = (CT_AbstractStandardItemGroup*)itI->next();

            if(gr->willBeRemovedLater())
            {
                itI->remove();
                delete gr;
            }
            else if(gr->atLeastOneChildMustBeRemovedLater())
            {
                gr->recursiveSetComplete();
            }
        }

        delete itI;

        if(c->getList()->isEmpty())
        {
            it.remove();
            delete c;
        }
    }

    // delete backup groups that must be removed

    it = _groupsBackup;

    while(it.hasNext())
    {
        it.next();

        CT_Container *c = it.value();

        itI = new QMutableListIterator<CT_AbstractItemDrawable*>(*c->getList());

        while(itI->hasNext())
        {
            CT_AbstractItemGroup *gr = (CT_AbstractItemGroup*)itI->next();

            if(gr->willBeRemovedLater())
            {
                itI->remove();
                delete gr;
            }
            else if(gr->atLeastOneChildMustBeRemovedLater())
            {
                CT_AbstractItemGroup::staticResursiveSetComplete(gr);
            }
        }

        delete itI;

        if(c->getList()->isEmpty())
        {
            it.remove();
            delete c;
        }
    }

    // remove backup items that must be removed
    QMutableHashIterator<QString, CT_GroupItemDrawableContainer*> itB(_itemsBackup);

    while(itB.hasNext())
    {
        itB.next();

        CT_GroupItemDrawableContainer *ct = itB.value();

        if(ct->willBeRemovedLater())
        {
            itB.remove();
            delete ct;
        }
    }

    // delete new items that must be removed
    itB = _itemsNew;

    while(itB.hasNext())
    {
        itB.next();

        CT_GroupItemDrawableContainer *ct = itB.value();

        if(ct->willBeRemovedLater())
        {
            itB.remove();
            delete ct;
        }
    }
}

void CT_AbstractStandardItemGroup::internalSetWillBeRemovedFromResult(const CT_AbstractResult *result)
{
    QHashIterator<QString, CT_Container*> it(_groupsNew);

    while(it.hasNext())
        CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(it.next().value(), result);

    it = _groupsBackup;

    while(it.hasNext())
        CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(it.next().value(), result);

    QHashIterator<QString, CT_GroupItemDrawableContainer*> itB(_itemsBackup);

    while(itB.hasNext())
        CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(itB.next().value()->item(), result);

    itB = _itemsNew;

    while(itB.hasNext())
        CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(itB.next().value()->item(), result);
}
