#include "ct_edgecloudindexvector.h"

CT_EdgeCloudIndexVector::CT_EdgeCloudIndexVector(const size_t &size) : CT_AbstractEdgeCloudIndex(), CT_CloudIndexStdVectorT<CT_Edge>(size)
{
}

CT_AbstractCloudIndex* CT_EdgeCloudIndexVector::copy() const
{
    size_t s = size();

    CT_EdgeCloudIndexVector *index = new CT_EdgeCloudIndexVector(s);

    std::vector<size_type> *vector = internalData();
    std::vector<size_type> *cpyVector = index->internalData();

    for(size_t i=0; i<s; ++i)
        (*cpyVector)[i] = (*vector)[i];

    index->setSortType(sortType());

    return index;
}
