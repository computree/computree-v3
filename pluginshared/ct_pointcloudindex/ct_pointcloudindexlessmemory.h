#ifndef CT_POINTCLOUDINDEXLESSMEMORY_H
#define CT_POINTCLOUDINDEXLESSMEMORY_H

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

/**
 * @brief Un nuage d'index consommant très peu de mémoire mais ne peut pas être modifié (à part décalé d'un offset)
 */
class PLUGINSHAREDSHARED_EXPORT CT_PointCloudIndexLessMemory : public CT_AbstractPointCloudIndex, public CT_CloudIndexLessMemoryT<CT_Point>
{
public:
    CT_PointCloudIndexLessMemory(const size_t &begin, const size_t &size);

    CT_AbstractCloud* copy() const;
};

#endif // CT_POINTCLOUDINDEXLESSMEMORY_H
