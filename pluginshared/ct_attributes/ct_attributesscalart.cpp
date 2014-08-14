#include "ct_attributesscalart.h"

template<>
void CT_AttributesScalarT<bool>::initMinMax()
{
    m_min = true;
    m_max = false;
}
