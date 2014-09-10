#include "ct_resultiterator.h"

#include "ct_item/abstract/ct_abstractitem.h"
#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"
#include "ct_result/ct_resultgroup.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

CT_ResultIterator::CT_ResultIterator(const CT_ResultGroup *result,
                                     bool recursiveIteration)
{
    m_size = -1;
    m_result = (CT_ResultGroup*)result;
    m_itemIT = NULL;
    m_groupIT = NULL;

    if(recursiveIteration)
        m_itemIT = new CT_ResultItemIterator(m_result);
    else
        m_groupIT = new CT_ResultGroupIterator(m_result);

    m_model = NULL;
}

CT_ResultIterator::CT_ResultIterator(const CT_ResultGroup *result,
                                     const CT_AbstractModel *model)
{
    m_size = -1;
    m_result = (CT_ResultGroup*)result;
    m_itemIT = NULL;
    m_groupIT = NULL;
    m_model = (CT_AbstractModel*)model;

    initModel(m_model);
}

CT_ResultIterator::CT_ResultIterator(const CT_ResultGroup *result,
                                     const CT_VirtualAbstractStep *step,
                                     const QString &modelUniqueName)
{
    m_size = -1;
    m_result = (CT_ResultGroup*)result;
    m_itemIT = NULL;
    m_groupIT = NULL;
    m_model = PS_MODELS->searchModel(modelUniqueName, result, step);

    Q_ASSERT_X(m_model != NULL, "CT_ResultIterator constructor", "You created a CT_ResultIterator with a modelName but the model was not found");

    initModel(m_model);
}

CT_ResultIterator::~CT_ResultIterator()
{
    delete m_itemIT;
    delete m_groupIT;
}

int CT_ResultIterator::size() const
{
    if(m_size == -1)
    {
        if(m_itemIT != NULL)
        {
            while(m_itemIT->hasNext())
                m_collection.append((CT_AbstractSingularItemDrawable*)m_itemIT->next());

            delete m_itemIT;
            m_itemIT = NULL;
        }
        else if(m_groupIT != NULL)
        {
            while(m_groupIT->hasNext())
                m_collection.append((CT_AbstractItemGroup*)m_groupIT->next());

            delete m_groupIT;
            m_groupIT = NULL;
        }

        m_size = m_collection.size();
    }

    return m_size;
}

bool CT_ResultIterator::hasNext() const
{
    if(m_itemIT != NULL)
        return m_itemIT->hasNext();
    else if(m_groupIT != NULL)
        return m_groupIT->hasNext();

    return !m_collection.isEmpty();
}

const CT_AbstractItem* CT_ResultIterator::next()
{
    if(m_itemIT != NULL)
        return m_itemIT->next();
    else if(m_groupIT != NULL)
        return m_groupIT->next();

    return m_collection.takeFirst();
}

void CT_ResultIterator::initModel(const CT_AbstractModel *model)
{
    const CT_OutAbstractSingularItemModel *outItemModel = dynamic_cast<const CT_OutAbstractSingularItemModel*>(model);

    if(outItemModel != NULL)
    {
        this->initItemT<CT_OutAbstractSingularItemModel>(model);
    }
    else
    {
        const CT_InAbstractSingularItemModel *inItemModel = dynamic_cast<const CT_InAbstractSingularItemModel*>(model);

        if(inItemModel != NULL)
        {
            this->initItemT<CT_InAbstractSingularItemModel>(model);
        }
        else
        {
            const CT_OutAbstractGroupModel *outGModel = dynamic_cast<const CT_OutAbstractGroupModel*>(model);

            if(outGModel != NULL)
            {
                this->initGroupT<CT_OutAbstractGroupModel>(model);
            }
            else
            {
                const CT_InAbstractGroupModel *inGModel = dynamic_cast<const CT_InAbstractGroupModel*>(model);

                if(inGModel != NULL)
                    this->initGroupT<CT_InAbstractGroupModel>(model);
            }
        }
    }
}

template<typename T>
void CT_ResultIterator::initGroupT(const CT_AbstractModel *model)
{
    m_groupIT = new CT_ResultGroupIterator(m_result, (T*)model);
}

template<typename T>
void CT_ResultIterator::initItemT(const CT_AbstractModel *model)
{
    m_itemIT = new CT_ResultItemIterator(m_result, (T*)model);
}
