#include "ct_facecloudindexlessmemory.h"

CT_FaceCloudIndexLessMemory::CT_FaceCloudIndexLessMemory(const size_t &begin, const size_t &size) : CT_AbstractFaceCloudIndex(), CT_CloudIndexLessMemoryT<CT_Face>(begin, size)
{
}

CT_AbstractCloud* CT_FaceCloudIndexLessMemory::copy() const
{
    return new CT_FaceCloudIndexLessMemory(first(), size());
}

