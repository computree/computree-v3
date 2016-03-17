#include "ct_abstractfilter.h"

CT_AbstractFilter::CT_AbstractFilter() : CT_AbstractConfigurableElement()
{
}

CT_AbstractFilter::CT_AbstractFilter(const CT_AbstractFilter &other) : CT_AbstractConfigurableElement()
{
    Q_UNUSED(other);
}

CT_AbstractFilter::~CT_AbstractFilter()
{
}

