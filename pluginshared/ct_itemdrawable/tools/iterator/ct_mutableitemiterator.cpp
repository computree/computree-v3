#include "ct_mutableitemiterator.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractstandarditemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_container.h"

#include "ct_itemdrawable/tools/iterator/ct_containeriterator.h"

#include "ct_result/ct_resultgroup.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_MutableItemIterator::CT_MutableItemIterator(CT_AbstractItemGroup *parent)
{
    m_items = m_parent->items();
    m_iterators.append(m_items.uniqueKeys());

    m_parent = parent;
    m_current = NULL;
    m_hasNextCalled = false;
}

CT_MutableItemIterator::CT_MutableItemIterator(CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel)
{
    initIterator(parent, inModel);
}

CT_MutableItemIterator::CT_MutableItemIterator(CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel)
{
    initIterator(parent, outModel);
}

CT_MutableItemIterator::CT_MutableItemIterator(const CT_AbstractItemGroup *parent, const CT_VirtualAbstractStep *step, const QString &modelName)
{
    CT_ResultGroup *result = (CT_ResultGroup*)parent->result();

    Q_ASSERT_X(result != NULL, "CT_MutableItemIterator constructor", "You created a CT_MutableItemIterator with a modelName but the result of the group is null");

    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, result, step);

    Q_ASSERT_X(model != NULL, "CT_MutableItemIterator constructor", "You created a CT_MutableItemIterator with a modelName but the model was not found");

    CT_InAbstractSingularItemModel *inModel = dynamic_cast<CT_InAbstractSingularItemModel*>(model);
    CT_OutAbstractSingularItemModel *outModel = NULL;

    if(inModel != NULL)
        initIterator(parent, inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractSingularItemModel*>(model)) != NULL)
        initIterator(parent, outModel);
    else
        qFatal("You created a CT_MutableItemIterator with a modelName but the model was not a singular item model");
}

CT_MutableItemIterator::~CT_MutableItemIterator()
{
}

bool CT_MutableItemIterator::hasNext() const
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

CT_AbstractSingularItemDrawable* CT_MutableItemIterator::next()
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

CT_AbstractSingularItemDrawable* CT_MutableItemIterator::current()
{
    return m_current;
}

void CT_MutableItemIterator::remove()
{
    CT_AbstractSingularItemDrawable *item = current();

    ((CT_ResultGroup*)m_parent->result())->removeSingularItemDrawableSomethingInStructure(m_parent, item);
}

void CT_MutableItemIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_InAbstractSingularItemModel *inModel)
{
    m_items = parent->items();
    QList<CT_InStdModelPossibility*> list = inModel->getPossibilitiesSavedSelected();
    QListIterator<CT_InStdModelPossibility*> it(list);

    while(it.hasNext())
        m_iterators.append(it.next()->outModel()->uniqueName());

    m_parent = (CT_AbstractItemGroup*)parent;
    m_current = NULL;
    m_hasNextCalled = false;
}

void CT_MutableItemIterator::initIterator(const CT_AbstractItemGroup *parent, const CT_OutAbstractSingularItemModel *outModel)
{
    m_items = parent->items();
    m_iterators.append(outModel->uniqueName());

    m_parent =(CT_AbstractItemGroup*) parent;
    m_current = NULL;
    m_hasNextCalled = false;
}
