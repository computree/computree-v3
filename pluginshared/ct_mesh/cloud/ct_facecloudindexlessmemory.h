#ifndef CT_FACECLOUDINDEXLESSMEMORY_H
#define CT_FACECLOUDINDEXLESSMEMORY_H

#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"
#include "ct_cloudindex/ct_cloudindexlessmemoryt.h"

/**
 * @brief Un nuage d'index consommant très peu de mémoire mais ne peut pas être modifié (à part décaler d'un offset ou redimensionner ou couper)
 */
class PLUGINSHAREDSHARED_EXPORT CT_FaceCloudIndexLessMemory : public CT_AbstractFaceCloudIndex, public CT_CloudIndexLessMemoryT<CT_Face>
{
public:
    CT_FaceCloudIndexLessMemory(const size_t &begin, const size_t &size);

    CT_AbstractCloudIndex* copy() const;
};

#endif // CT_FACECLOUDINDEXLESSMEMORY_H
