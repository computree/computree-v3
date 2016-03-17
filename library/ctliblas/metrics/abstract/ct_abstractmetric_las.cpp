#include "ct_abstractmetric_las.h"

CT_AbstractMetric_LAS::CT_AbstractMetric_LAS() : CT_AbstractMetric_XYZ()
{
    _lasAttributes = NULL;
    m_lasPointCloudIndex = NULL;
}

CT_AbstractMetric_LAS::CT_AbstractMetric_LAS(const CT_AbstractMetric_LAS &other) : CT_AbstractMetric_XYZ(other)
{
    _lasAttributes = other._lasAttributes;
    m_lasPointCloudIndex = other.m_lasPointCloudIndex;
}

CT_AbstractMetric_LAS::~CT_AbstractMetric_LAS()
{
}

bool CT_AbstractMetric_LAS::initLasDatas(const CT_AbstractPointCloudIndex *inCloud, const CT_AreaShape2DData *plotArea, const CT_StdLASPointsAttributesContainer *lasAttributes)
{
    _lasAttributes = (CT_StdLASPointsAttributesContainer*)lasAttributes;
    m_lasPointCloudIndex = NULL;

    if (_lasAttributes == NULL)
        return false;

    QHashIterator<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> it(_lasAttributes->lasPointsAttributes());

    if(!it.hasNext())
        return false;

    CT_AbstractPointAttributesScalar *firstAttribute = it.next().value();

    if((m_lasPointCloudIndex = (CT_AbstractPointCloudIndex*)firstAttribute->getPointCloudIndex()) == NULL)
        _lasAttributes = NULL;

    if((_lasAttributes != NULL) && !initDatas(inCloud, plotArea)) {
        _lasAttributes = NULL;
        m_lasPointCloudIndex = NULL;
    }

    return (_lasAttributes != NULL);
}

CT_StdLASPointsAttributesContainer *CT_AbstractMetric_LAS::lasAttributes() const
{
    return _lasAttributes;
}

CT_AbstractPointCloudIndex *CT_AbstractMetric_LAS::lasPointCloudIndex() const
{
    return m_lasPointCloudIndex;
}
