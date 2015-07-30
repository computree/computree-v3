#include "ct_normalcloudosg.h"

CT_NormalCloudOsg::CT_NormalCloudOsg(const size_t &initialSize) : CT_StandardCloudOsgT<CT_Normal, osg::Array::Vec4ArrayType, 4, GL_FLOAT>(initialSize), CT_AbstractNormalCloud()
{
}

size_t CT_NormalCloudOsg::size() const
{
    return CT_StandardCloudOsgT<CT_Normal, osg::Array::Vec4ArrayType, 4, GL_FLOAT>::size();
}

void CT_NormalCloudOsg::addNormal(const CT_Normal &normal)
{
    addT(normal);
}

CT_Normal &CT_NormalCloudOsg::addNormal()
{
    return addT();
}

CT_Normal &CT_NormalCloudOsg::normalAt(const size_t &index)
{
    return pTAt(index);
}

const CT_Normal &CT_NormalCloudOsg::constNormalAt(const size_t &index) const
{
    return pTAt(index);
}

CT_Normal &CT_NormalCloudOsg::operator[](const size_t &index)
{
    return pTAt(index);
}

const CT_Normal &CT_NormalCloudOsg::operator[](const size_t &index) const
{
    return pTAt(index);
}

CT_AbstractCloud *CT_NormalCloudOsg::copy() const
{
    size_t s = size();

    CT_NormalCloudOsg *cloud = new CT_NormalCloudOsg(s);

    for(size_t i=0; i<s; ++i)
        (*cloud)[i] = (*this)[i];

    return cloud;
}
