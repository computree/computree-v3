#include "ct_abstractfilter_las.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

CT_AbstractFilter_LAS::CT_AbstractFilter_LAS() : CT_AbstractFilter_XYZ()
{
    _lasAttributes = NULL;
    m_lasPointCloudIndex = NULL;
}

CT_AbstractFilter_LAS::CT_AbstractFilter_LAS(const CT_AbstractFilter_LAS &other) : CT_AbstractFilter_XYZ(other)
{
    _lasAttributes = other._lasAttributes;
    m_lasPointCloudIndex = other.m_lasPointCloudIndex;
}

CT_AbstractFilter_LAS::~CT_AbstractFilter_LAS()
{
}

bool CT_AbstractFilter_LAS::filterPointCloudIndex()
{
    if((_lasAttributes == NULL) || (inputPointCloudIndex() == NULL))
        return false;

    CT_LASData lasData;

    size_t size = inputPointCloudIndex()->size();

    size_t nPointsKeept = 0;

    CT_PointIterator itP(inputPointCloudIndex());

    size_t cptErrors = 0;
    size_t maxAttSize = _lasAttributes->pointsAttributesAt(CT_LasDefine::Return_Number)->getPointCloudIndex()->size();
    while(itP.hasNext())
    {
        size_t globalIndex = itP.next().currentGlobalIndex();
        size_t lasIndex = m_lasPointCloudIndex->indexOf(globalIndex);

        if (lasIndex < maxAttSize)
        {
            _lasAttributes->getLASDataAt(lasIndex, lasData);

            if(validatePoint(itP, lasData)) {
                outputPointCloudIndex()->addIndex(globalIndex);
                ++nPointsKeept;
            }
        } else {
            ++cptErrors;
        }
    }

    //PS_LOG->addInfoMessage(LogInterface::filter, tr("%1 points conservés sur %2 points").arg(nPointsKeept).arg(size));
    if (cptErrors > 0)
    {
        PS_LOG->addMessage(LogInterface::info, LogInterface::filter, tr("Pas d'informations LAS pour %1 points : points non conservés").arg(cptErrors));
    }

    return true;
}

bool CT_AbstractFilter_LAS::validatePoint(const CT_PointIterator &pointIt)
{
    if(_lasAttributes != NULL)
    {
        size_t lasIndex = m_lasPointCloudIndex->indexOf(pointIt.currentGlobalIndex());

        if (lasIndex < outputPointCloudIndex()->size())
        {
            CT_LASData lasData;
            _lasAttributes->getLASDataAt(lasIndex, lasData);

            return validatePoint(pointIt, lasData);
        }
    }

    return false;
}

CT_StdLASPointsAttributesContainer* CT_AbstractFilter_LAS::lasAttributes() const
{
    return _lasAttributes;
}

CT_AbstractPointCloudIndex *CT_AbstractFilter_LAS::lasPointCloudIndex() const
{
    return m_lasPointCloudIndex;
}

bool CT_AbstractFilter_LAS::setLASAttributesContainer(const CT_StdLASPointsAttributesContainer *lasAttributes)
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

    return (_lasAttributes != NULL);
}

