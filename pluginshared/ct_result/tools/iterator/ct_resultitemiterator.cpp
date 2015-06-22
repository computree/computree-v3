#include "ct_resultitemiterator.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/tools/iterator/private/ct_treestructureforiterator.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_ResultItemIterator::CT_ResultItemIterator()
{
    m_result = NULL;
    m_rootTree = NULL;
    m_hasNextCalled = false;
    m_current = NULL;
    m_currentInTree = m_rootTree;
}

CT_ResultItemIterator::CT_ResultItemIterator(const CT_ResultGroup *result)
{
    m_result = (CT_ResultGroup*)result;
    m_rootTree = NULL;

    if(!m_result->groups().isEmpty())
    {
        CT_OutAbstractResultModelGroup *outModel = (CT_OutAbstractResultModelGroup*)m_result->model();

        CT_TreeStructureConstructor constructor;

        if(outModel->rootGroup() != NULL)
            recursiveCreateStructureForAllItems(outModel->rootGroup(), &constructor);

        CT_TreeStructureForIterator *rootTree = constructor.takeStructure();

        // the structure must contains at least one group and this group
        // must have the same model that root groups of the result
        if((rootTree != NULL)
                && (m_result->groups().first()->model()->uniqueName() == rootTree->outModel()->uniqueName()))
        {
            m_rootTree = rootTree;
            m_rootTree->setIterator(new QListIterator<CT_AbstractItemGroup*>(m_result->groups()));
        }
        // otherwise we don't iterate
        else
        {
            delete rootTree;
        }
    }

    m_hasNextCalled = false;
    m_current = NULL;

    m_currentInTree = m_rootTree;
}

CT_ResultItemIterator::CT_ResultItemIterator(const CT_ResultGroup *result,
                                             const CT_InAbstractSingularItemModel *inModel)
{
    initIterator(result, inModel);
}

CT_ResultItemIterator::CT_ResultItemIterator(const CT_ResultGroup *result,
                                             const CT_OutAbstractSingularItemModel *outModel)
{
    initIterator(result, outModel);
}

CT_ResultItemIterator::CT_ResultItemIterator(const CT_ResultGroup *result,
                                             const CT_VirtualAbstractStep *step,
                                             const QString &modelName)
{

    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, result, step);

    Q_ASSERT_X(model != NULL, "CT_ResultItemIterator constructor", "You created a CT_ResultItemIterator with a modelName but the model was not found");

    CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);
    CT_OutAbstractSingularItemModel *outModel = NULL;

    if(inModel != NULL)
        initIterator(result, inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model)) != NULL)
        initIterator(result, outModel);
    else
        qFatal("You created a CT_ResultItemIterator with a modelName but the model was not a singular item model");
}

CT_ResultItemIterator::~CT_ResultItemIterator()
{
    if(m_rootTree != NULL)
    {
        delete (QListIterator<CT_AbstractItemGroup*>*)m_rootTree->iterator();

        // recursive delete iterator
        QListIterator<CT_TreeStructureForIterator*> it(m_rootTree->children());

        while(it.hasNext())
            recursiveDeleteIterator(it.next());

        delete m_rootTree;
    }
}

bool CT_ResultItemIterator::hasNext() const
{
    if(m_currentInTree == NULL)
        return false;

    if(!m_hasNextCalled)
    {
        m_current = NULL;

        bool again = true;

        do
        {
            if(m_currentInTree == m_rootTree)
            {
                again = false;

                if(!hasNextT< QListIterator<CT_AbstractItemGroup*> >((QListIterator<CT_AbstractItemGroup*>*)m_currentInTree->iterator()))
                    return false;
            }
            else
            {
                if(m_currentInTree->isAGroup())
                {
                    if(hasNextT< CT_GroupIterator >((CT_GroupIterator*)m_currentInTree->iterator()))
                        again = false;
                }
                else
                {
                    m_currentInTree = m_currentInTree->parent();

                    if(m_currentInTree->isLastIncrementChildIndexSuccess())
                    {
                        CT_TreeStructureForIterator *backup = m_currentInTree;

                        while(again
                              && m_currentInTree->incrementChildIndexAndSetCurrentChild())
                        {
                            if(hasNextWithCurrentChild())
                                again = false;
                            else
                                m_currentInTree = backup;
                        }
                    }

                    if(again)
                    {
                        m_currentInTree->resetChildIndexAndSetCurrentChild();
                        m_currentInTree->resetIncrementChildIndexSuccess();
                    }
                }
            }

        }while(again);
    }

    m_hasNextCalled = true;
    return true;
}

const CT_AbstractSingularItemDrawable* CT_ResultItemIterator::next()
{
    if(!m_hasNextCalled)
    {
        if(hasNext())
        {
            const CT_AbstractSingularItemDrawable *item = (const CT_AbstractSingularItemDrawable*)m_currentInTree->currentItem();
            m_currentInTree->setCurrentItem(NULL);

            m_hasNextCalled = false;

            return item;
        }

        return NULL;
    }

    m_hasNextCalled = false;
    return (const CT_AbstractSingularItemDrawable*)m_currentInTree->currentItem();
}

