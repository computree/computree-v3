#ifndef CT_ITEMPLATEDDATA4DARRAY_H
#define CT_ITEMPLATEDDATA4DARRAY_H

#include "pluginShared_global.h"

template< typename DataT>
class CT_ITemplatedData4DArray
{
public:
    virtual ~CT_ITemplatedData4DArray() {}

    virtual size_t wArraySize() const = 0;
    virtual size_t xArraySize() const = 0;
    virtual size_t yArraySize() const = 0;
    virtual size_t zArraySize() const = 0;
    virtual DataT dataFromArray(const size_t &w,const  size_t &x,const  size_t &y,const  size_t &z) const = 0;
    virtual DataT dataFromArray(const size_t &index) const = 0;
};

#endif // CT_ITEMPLATEDDATA4DARRAY_H
