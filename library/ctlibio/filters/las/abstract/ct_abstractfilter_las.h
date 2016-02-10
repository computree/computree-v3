#ifndef CT_ABSTRACTFILTER_LAS_H
#define CT_ABSTRACTFILTER_LAS_H

#include "ct_filter/abstract/ct_abstractfilter_xyz.h"
#include "ctlibio/itemdrawable/las/ct_stdlaspointsattributescontainer.h"

#include "ctlibio/ctlibio_global.h"

class CTLIBIO_EXPORT CT_AbstractFilter_LAS : public CT_AbstractFilter_XYZ
{
    Q_OBJECT
public:
    CT_AbstractFilter_LAS();
    CT_AbstractFilter_LAS(const CT_AbstractFilter_LAS* other);
    ~CT_AbstractFilter_LAS();

    bool setLASAttributesContainer(const CT_StdLASPointsAttributesContainer *lasAttributes);

    virtual CT_PointCloudIndexVector *filterPointCloudIndex() const;
    virtual void validatePoint(CT_PointIterator& pointIt, CT_LASData &LADData) const = 0;
    void validatePoint(CT_PointIterator& pointIt) const; // neutralized: unused

protected:

    const CT_StdLASPointsAttributesContainer* _lasAttributes;
    const CT_AbstractPointCloudIndex* _LASPointCloudIndex;

};

#endif // CT_ABSTRACTFILTER_LAS_H
