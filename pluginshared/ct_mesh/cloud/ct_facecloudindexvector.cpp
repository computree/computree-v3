#include "ct_facecloudindexvector.h"

CT_FaceCloudIndexVector::CT_FaceCloudIndexVector(const size_t &size) : CT_AbstractFaceCloudIndex(), CT_CloudIndexStdVectorT<CT_Face>(size)
{
}

CT_AbstractCloudIndex* CT_FaceCloudIndexVector::copy() const
{
    size_t s = size();

    CT_FaceCloudIndexVector *index = new CT_FaceCloudIndexVector(s);

    std::vector<size_type> *vector = internalData();
    std::vector<size_type> *cpyVector = index->internalData();

    for(size_t i=0; i<s; ++i)
        (*cpyVector)[i] = (*vector)[i];

    index->setSortType(sortType());

    return index;
}
