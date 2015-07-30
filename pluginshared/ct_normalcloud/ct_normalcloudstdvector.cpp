#include "ct_normalcloudstdvector.h"

CT_NormalCloudStdVector::CT_NormalCloudStdVector(size_t size) : CT_StandardCloudStdVectorT<CT_Normal>(size), CT_AbstractNormalCloud()
{
}

size_t CT_NormalCloudStdVector::size() const
{
    return CT_StandardCloudStdVectorT<CT_Normal>::size();
}

void CT_NormalCloudStdVector::addNormal(const CT_Normal &normal)
{
    addT(normal);
}

CT_Normal& CT_NormalCloudStdVector::addNormal()
{
    return addT();
}

CT_Normal& CT_NormalCloudStdVector::normalAt(const size_t &index)
{
    return pTAt(index);
}

const CT_Normal& CT_NormalCloudStdVector::constNormalAt(const size_t &index) const
{
    return pTAt(index);
}

CT_Normal& CT_NormalCloudStdVector::operator[](const size_t &index)
{
    return pTAt(index);
}

const CT_Normal& CT_NormalCloudStdVector::operator[](const size_t &index) const
{
    return pTAt(index);
}

CT_AbstractCloud* CT_NormalCloudStdVector::copy() const
{
    size_t s = size();

    CT_NormalCloudStdVector *cloud = new CT_NormalCloudStdVector(s);

    for(size_t i=0; i<s; ++i)
        (*cloud)[i] = pTAt(i);

    return cloud;
}
