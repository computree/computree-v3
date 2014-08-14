#include "ct_resultiteratorforgui.h"

#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"

CT_ResultIteratorForGui::CT_ResultIteratorForGui(const CT_ResultGroup *result)
{
    m_result = (CT_ResultGroup*)result;
}

void CT_ResultIteratorForGui::init(const CT_OutAbstractItemModel *model)
{
    const CT_OutAbstractSingularItemModel *itemModel = dynamic_cast<const CT_OutAbstractSingularItemModel*>(model);

    if(itemModel == NULL)
    {
        CT_ResultGroupIterator it(m_result, (CT_OutAbstractGroupModel*)model);

        while(it.hasNext())
            m_collection.append((CT_AbstractItemDrawable*)it.next());
    }
    else
    {
        CT_ResultItemIterator it(m_result, itemModel);

        while(it.hasNext())
            m_collection.append((CT_AbstractItemDrawable*)it.next());
    }
}

int CT_ResultIteratorForGui::size() const
{
    return m_collection.size();
}

bool CT_ResultIteratorForGui::hasNext() const
{
    return !m_collection.isEmpty();
}

CT_AbstractItemDrawable* CT_ResultIteratorForGui::next()
{
    return m_collection.takeFirst();
}
