#ifndef CT_NORMALCLOUDSTDVECTOR_H
#define CT_NORMALCLOUDSTDVECTOR_H

#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 * A cloud of normals that use a std::vector
 */
class PLUGINSHAREDSHARED_EXPORT CT_NormalCloudStdVector : public CT_StandardCloudStdVectorT<CT_Normal>, public CT_AbstractNormalCloud
{
public:
    CT_NormalCloudStdVector(size_t size = 0);

    /**
     * @brief Returns the number of normals
     */
    size_t size() const;

    /**
     * @brief Add the normal passed in parameter to the cloud
     */
    void addNormal(const CT_Normal &normal);

    /**
     * @brief Add a default normal to the cloud and return a reference to it
     */
    CT_Normal& addNormal();

    /**
     * @brief Returns a reference to the normal at index 'index'
     */
    CT_Normal& normalAt(const size_t &index);

    /**
     * @brief Returns a const reference to the normal at index 'index'
     */
    const CT_Normal& constNormalAt(const size_t &index) const;

    /**
     * @brief Returns a reference to the normal at index 'index'
     */
    CT_Normal& operator[](const size_t &index);

    /**
     * @brief Returns a const reference to the normal at index 'index'
     */
    const CT_Normal& operator[](const size_t &index) const;

    /**
     * @brief Returns a copy of this cloud
     */
    CT_AbstractCloud* copy() const;

};

#endif // CT_NORMALCLOUDSTDVECTOR_H
