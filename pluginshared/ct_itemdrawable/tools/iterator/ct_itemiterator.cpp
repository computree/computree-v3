#include "ct_itemiterator.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_container.h"

#include "ct_itemdrawable/tools/iterator/ct_containeriterator.h"

#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

CT_ItemIterator::CT_ItemIterator(const CT_AbstractItemGroup *parent)
{
    m_items = parent->items();
    m_iterators.append(m_items.uniqueKeys());

    m_parent = (CT_AbstractItemGroup*)parent;
    m_current = NULL;
}

CT_ItemIterator::CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel)
{
    initIterator(parent, inModel);
}

CT_ItemIterator::CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel)
{
    initIterator(parent , outModel);
}

CT_ItemIterator::CT_ItemIterator(const CT_AbstractItemGroup *parent, const CT_VirtualAbstractStep *step, const QString &modelName)
{
    CT_ResultGroup *result = (CT_ResultGroup*)parent->result();

    Q_ASSERT_X(result != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the result of the group is null");

    // if the group passed in parameter is a group of a OUT result of the step
    if(result->parentStep() == step)
    {
        // check if the result is a copy
        CT_OutResultModelGroupCopy *outCopyModel = dynamic_cast<CT_OutResultModelGroupCopy*>(result->model());

        if(outCopyModel != NULL)
        {
            // search the IN result model that correspond to this out model copy
            const CT_InResultModelGroupToCopy *inResultCopyModel = outCopyModel->inResultModelCopy();

            // search a IN model
            CT_InAbstractModel *model = step->getInModelForResearchIfUseCopy(inResultCopyModel->uniqueName(), modelName);

            Q_ASSERT_X(model != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not found");

            CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

            Q_ASSERT_X(inModel != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not a item model");

            initIterator(parent, inModel);
        }
        else
        {
            // search a out model
            CT_OutAbstractModel *model = step->getOutModelForCreation(result, modelName);

            Q_ASSERT_X(model != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not found");

            CT_OutAbstractSingularItemModel *outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model);

            Q_ASSERT_X(outModel != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not a item model");

            initIterator(parent, outModel);
        }
    }
    else
    {
        // search a in model
        CT_InAbstractModel *model = step->getInModelForResearch(result, modelName);

        Q_ASSERT_X(model != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not found");

        CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);

        Q_ASSERT_X(inModel != NULL, "CT_ItemIterator constructor", "You created a CT_ItemIterator with a modelName but the model was not a item model");

        initIterator(parent, inModel);
    }
}

CT_ItemIterator::~CT_ItemIterator()
{
}

bool CT_ItemIterator::hasNext() const
{
    bool searchAgain = true;

    while(searchAgain)
    {
        searchAgain = false;

        if(m_iterators.isEmpty())
            return false;

        if(!m_hasNextCalled)
        {
            m_current = NULL;

            QString modelName = m_iterators.takeFirst();

            CT_GroupItemDrawableContainer *ct = m_items.value(modelName, NULL);

            if((ct == NULL) || ct->willBeRemovedLater())
                searchAgain = true;
            else
                m_current = ct->item();
        }
    }

    m_hasNextCalled = true;
    return true;
}

const CT_AbstractSingularItemDrawable* CT_ItemIterator::next()
{
    if(!m_hasNextCalled)
    {
        if(hasNext())
        {
            m_hasNextCalled = false;
            return m_current;
        }

        return NULL;
    }

    m_hasNextCalled = false;
    return m_current;
}

const CT_AbstractSingularItemDrawable* CT_ItemIterator::current() const
{
    return m_current;
}

void CT_ItemIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel)
{
    m_items = parent->items();
    QList<CT_InStdModelPossibility*> list = inModel->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
        m_iterators.append(it.next()->outModel()->uniqueName());

    m_parent = (CT_AbstractItemGroup*)parent;
    m_current = NULL;
}

void CT_ItemIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel)
{
    m_items = parent->items();
    m_iterators.append(outModel->uniqueName());

    m_parent = (CT_AbstractItemGroup*)parent;
    m_current = NULL;
}
