#ifndef ITEMPLATEDDATA2DARRAY_H
#define ITEMPLATEDDATA2DARRAY_H

#include "pluginShared_global.h"

template< typename DataT>
class CT_ITemplatedData2DArray
{
public:
    virtual ~CT_ITemplatedData2DArray() {}

    virtual size_t xArraySize() const = 0;
    virtual size_t yArraySize() const = 0;
    virtual DataT dataFromArray(const size_t &x, const size_t &y) const = 0;
    virtual DataT dataFromArray(const size_t &index) const = 0;
};

#endif // ITEMPLATEDDATA2DARRAY_H
