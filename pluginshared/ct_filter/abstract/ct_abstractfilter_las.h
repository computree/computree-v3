#ifndef CT_ABSTRACTFILTER_LAS_H
#define CT_ABSTRACTFILTER_LAS_H

#include "ct_filter/abstract/ct_abstractfilter_xyz.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractFilter_LAS : public CT_AbstractFilter_XYZ
{
    Q_OBJECT
public:
    CT_AbstractFilter_LAS();
    ~CT_AbstractFilter_LAS();
};

#endif // CT_ABSTRACTFILTER_LAS_H
