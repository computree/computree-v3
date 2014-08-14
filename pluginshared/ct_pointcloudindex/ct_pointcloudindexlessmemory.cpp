#include "ct_pointcloudindexlessmemory.h"

CT_PointCloudIndexLessMemory::CT_PointCloudIndexLessMemory(const size_t &begin, const size_t &size) : CT_AbstractPointCloudIndex(), CT_CloudIndexLessMemoryT<CT_Point>(begin, size)
{
}

CT_AbstractCloudIndex* CT_PointCloudIndexLessMemory::copy() const
{
    return new CT_PointCloudIndexLessMemory(first(), size());
}
