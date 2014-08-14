#ifndef CT_ITEMPLATEDDATA1DARRAY_H
#define CT_ITEMPLATEDDATA1DARRAY_H

#include "pluginShared_global.h"

template< typename DataT>
class CT_ITemplatedData1DArray
{
public:
    virtual ~CT_ITemplatedData1DArray() {}

    virtual size_t arraySize() const = 0;
    virtual DataT dataFromArray(const size_t &i) const = 0;
};

#endif // CT_ITEMPLATEDDATA1DARRAY_H
