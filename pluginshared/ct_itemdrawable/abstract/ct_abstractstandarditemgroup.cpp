#include "ct_abstractstandarditemgroup.h"
#include <assert.h>

#include "ct_global/ct_context.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

const CT_StandardStandardItemGroupDrawManager CT_AbstractStandardItemGroup::SIG_DRAW_MANAGER;

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup() : CT_AbstractItemGroup()
{
    _itChildGroupsNew = NULL;
    _itChildGroupsBackup = NULL;
    _itChildItemsNew = NULL;
    _itChildItemsBackup = NULL;

    m_gIterator = NULL;
    m_iIterator = NULL;
    m_mgIterator = NULL;
    m_miIterator = NULL;

    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup(const CT_OutAbstractGroupModel *model,
                                                           const CT_AbstractResult *result) : CT_AbstractItemGroup(model, result)
{
    _itChildGroupsNew = NULL;
    _itChildGroupsBackup = NULL;
    _itChildItemsNew = NULL;
    _itChildItemsBackup = NULL;

    m_gIterator = NULL;
    m_iIterator = NULL;
    m_mgIterator = NULL;
    m_miIterator = NULL;

    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::CT_AbstractStandardItemGroup(const QString &modelName,
                                                           const CT_AbstractResult *result) : CT_AbstractItemGroup(modelName, result)
{
    _itChildGroupsNew = NULL;
    _itChildGroupsBackup = NULL;
    _itChildItemsNew = NULL;
    _itChildItemsBackup = NULL;

    m_gIterator = NULL;
    m_iIterator = NULL;
    m_mgIterator = NULL;
    m_miIterator = NULL;

    _mutexAccessGroup = new QMutex(QMutex::Recursive);
    _mutexAccessItem = new QMutex(QMutex::Recursive);

    setBaseDrawManager(&SIG_DRAW_MANAGER);
}

CT_AbstractStandardItemGroup::~CT_AbstractStandardItemGroup()
{
    delete _mutexAccessGroup;
    delete _mutexAccessItem;

    delete _itChildGroupsNew;
    delete _itChildGroupsBackup;
    delete _itChildItemsNew;
    delete _itChildItemsBackup;

    delete m_gIterator;
    delete m_iIterator;
    delete m_mgIterator;
    delete m_miIterator;

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

bool CT_AbstractStandardItemGroup::containsItemDrawable(const CT_InAbstractSingularItemModel &model) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_InStdModelPossibility*> list = model.getPossibilitiesSaved();
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

bool CT_AbstractStandardItemGroup::containsItemDrawable(const CT_InAbstractSingularItemModel *model) const
{
    assert(model != NULL);
    return containsItemDrawable(*model);
}

bool CT_AbstractStandardItemGroup::containsItemDrawable(const QVector<CT_OutAbstractSingularItemModel*> &outModelList) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QString modelName;
    int size = outModelList.size();

    for(int i=0; i<size; ++i)
    {
        modelName = outModelList[i]->uniqueName();

        CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

        if(ct == NULL)
        {
            ct = _itemsBackup.value(modelName, NULL);

            if((ct != NULL) && !ct->willBeRemovedLater())
                return true;
        }
        else if(!ct->willBeRemovedLater())
        {
            return true;
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

int CT_AbstractStandardItemGroup::nItemDrawable() const
{
    return nItems();
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
    QHashIterator<QString, CT_GroupItemDrawableContainer*> it(constItemsNew());

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    it = constItemsBackup();

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    return l;
}

bool CT_AbstractStandardItemGroup::beginIterateItems(const CT_InAbstractSingularItemModel &model, bool mutableIterator)
{
    delete m_iIterator;
    m_iIterator = NULL;

    delete m_miIterator;
    m_miIterator = NULL;

    if(mutableIterator)
    {
        m_miIterator = new CT_MutableItemIterator(this, &model);
        return m_miIterator->hasNext();
    }

    m_iIterator = new CT_ItemIterator(this, &model);
    return m_iIterator->hasNext();
}

bool CT_AbstractStandardItemGroup::beginIterateItems(const CT_InAbstractSingularItemModel *model, bool mutableIterator)
{
    assert(model != NULL);
    return beginIterateItems(*model, mutableIterator);
}

bool CT_AbstractStandardItemGroup::beginIterateItems(bool mutableIterator)
{
    delete m_iIterator;
    m_iIterator = NULL;

    delete m_miIterator;
    m_miIterator = NULL;

    if(mutableIterator)
    {
        m_miIterator = new CT_MutableItemIterator(this);
        return m_miIterator->hasNext();
    }

    m_iIterator = new CT_ItemIterator(this);
    return m_iIterator->hasNext();
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::nextItem()
{
    if(m_iIterator != NULL)
    {
        if(m_iIterator->hasNext())
            return (CT_AbstractSingularItemDrawable*)m_iIterator->next();

        return NULL;
    }

    if(m_miIterator != NULL)
    {
        if(m_miIterator->hasNext())
            return m_miIterator->next();

        return NULL;
    }

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::currentItem() const
{
    if(m_iIterator != NULL)
        return (CT_AbstractSingularItemDrawable*)m_iIterator->current();

    if(m_miIterator != NULL)
        return m_miIterator->current();

    return NULL;
}

bool CT_AbstractStandardItemGroup::removeCurrentItem()
{
    if(m_miIterator != NULL)
    {
        m_miIterator->remove();
        return true;
    }

    return false;
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

QList<CT_AbstractSingularItemDrawable*> CT_AbstractStandardItemGroup::findItem(const CT_InAbstractSingularItemModel &model,
                                                               CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_AbstractSingularItemDrawable*> retList;

    QList<CT_InStdModelPossibility*> list = model.getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        QString modelName = it.next()->outModel()->uniqueName();

        if(searchMode & S_New)
        {
            CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

            if((ct != NULL) && !ct->willBeRemovedLater())
                retList.append(ct->item());
        }

        if(searchMode & S_Backup)
        {
            CT_GroupItemDrawableContainer *ct = _itemsBackup.value(modelName, NULL);

            if((ct != NULL) && !ct->willBeRemovedLater())
                retList.append(ct->item());
        }
    }

    return retList;
}

QList<CT_AbstractSingularItemDrawable*> CT_AbstractStandardItemGroup::findItem(const CT_InAbstractSingularItemModel *model,
                                                               CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findItem(*model, searchMode);
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_InAbstractSingularItemModel &model,
                                                             CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_InStdModelPossibility*> list = model.getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        QString modelName = it.next()->outModel()->uniqueName();

        if(searchMode & S_New)
        {
            CT_GroupItemDrawableContainer *ct = _itemsNew.value(modelName, NULL);

            if((ct != NULL) && !ct->willBeRemovedLater())
                return ct->item();
        }

        if(searchMode & S_Backup)
        {
            CT_GroupItemDrawableContainer *ct = _itemsBackup.value(modelName, NULL);

            if((ct != NULL) && !ct->willBeRemovedLater())
                return ct->item();
        }
    }

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_InAbstractSingularItemModel *model,
                                                             CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findFirstItem(*model, searchMode);
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_InAbstractSingularItemModel &model) const
{
    return findFirstItem(model, (CT_AbstractItemGroup::S_Backup | CT_AbstractItemGroup::S_New));
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_InAbstractSingularItemModel *model) const
{
    assert(model != NULL);
    return findFirstItem(model, (CT_AbstractItemGroup::S_Backup | CT_AbstractItemGroup::S_New));
}

QList<CT_AbstractSingularItemDrawable*> CT_AbstractStandardItemGroup::findItem(const CT_OutAbstractSingularItemModel &model,
                                                               CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    QList<CT_AbstractSingularItemDrawable*> retList;

    if(searchMode & S_New)
    {
        CT_GroupItemDrawableContainer *ct = _itemsNew.value(model.uniqueName(), NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            retList.append(ct->item());
    }

    if(searchMode & S_Backup)
    {
        CT_GroupItemDrawableContainer *ct = _itemsBackup.value(model.uniqueName(), NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            retList.append(ct->item());
    }

    return retList;
}

QList<CT_AbstractSingularItemDrawable*> CT_AbstractStandardItemGroup::findItem(const CT_OutAbstractSingularItemModel *model,
                                                               CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findItem(*model, searchMode);
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_OutAbstractSingularItemModel &model,
                                                                     CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessItem));

    if(searchMode & S_New)
    {
        CT_GroupItemDrawableContainer *ct = _itemsNew.value(model.uniqueName(), NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            return ct->item();
    }

    if(searchMode & S_Backup)
    {
        CT_GroupItemDrawableContainer *ct = _itemsBackup.value(model.uniqueName(), NULL);

        if((ct != NULL) && !ct->willBeRemovedLater())
            return ct->item();
    }

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_OutAbstractSingularItemModel *model,
                                                                     CT_AbstractStandardItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findFirstItem(*model, searchMode);
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_OutAbstractSingularItemModel &model) const
{
    return findFirstItem(model, (CT_AbstractItemGroup::S_Backup | CT_AbstractItemGroup::S_New));
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::findFirstItem(const CT_OutAbstractSingularItemModel *model) const
{
    assert(model != NULL);
    return findFirstItem(*model, (CT_AbstractItemGroup::S_Backup | CT_AbstractItemGroup::S_New));
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
    QHashIterator<QString, CT_Container*> it(constGroupsNew());

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    it = constGroupsBackup();

    while(it.hasNext())
    {
        it.next();
        l.insert(it.key(), it.value());
    }

    return l;
}

bool CT_AbstractStandardItemGroup::beginIterateGroups(const CT_InAbstractGroupModel &inModel, bool mutableIterator)
{
    delete m_gIterator;
    m_gIterator = NULL;

    delete m_mgIterator;
    m_mgIterator = NULL;

    if(mutableIterator)
    {
        m_mgIterator = new CT_MutableGroupIterator(this, &inModel);
        return m_mgIterator->hasNext();
    }

    m_gIterator = new CT_GroupIterator(this, &inModel);
    return m_gIterator->hasNext();
}

bool CT_AbstractStandardItemGroup::beginIterateGroups(const CT_InAbstractGroupModel *inModel, bool mutableIterator)
{
    assert(inModel != NULL);
    return beginIterateGroups(*inModel, mutableIterator);
}

bool CT_AbstractStandardItemGroup::beginIterateGroups(const CT_OutAbstractGroupModel &outModel, bool mutableIterator)
{
    delete m_gIterator;
    m_gIterator = NULL;

    delete m_mgIterator;
    m_mgIterator = NULL;

    if(mutableIterator)
    {
        m_mgIterator = new CT_MutableGroupIterator(this, &outModel);
        return m_mgIterator->hasNext();
    }

    m_gIterator = new CT_GroupIterator(this, &outModel);
    return m_gIterator->hasNext();
}

bool CT_AbstractStandardItemGroup::beginIterateGroups(const CT_OutAbstractGroupModel *outModel, bool mutableIterator)
{
    assert(outModel != NULL);
    return beginIterateGroups(*outModel, mutableIterator);
}

bool CT_AbstractStandardItemGroup::beginIterateGroups(bool mutableIterator)
{
    delete m_gIterator;
    m_gIterator = NULL;

    delete m_mgIterator;
    m_mgIterator = NULL;

    if(mutableIterator)
    {
        m_mgIterator = new CT_MutableGroupIterator(this);
        return m_mgIterator->hasNext();
    }

    m_gIterator = new CT_GroupIterator(this);
    return m_gIterator->hasNext();
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::nextGroup()
{
    if(m_gIterator != NULL)
    {
        if(m_gIterator->hasNext())
            return (CT_AbstractItemGroup*)m_gIterator->next();

        return NULL;
    }

    if(m_mgIterator != NULL)
    {
        if(m_mgIterator->hasNext())
            return m_mgIterator->next();

        return NULL;
    }

    return NULL;
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::currentGroup() const
{
    if(m_gIterator != NULL)
        return (CT_AbstractItemGroup*)m_gIterator->current();

    if(m_mgIterator != NULL)
        return m_mgIterator->current();

    return NULL;
}

bool CT_AbstractStandardItemGroup::removeCurrentGroup()
{
    if(m_mgIterator != NULL)
    {
        m_mgIterator->remove();
        return true;
    }

    return false;
}

bool CT_AbstractStandardItemGroup::removeGroup(CT_AbstractItemGroup *group)
{
    return ((CT_ResultGroup*)result())->removeGroupSomethingInStructure(group);
}

QList<CT_AbstractItemGroup*> CT_AbstractStandardItemGroup::findGroup(const CT_InAbstractGroupModel &model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    QList<CT_AbstractItemGroup*> retList;

    QList<CT_InStdModelPossibility*> list = model.getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        CT_Container *container = NULL;
        QString modelName = it.next()->outModel()->uniqueName();

        if(searchMode & S_New)
        {
            container = _groupsNew.value(modelName, NULL);

            if(container != NULL)
            {
                QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

                while(itG.hasNext())
                {
                    CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                    if(!gg->willBeRemovedLater())
                        retList.append(gg);
                }
            }
        }

        if(searchMode & S_Backup)
        {
            container = _groupsBackup.value(modelName, NULL);

            if(container != NULL)
            {
                QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

                while(itG.hasNext())
                {
                    CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                    if(!gg->willBeRemovedLater())
                        retList.append(gg);
                }
            }
        }
    }

    return retList;
}

QList<CT_AbstractItemGroup*> CT_AbstractStandardItemGroup::findGroup(const CT_InAbstractGroupModel *model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findGroup(*model, searchMode);
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_InAbstractGroupModel &model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    QList<CT_InStdModelPossibility*> list = model.getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
    {
        CT_Container *container = NULL;
        QString modelName = it.next()->outModel()->uniqueName();

        if(searchMode & S_New)
        {
            container = _groupsNew.value(modelName, NULL);

            if(container != NULL)
            {
                QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

                while(itG.hasNext())
                {
                    CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                    if(!gg->willBeRemovedLater())
                        return gg;
                }
            }
        }

        if(searchMode & S_Backup)
        {
            container = _groupsBackup.value(modelName, NULL);

            if(container != NULL)
            {
                QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

                while(itG.hasNext())
                {
                    CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                    if(!gg->willBeRemovedLater())
                        return gg;
                }
            }
        }
    }

    return NULL;
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_InAbstractGroupModel *model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findFirstGroup(*model, searchMode);
}

QList<CT_AbstractItemGroup*> CT_AbstractStandardItemGroup::findGroup(const CT_OutAbstractGroupModel &model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    QList<CT_AbstractItemGroup*> retList;

    if(searchMode & S_New)
    {
        CT_Container *container = _groupsNew.value(model.uniqueName(), NULL);

        if(container != NULL)
        {
            QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

            while(itG.hasNext())
            {
                CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                if(!gg->willBeRemovedLater())
                    retList.append(gg);
            }
        }
    }

    if(searchMode & S_Backup)
    {
        CT_Container *container = _groupsBackup.value(model.uniqueName(), NULL);

        if(container != NULL)
        {
            QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

            while(itG.hasNext())
            {
                CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                if(!gg->willBeRemovedLater())
                    retList.append(gg);
            }
        }
    }

    return retList;
}

QList<CT_AbstractItemGroup*> CT_AbstractStandardItemGroup::findGroup(const CT_OutAbstractGroupModel *model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findGroup(*model, searchMode);
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_OutAbstractGroupModel &model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    QMutexLocker locker(const_cast<QMutex*>(_mutexAccessGroup));

    if(searchMode & S_New)
    {
        CT_Container *container = _groupsNew.value(model.uniqueName(), NULL);

        if(container != NULL)
        {
            QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

            while(itG.hasNext())
            {
                CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                if(!gg->willBeRemovedLater())
                    return gg;
            }
        }
    }

    if(searchMode & S_Backup)
    {
        CT_Container *container = _groupsBackup.value(model.uniqueName(), NULL);

        if(container != NULL)
        {
            QListIterator<CT_AbstractItemDrawable*> itG(*container->getList());

            while(itG.hasNext())
            {
                CT_AbstractItemGroup *gg = (CT_AbstractItemGroup*)itG.next();

                if(!gg->willBeRemovedLater())
                    return gg;
            }
        }
    }

    return NULL;
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_OutAbstractGroupModel *model, CT_AbstractItemGroup::SearchMode searchMode) const
{
    assert(model != NULL);
    return findFirstGroup(*model, searchMode);
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_OutAbstractGroupModel &model) const
{
    return findFirstGroup(model, (CT_AbstractItemGroup::S_New | CT_AbstractItemGroup::S_Backup));
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::findFirstGroup(const CT_OutAbstractGroupModel *model) const
{
    assert(model != NULL);
    return findFirstGroup(*model, (CT_AbstractItemGroup::S_New | CT_AbstractItemGroup::S_Backup));
}


// Ajout AP 24/07/2013
CT_AbstractItemGroup* CT_AbstractStandardItemGroup::beginGroup(const CT_InAbstractGroupModel *model, bool mutableIterator)
{
    if (!beginIterateGroups(model, mutableIterator))
    {
        return NULL;
    } else {
        return nextGroup();
    }
}

CT_AbstractItemGroup* CT_AbstractStandardItemGroup::beginGroup(const CT_OutAbstractGroupModel *model, bool mutableIterator)
{
    if (!beginIterateGroups(model, mutableIterator))
    {
        return NULL;
    } else {
        return nextGroup();
    }
}

CT_AbstractSingularItemDrawable* CT_AbstractStandardItemGroup::beginItem(const CT_InAbstractSingularItemModel *model, bool mutableIterator)
{
    if (!beginIterateItems(model, mutableIterator))
    {
        return NULL;
    } else {
        return nextItem();
    }
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

// Fin d'ajout AP 24/07/2013

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

const QHash<QString, CT_GroupItemDrawableContainer*>& CT_AbstractStandardItemGroup::constItemsNew() const
{
    return _itemsNew;
}

const QHash<QString, CT_GroupItemDrawableContainer*>& CT_AbstractStandardItemGroup::constItemsBackup() const
{
    return _itemsBackup;
}

const QHash<QString, CT_Container*>& CT_AbstractStandardItemGroup::constGroupsNew() const
{
    return _groupsNew;
}

const QHash<QString, CT_Container*>& CT_AbstractStandardItemGroup::constGroupsBackup() const
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
    else
        getBaseDrawManager()->draw(view, painter, *this);
}

bool CT_AbstractStandardItemGroup::hasChildren() const
{
    return !isEmpty();
}

bool CT_AbstractStandardItemGroup::beginIterateChild()
{
    if(hasChildren())
    {
        delete _itChildGroupsNew;
        _itChildGroupsNew = new QHashIterator<QString, CT_Container*>(_groupsNew);

        delete _itChildGroupsBackup;
        _itChildGroupsBackup = new QHashIterator<QString, CT_Container*>(_groupsBackup);

        delete _itChildItemsNew;
        _itChildItemsNew = new QHashIterator<QString, CT_GroupItemDrawableContainer*>(_itemsNew);

        delete _itChildItemsBackup;
        _itChildItemsBackup = new QHashIterator<QString, CT_GroupItemDrawableContainer*>(_itemsBackup);

        return true;
    }

    return false;
}

CT_AbstractItemDrawable* CT_AbstractStandardItemGroup::nextChild()
{
    if(_itChildGroupsNew->hasNext())
        return _itChildGroupsNew->next().value();

    if(_itChildGroupsBackup->hasNext())
        return _itChildGroupsBackup->next().value();

    if(_itChildItemsNew->hasNext())
        return _itChildItemsNew->next().value()->item();

    if(_itChildItemsBackup->hasNext())
        return _itChildItemsBackup->next().value()->item();

    return NULL;
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

    group->setParentGroup(this);
    group->setParentContainer(container);
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
            containerSource->beginIterateItemDrawable();

            while((groupSource = dynamic_cast<CT_AbstractItemGroup*>(containerSource->nextItemDrawable())) != NULL)
            {
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

        if(!list.contains(iModel->item()->getViewConfiguration()))
            list.append(iModel->item()->getViewConfiguration());
    }

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(gModel->groups());

    while(itG.hasNext())
        recursiveGetDependantViewConfigurations(list, (CT_OutAbstractGroupModel*)itG.next());
}

void CT_AbstractStandardItemGroup::setColorToGroups(const QHash<QString, CT_Container*> &groups, const QColor &color)
{
    QHashIterator<QString, CT_Container*> itG(groups);

    while(itG.hasNext())
    {
        itG.next();

        QListIterator<CT_AbstractItemDrawable*> it(*itG.value()->getList());

        while(it.hasNext())
        {
            it.next()->setColor(color);
        }
    }
}

void CT_AbstractStandardItemGroup::setColorToItems(const QHash<QString, CT_GroupItemDrawableContainer*> &items, const QColor &color)
{
    QHashIterator<QString, CT_GroupItemDrawableContainer*> itI(items);

    while(itI.hasNext())
    {
        itI.next();
        itI.value()->item()->setColor(color);
    }
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


// SLOTS //

void CT_AbstractStandardItemGroup::setColor(QColor color)
{
    if(this->color() != color)
    {
        CT_AbstractItemGroup::setColor(color);

        setColorToGroups(constGroupsNew(), color);
        setColorToGroups(constGroupsBackup(), color);
        setColorToItems(constItemsNew(), color);
        setColorToItems(constItemsBackup(), color);

    }
}
