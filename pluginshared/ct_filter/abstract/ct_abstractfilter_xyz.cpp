#include "ct_abstractfilter_xyz.h"
#include "ct_iterator/ct_pointiterator.h"

CT_AbstractFilter_XYZ::CT_AbstractFilter_XYZ() : CT_AbstractFilter()
{
    _inCloud = NULL;
    _outCloud = new CT_PointCloudIndexVector();
}

CT_AbstractFilter_XYZ::CT_AbstractFilter_XYZ(const CT_AbstractFilter_XYZ *other) : CT_AbstractFilter(other)
{
    _inCloud = other->_inCloud;
    _outCloud = new CT_PointCloudIndexVector();
}

CT_AbstractFilter_XYZ::~CT_AbstractFilter_XYZ()
{
}

bool CT_AbstractFilter_XYZ::setPointCloudIndex(const CT_AbstractItemDrawableWithPointCloud *inItem)
{
    _inCloud = NULL;
    _inItem = NULL;

    if (inItem == NULL || inItem->getPointCloudIndex() == NULL) {return false;}

    _inCloud = inItem->getPointCloudIndex();
    _inItem = inItem;
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

