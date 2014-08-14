#ifndef CT_ABSTRACTATTRIBUTESSCALAR_H
#define CT_ABSTRACTATTRIBUTESSCALAR_H

#include "pluginShared_global.h"
#include "interfaces.h"

/**
 * @brief A cloud attributes of type Scalar (float, int, double, etc..). The min() and max() values are used to define the upper
 *        and lower gradient color to apply to the object to colorize per example.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractAttributesScalar
{
public:
    CT_AbstractAttributesScalar();

    virtual double dMin() const = 0;
    virtual double dMax() const = 0;

    virtual double dValueAt(const size_t &index) const = 0;

    virtual size_t attributesSize() const = 0;
};

#endif // CT_ABSTRACTATTRIBUTESSCALAR_H
