#include "ct_resultiterator.h"

#include "ct_item/abstract/ct_abstractitem.h"
#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"

#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h"
#include "ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

CT_ResultIterator::CT_ResultIterator(const CT_ResultGroup *result,
                                     const CT_AbstractModel *model,
                                     bool mustGetSize)
{
    m_result = (CT_ResultGroup*)result;
    m_itemIT = NULL;
    m_groupIT = NULL;

    const CT_OutAbstractSingularItemModel *outItemModel = dynamic_cast<const CT_OutAbstractSingularItemModel*>(model);

    if(outItemModel != NULL)
    {
        this->initItemT<CT_OutAbstractSingularItemModel>(model, mustGetSize);
    }
    else
    {
        const CT_InAbstractSingularItemModel *inItemModel = dynamic_cast<const CT_InAbstractSingularItemModel*>(model);

        if(inItemModel != NULL)
        {
            this->initItemT<CT_InAbstractSingularItemModel>(model, mustGetSize);
        }
        else
        {
            const CT_OutAbstractGroupModel *outGModel = dynamic_cast<const CT_OutAbstractGroupModel*>(model);

            if(outGModel != NULL)
            {
                this->initGroupT<CT_OutAbstractGroupModel>(model, mustGetSize);
            }
            else
            {
                const CT_InAbstractGroupModel *inGModel = dynamic_cast<const CT_InAbstractGroupModel*>(model);

                if(inGModel != NULL)
                {
                    this->initGroupT<CT_InAbstractGroupModel>(model, mustGetSize);
                }
            }
        }
    }
}

CT_ResultIterator::~CT_ResultIterator()
{
    delete m_itemIT;
    delete m_groupIT;
}

int CT_ResultIterator::size() const
{
    return m_collection.size();
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

template<typename T>
void CT_ResultIterator::initGroupT(const CT_AbstractModel *model, bool mustGetSize)
{
    if(mustGetSize)
    {
        CT_ResultGroupIterator it(m_result, (T*)model);

        while(it.hasNext())
            m_collection.append((CT_AbstractItemGroup*)it.next());
    }
    else
    {
        m_groupIT = new CT_ResultGroupIterator(m_result, (T*)model);
    }
}

template<typename T>
void CT_ResultIterator::initItemT(const CT_AbstractModel *model, bool mustGetSize)
{
    if(mustGetSize)
    {
        CT_ResultItemIterator it(m_result, (T*)model);

        while(it.hasNext())
            m_collection.append((CT_AbstractSingularItemDrawable*)it.next());
    }
    else
    {
        m_itemIT = new CT_ResultItemIterator(m_result, (T*)model);
    }
}
