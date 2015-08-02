#include "ct_abstractfilter_xyz.h"
#include "ct_iterator/ct_pointiterator.h"

CT_AbstractFilter_XYZ::CT_AbstractFilter_XYZ() : CT_AbstractFilter()
{
    _inCloud = NULL;
}

CT_AbstractFilter_XYZ::~CT_AbstractFilter_XYZ()
{
    _inCloud = NULL;
}

bool CT_AbstractFilter_XYZ::setPointCloudIndex(const CT_AbstractPointCloudIndex *inCloud)
{
    _inCloud = NULL;

    if (inCloud == NULL) {return false;}

    _inCloud = inCloud;
    return true;
}

CT_PointCloudIndexVector* CT_AbstractFilter_XYZ::filterPointCloudIndex() const
{
    CT_PointIterator itP(_inCloud);
    while(itP.hasNext())
    {
        validatePoint(itP.next());
    }
    return _outCloud;
}

void CT_AbstractFilter_XYZ::validatePoint(CT_PointIterator &pointIt) const
{
    _outCloud->addIndex(pointIt.currentGlobalIndex());
}