void CT_ResultItemIterator::initIterator(const CT_ResultGroup *result, const CT_InAbstractSingularItemModel *inModel)
{
    m_result = (CT_ResultGroup*)result;
    m_rootTree = NULL;

    m_hasNextCalled = false;
    m_current = NULL;

    const QList<CT_AbstractItemGroup*> &groups = m_result->groups();

    if(!groups.isEmpty())
    {
        CT_TreeStructureConstructor constructor;

        // création de la structure à parcourir en fonction des possibilités sélectionnées du modèle d'entrée
        QList<CT_InStdModelPossibility*> savedChecked = inModel->getPossibilitiesSavedSelected();

        QListIterator<CT_InStdModelPossibility*> itChecked(savedChecked);

        while(itChecked.hasNext())
            constructor.addModelsForItemsToStructure(itChecked.next());

        CT_TreeStructureForIterator *rootTree = constructor.takeStructure();

        // the structure must contains at least one group and this group
        // must have the same model that root groups of the result
        if((rootTree != NULL)
                && (groups.first()->model()->uniqueName() == rootTree->outModel()->uniqueName()))
        {
            // we must iterate over root groups whatever happens
            QListIterator<CT_AbstractItemGroup*> *it = new QListIterator<CT_AbstractItemGroup*>(groups);

            m_rootTree = rootTree;
            m_rootTree->setIterator(it);
        }
        // otherwise we don't iterate
        else
        {
            delete rootTree;
        }
    }

    m_currentInTree = m_rootTree;
}

void CT_ResultItemIterator::initIterator(const CT_ResultGroup *result, const CT_OutAbstractSingularItemModel *outModel)
{
    m_result = (CT_ResultGroup*)result;
    m_rootTree = NULL;

    m_hasNextCalled = false;
    m_current = NULL;

    const QList<CT_AbstractItemGroup*> &groups = m_result->groups();

    if(!groups.isEmpty())
    {
        CT_TreeStructureConstructor constructor;

        constructor.addModelsForItemsToStructure((CT_OutAbstractSingularItemModel*)outModel);

        CT_TreeStructureForIterator *rootTree = constructor.takeStructure();

        // the structure must contains at least one group and this group
        // must have the same model that root groups of the result
        if((rootTree != NULL)
                && (groups.first()->model()->uniqueName() == rootTree->outModel()->uniqueName()))
        {
            // we must iterate over root groups whatever happens
            QListIterator<CT_AbstractItemGroup*> *it = new QListIterator<CT_AbstractItemGroup*>(groups);

            m_rootTree = rootTree;
            m_rootTree->setIterator(it);
        }
        // otherwise we don't iterate
        else
        {
            delete rootTree;
        }
    }

    m_currentInTree = m_rootTree;
}

void CT_ResultItemIterator::recursiveCreateStructureForAllItems(DEF_CT_AbstractGroupModelOut *groupModel, CT_TreeStructureConstructor *constructor)
{
    QListIterator<CT_OutAbstractSingularItemModel*> it(groupModel->items());

    while(it.hasNext())
        constructor->addModelsForItemsToStructure(it.next());

    QListIterator<DEF_CT_AbstractGroupModelOut*> itG(groupModel->groups());

    while(itG.hasNext())
        recursiveCreateStructureForAllItems(itG.next(), constructor);
}

void CT_ResultItemIterator::recursiveDeleteIterator(CT_TreeStructureForIterator *tsfi)
{
    delete ((CT_GroupIterator*)tsfi->iterator());
    tsfi->setIterator(NULL);

    QListIterator<CT_TreeStructureForIterator*> it(tsfi->children());

    while(it.hasNext())
        recursiveDeleteIterator(it.next());
}

template<class IT>
bool CT_ResultItemIterator::hasNextT(IT *it) const
{
    bool again = false;

    do
    {
        again = false;

        CT_AbstractItemGroup *current = NULL;
        bool continueLoop = true;

        // search the next group not removed
        while(it->hasNext()
              && continueLoop)
        {
            current = (CT_AbstractItemGroup*)it->next();
            continueLoop = current->willBeRemovedLater();
        }

        // if we have found a group
        if(!continueLoop)
        {
            // if groups of this level must be returned, we backup the group to return it
            // when we go up in the structure
            m_currentInTree->setCurrentItem(current);

            // if the current node in tree has children
            if(m_currentInTree->nChildren() > 0)
            {
                CT_TreeStructureForIterator *backup = m_currentInTree;

                do
                {
                    if(hasNextWithCurrentChild())
                        return true;

                    m_currentInTree = backup;

                }while(m_currentInTree->incrementChildIndexAndSetCurrentChild());

                // else, go to next item in the iterator
                again = true;
            }
        }

    }while(again);

    // if the current node in tree don't have children
    // OR
    // if we don't found a group in the current iterator

    // go up in the structure
    if(m_currentInTree->parent() != NULL)
    {
        m_currentInTree = m_currentInTree->parent();

        if(m_currentInTree->isLastIncrementChildIndexSuccess())
        {
            CT_TreeStructureForIterator *backup = m_currentInTree;

            while(m_currentInTree->incrementChildIndexAndSetCurrentChild())
            {
                if(hasNextWithCurrentChild())
                    return true;

                m_currentInTree = backup;
            }
        }

        m_currentInTree->resetChildIndexAndSetCurrentChild();
        m_currentInTree->resetIncrementChildIndexSuccess();
    }

    return false;
}


bool CT_ResultItemIterator::hasNextWithCurrentChild() const
{
    CT_TreeStructureForIterator *child = m_currentInTree->currentChild();

    CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)m_currentInTree->currentItem();

    // current node in tree is the current child
    m_currentInTree = child;

    if(child->isAGroup())
    {
        delete (CT_GroupIterator*)child->iterator();

        // create a iterator for the child
        CT_GroupIterator *it = new CT_GroupIterator(group, (CT_OutAbstractGroupModel*)child->outModel());
        child->setIterator(it);

        // recursively go down in the tree structure

        // if in down of the structure something has been founded
        if(hasNextT<CT_GroupIterator>(it))
            return true;
    }
    else
    {
        if(child->isSearched())
        {
            CT_AbstractSingularItemDrawable *itemd = group->item((CT_OutAbstractSingularItemModel*)child->outModel());

            if(itemd != NULL)
            {
                child->setCurrentItem(itemd);
                return true;
            }
        }
    }

    return false;
}
