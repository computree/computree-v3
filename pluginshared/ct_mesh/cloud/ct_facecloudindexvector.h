#ifndef CT_FACECLOUDINDEXVECTOR_H
#define CT_FACECLOUDINDEXVECTOR_H

#include "ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h"
#include "ct_cloudindex/ct_cloudindexstdvectort.h"

/**
 * @brief Nuage d'index consommant plus de mémoire que le CT_CloudIndexLessMemoryT mais modifiable. Tri par ordre croissant
 *        des index par défaut.
 */
class PLUGINSHAREDSHARED_EXPORT CT_FaceCloudIndexVector : public CT_AbstractFaceCloudIndex, public CT_CloudIndexStdVectorT<CT_Face>
{
public:
    CT_FaceCloudIndexVector(const size_t &size = 0);

    CT_AbstractCloud* copy() const;
};

#endif // CT_FACECLOUDINDEXVECTOR_H
