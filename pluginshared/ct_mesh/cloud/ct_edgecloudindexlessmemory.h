#ifndef CT_EDGECLOUDINDEXLESSMEMORY_H
#define CT_EDGECLOUDINDEXLESSMEMORY_H

#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

/**
 * @brief Un nuage d'index consommant très peu de mémoire mais ne peut pas être modifié (à part décaler d'un offset ou redimensionner ou couper)
 */
class PLUGINSHAREDSHARED_EXPORT CT_EdgeCloudIndexLessMemory : public CT_AbstractEdgeCloudIndex, public CT_CloudIndexLessMemoryT<CT_Edge>
{
public:
    CT_EdgeCloudIndexLessMemory(const size_t &begin, const size_t &size);

    CT_AbstractCloudIndex* copy() const;
};

#endif // CT_EDGECLOUDINDEXLESSMEMORY_H
