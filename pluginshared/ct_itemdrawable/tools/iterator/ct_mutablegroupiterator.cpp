#include "ct_mutablegroupiterator.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractstandarditemgroup.h"
#include "ct_itemdrawable/ct_container.h"

#include "ct_itemdrawable/tools/iterator/ct_containeriterator.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

CT_MutableGroupIterator::CT_MutableGroupIterator(CT_AbstractItemGroup *parent)
{
    QHash<QString, CT_Container*> gs = parent->groups();
    QHashIterator<QString, CT_Container*> it(gs);

    while(it.hasNext())
        m_iterators.append(new CT_ContainerIterator(it.next().value()));

    m_hasNextCalled = false;
}

CT_MutableGroupIterator::CT_MutableGroupIterator(CT_AbstractItemGroup *parent, const CT_InAbstractGroupModel *inModel)
{
    QList<CT_InStdModelPossibility*> list = inModel->getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(list);

    QHash<QString, CT_Container*> gs = parent->groups();

    while(it.hasNext())
    {
        CT_Container *container = NULL;
        QString modelName = it.next()->outModel()->uniqueName();

        container = gs.value(modelName, NULL);

        if(container != NULL)
            m_iterators.append(new CT_ContainerIterator(container));
    }

    m_hasNextCalled = false;
}

CT_MutableGroupIterator::CT_MutableGroupIterator(CT_AbstractItemGroup *parent, const CT_OutAbstractGroupModel *outModel)
{
    QHash<QString, CT_Container*> gs = parent->groups();

    CT_Container *container = gs.value(outModel->uniqueName(), NULL);

    if(container != NULL)
        m_iterators.append(new CT_ContainerIterator(container));

    m_hasNextCalled = false;
}

CT_MutableGroupIterator::~CT_MutableGroupIterator()
{
    qDeleteAll(m_iterators.begin(), m_iterators.end());
}

bool CT_MutableGroupIterator::hasNext() const
{
    bool searchAgain = true;

    while(searchAgain)
    {
        searchAgain = false;

        if(m_iterators.isEmpty())
            return false;

        if(!m_hasNextCalled)
        {
            CT_ContainerIterator *it = m_iterators[0];

            if(!it->hasNext())
            {
                delete m_iterators.takeFirst();

                searchAgain = true;
            }
            else
            {
                bool continueLoop = true;

                // search the next group not removed
                while(it->hasNext()
                      && continueLoop)
                    continueLoop = ((CT_AbstractItemGroup*)it->next())->willBeRemovedLater();

                if(continueLoop)
                {
                    delete m_iterators.takeFirst();

                    searchAgain = true;
                }
            }
        }
    }

    m_hasNextCalled = true;
    return true;
}

CT_AbstractItemGroup* CT_MutableGroupIterator::next()
{
    if(!m_hasNextCalled)
    {
        if(hasNext())
        {
            m_hasNextCalled = false;
            return (CT_AbstractItemGroup*)m_iterators[0]->current();
        }

        return NULL;
    }

    m_hasNextCalled = false;
    return (CT_AbstractItemGroup*)m_iterators[0]->current();
}

CT_AbstractItemGroup* CT_MutableGroupIterator::current()
{
    return (CT_AbstractItemGroup*)m_iterators[0]->current();
}

void CT_MutableGroupIterator::remove()
{
    CT_AbstractItemGroup *group = current();

    ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
}
