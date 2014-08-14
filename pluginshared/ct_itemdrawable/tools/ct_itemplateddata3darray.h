#ifndef CT_ITEMPLATEDDATA3DARRAY_H
#define CT_ITEMPLATEDDATA3DARRAY_H

#include "pluginShared_global.h"

template< typename DataT>
class CT_ITemplatedData3DArray
{
public:
    virtual ~CT_ITemplatedData3DArray() {}

    virtual size_t xArraySize() const = 0;
    virtual size_t yArraySize() const = 0;
    virtual size_t zArraySize() const = 0;
    virtual DataT dataFromArray(const size_t &x, const size_t &y, const size_t &z) const = 0;
    virtual DataT dataFromArray(const size_t &index) const = 0;
};


#endif // CT_ITEMPLATEDDATA3DARRAY_H
