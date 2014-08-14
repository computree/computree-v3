#include "ct_attributescolor.h"

CT_AttributesColor::CT_AttributesColor(CT_AbstractColorCloud *cloud)
{
    m_colorCloud = cloud;
}

CT_AttributesColor::~CT_AttributesColor()
{
    delete m_colorCloud;
}

const CT_Color& CT_AttributesColor::constColorAt(const size_t &index) const
{
    return m_colorCloud->colorAt(index);
}

CT_AbstractColorCloud* CT_AttributesColor::getColorCloud() const
{
    return m_colorCloud;
}

size_t CT_AttributesColor::attributesSize() const
{
    return m_colorCloud->size();
}

void CT_AttributesColor::setColorCloud(CT_AbstractColorCloud *cloud)
{
    delete m_colorCloud;
    m_colorCloud = cloud;
}
