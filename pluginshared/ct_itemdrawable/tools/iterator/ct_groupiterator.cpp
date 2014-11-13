#include "ct_groupiterator.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
#include "ct_itemdrawable/ct_container.h"

#include "ct_itemdrawable/tools/iterator/ct_containeriterator.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_GroupIterator::CT_GroupIterator(const CT_AbstractItemGroup *parent)
{
    QHash<QString, CT_Container*> gs = parent->groups();
    QHashIterator<QString, CT_Container*> it(gs);

    while(it.hasNext())
        m_iterators.append(new CT_ContainerIterator(it.next().value()));

    m_hasNextCalled = false;
}

CT_GroupIterator::CT_GroupIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractGroupModel *inModel)
{
    initIterator(parent, inModel);
}

CT_GroupIterator::CT_GroupIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractGroupModel *outModel)
{
    initIterator(parent, outModel);
}

CT_GroupIterator::CT_GroupIterator(const CT_AbstractItemGroup *parent, const CT_VirtualAbstractStep *step, const QString &modelName)
{
    CT_ResultGroup *result = (CT_ResultGroup*)parent->result();

    Q_ASSERT_X(result != NULL, "CT_GroupIterator constructor", "You created a CT_GroupIterator with a modelName but the result of the group is null");

    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, result, step);

    Q_ASSERT_X(model != NULL, "CT_GroupIterator constructor", "You created a CT_GroupIterator with a modelName but the model was not found");

    CT_InAbstractGroupModel *inModel = dynamic_cast<CT_InAbstractGroupModel*>(model);
    CT_OutAbstractGroupModel *outModel = NULL;

    if(inModel != NULL)
        initIterator(parent, inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractGroupModel*>(model)) != NULL)
        initIterator(parent, outModel);
    else
        qFatal("You created a CT_GroupIterator with a modelName but the model was not a group model");
}


CT_GroupIterator::~CT_GroupIterator()
{
    qDeleteAll(m_iterators.begin(), m_iterators.end());
}

bool CT_GroupIterator::hasNext() const
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

const CT_AbstractItemGroup* CT_GroupIterator::next()
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

const CT_AbstractItemGroup* CT_GroupIterator::current() const
{
    return (const CT_AbstractItemGroup*)m_iterators[0]->current();
}

void CT_GroupIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractGroupModel *inModel)
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

void CT_GroupIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractGroupModel *outModel)
{
    CT_Container *container = parent->groups().value(outModel->uniqueName(), NULL);

    if(container != NULL)
        m_iterators.append(new CT_ContainerIterator(container));

    m_hasNextCalled = false;
}
