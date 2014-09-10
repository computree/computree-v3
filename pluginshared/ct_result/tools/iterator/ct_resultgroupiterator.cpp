#include "ct_resultgroupiterator.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/tools/iterator/private/ct_treestructureforiterator.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_ResultGroupIterator::CT_ResultGroupIterator(const CT_ResultGroup *result)
{
    m_result = (CT_ResultGroup*)result;
    m_rootTree = NULL;

    QListIterator<CT_AbstractItemGroup*> *it = NULL;

    if(!m_result->groups().isEmpty())
        it = new QListIterator<CT_AbstractItemGroup*>(m_result->groups());

    if(it != NULL)
    {
        m_rootTree = new CT_TreeStructureForIterator();
        m_rootTree->setIterator(it);
    }

    m_hasNextCalled = false;
    m_current = NULL;

    m_currentInTree = m_rootTree;
}

CT_ResultGroupIterator::CT_ResultGroupIterator(const CT_ResultGroup *result,
                                               const CT_InAbstractGroupModel *inModel)
{
    initIterator(result, inModel);
}

CT_ResultGroupIterator::CT_ResultGroupIterator(const CT_ResultGroup *result, const CT_OutAbstractGroupModel *outModel)
{
    initIterator(result, outModel);
}

CT_ResultGroupIterator::CT_ResultGroupIterator(const CT_ResultGroup *result, const CT_VirtualAbstractStep *step, const QString &modelName)
{
    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, result, step);

    Q_ASSERT_X(model != NULL, "CT_ResultGroupIterator constructor", "You created a CT_ResultGroupIterator with a modelName but the model was not found");

    CT_InAbstractGroupModel *inModel = dynamic_cast<CT_InAbstractGroupModel*>(model);
    CT_OutAbstractGroupModel *outModel = NULL;

    if(inModel != NULL)
        initIterator(result, inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractGroupModel*>(model)) != NULL)
        initIterator(result, outModel);
    else
        qFatal("You created a CT_ResultGroupIterator with a modelName but the model was not a group model");
}

CT_ResultGroupIterator::~CT_ResultGroupIterator()
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

bool CT_ResultGroupIterator::hasNext() const
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
                if(hasNextT< CT_GroupIterator >((CT_GroupIterator*)m_currentInTree->iterator()))
                    again = false;
            }

        }while(again);
    }

    m_hasNextCalled = true;
    return true;
}

const CT_AbstractItemGroup* CT_ResultGroupIterator::next()
{
    if(!m_hasNextCalled)
    {
        if(hasNext())
        {
            const CT_AbstractItemGroup *group = (const CT_AbstractItemGroup*)m_currentInTree->currentItem();
            m_currentInTree->setCurrentItem(NULL);

            m_hasNextCalled = false;

            return group;
        }

        return NULL;
    }

    m_hasNextCalled = false;
    return (const CT_AbstractItemGroup*)m_currentInTree->currentItem();
}

void CT_ResultGroupIterator::initIterator(const CT_ResultGroup *result, const CT_InAbstractGroupModel *inModel)
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
            constructor.addModelsForGroupsToStructure(itChecked.next());

        CT_TreeStructureForIterator *rootTree = constructor.takeStructure();

        // the structure must contains at least one group and this group
        // must have the same model that root groups of the result
        if((rootTree != NULL)
                && (groups.first()->model()->uniqueName() == rootTree->outModel()->uniqueName()))
        {
            // we must iterate over root groups whatever happens
            QListIterator<CT_AbstractItemGroup*> *it = new QListIterator<CT_AbstractItemGroup*>(m_result->groups());

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

void CT_ResultGroupIterator::initIterator(const CT_ResultGroup *result, const CT_OutAbstractGroupModel *outModel)
{
    m_result = (CT_ResultGroup*)result;
    m_rootTree = NULL;

    m_hasNextCalled = false;
    m_current = NULL;

    const QList<CT_AbstractItemGroup*> &groups = m_result->groups();

    if(!groups.isEmpty())
    {
        CT_TreeStructureConstructor constructor;

        constructor.addModelsForGroupsToStructure((DEF_CT_AbstractGroupModelOut*)dynamic_cast<const DEF_CT_AbstractGroupModelOut*>(outModel));

        CT_TreeStructureForIterator *rootTree = constructor.takeStructure();

        // the structure must contains at least one group and this group
        // must have the same model that root groups of the result
        if((rootTree != NULL)
                && (groups.first()->model()->uniqueName() == rootTree->outModel()->uniqueName()))
        {
            // we must iterate over root groups whatever happens
            QListIterator<CT_AbstractItemGroup*> *it = new QListIterator<CT_AbstractItemGroup*>(m_result->groups());

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

void CT_ResultGroupIterator::recursiveDeleteIterator(CT_TreeStructureForIterator *tsfi)
{
    delete ((CT_GroupIterator*)tsfi->iterator());
    tsfi->setIterator(NULL);

    QListIterator<CT_TreeStructureForIterator*> it(tsfi->children());

    while(it.hasNext())
        recursiveDeleteIterator(it.next());
}

template<class IT>
bool CT_ResultGroupIterator::hasNextT(IT *it) const
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

                // if the current node in tree must be returned
                if(m_currentInTree->isSearched())
                    return true; // we have a next item (it's the currentItem() of m_currentInTree)

                // else, go to next item in the iterator
                again = true;
            }
            // if the current node in tree must be returned
            else if(m_currentInTree->isSearched())
            {
                return true; // we have a next item (it's the currentItem() of m_currentInTree)
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

        // if the current node in tree must be returned
        if(m_currentInTree->isSearched())
            return true; // we have a next item (it's the currentItem() of m_currentInTree)
    }

    return false;
}

bool CT_ResultGroupIterator::hasNextWithCurrentChild() const
{
    CT_TreeStructureForIterator *child = m_currentInTree->currentChild();

    CT_AbstractItemGroup *group = (CT_AbstractItemGroup*)m_currentInTree->currentItem();

    // current node in tree is the current child
    m_currentInTree = child;

    delete (CT_GroupIterator*)child->iterator();

    // create a iterator for the child
    CT_GroupIterator *it = new CT_GroupIterator(group, (CT_OutAbstractGroupModel*)child->outModel());
    child->setIterator(it);

    // recursively go down in the tree structure

    // if in down of the structure something has been founded
    if(hasNextT<CT_GroupIterator>(it))
        return true;

    return false;
}
