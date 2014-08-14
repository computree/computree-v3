#ifndef CT_EDGECLOUDINDEXVECTOR_H
#define CT_EDGECLOUDINDEXVECTOR_H

#include "ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h"
#include "ct_cloudindex/ct_cloudindexstdvectort.h"

/**
 * @brief Nuage d'index consommant plus de mémoire que le CT_CloudIndexLessMemoryT mais modifiable. Tri par ordre croissant
 *        des index par défaut.
 */
class PLUGINSHAREDSHARED_EXPORT CT_EdgeCloudIndexVector : public CT_AbstractEdgeCloudIndex, public CT_CloudIndexStdVectorT<CT_Edge>
{
public:
    CT_EdgeCloudIndexVector(const size_t &size = 0);

    CT_AbstractCloudIndex* copy() const;
};

#endif // CT_EDGECLOUDINDEXVECTOR_H
