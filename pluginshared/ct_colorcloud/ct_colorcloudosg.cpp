#include "ct_colorcloudosg.h"

CT_ColorCloudOsg::CT_ColorCloudOsg(const size_t &initialSize) : CT_StandardCloudOsgT<CT_Color, osg::Array::Vec4ubArrayType, 4, GL_UNSIGNED_BYTE>(initialSize), CT_AbstractColorCloud()
{
}

size_t CT_ColorCloudOsg::size() const
{
    return CT_StandardCloudOsgT<CT_Color, osg::Array::Vec4ubArrayType, 4, GL_UNSIGNED_BYTE>::size();
}

void CT_ColorCloudOsg::addColor(const CT_Color &color)
{
    addT(color);
}

CT_Color &CT_ColorCloudOsg::addColor()
{
    return addT();
}

CT_Color &CT_ColorCloudOsg::colorAt(const size_t &index)
{
    return pTAt(index);
}

const CT_Color &CT_ColorCloudOsg::constColorAt(const size_t &index) const
{
    return pTAt(index);
}

CT_Color &CT_ColorCloudOsg::operator[](const size_t &index)
{
    return pTAt(index);
}

const CT_Color &CT_ColorCloudOsg::operator[](const size_t &index) const
{
    return pTAt(index);
}

CT_AbstractCloud *CT_ColorCloudOsg::copy() const
{
    size_t s = size();

    CT_ColorCloudOsg *cloud = new CT_ColorCloudOsg(s);

    for(size_t i=0; i<s; ++i)
        (*cloud)[i] = (*this)[i];

    return cloud;
}
