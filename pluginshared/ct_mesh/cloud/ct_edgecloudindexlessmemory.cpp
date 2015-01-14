#include "ct_edgecloudindexlessmemory.h"

CT_EdgeCloudIndexLessMemory::CT_EdgeCloudIndexLessMemory(const size_t &begin, const size_t &size) : CT_AbstractEdgeCloudIndex(), CT_CloudIndexLessMemoryT<CT_Edge>(begin, size)
{
}

CT_AbstractCloud *CT_EdgeCloudIndexLessMemory::copy() const
{
    return new CT_EdgeCloudIndexLessMemory(first(), size());
}
