#ifndef CT_NORMALCLOUDOSG_H
#define CT_NORMALCLOUDOSG_H

#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"
#include "ct_cloud/ct_standardcloudosgt.h"
#include "ct_normal.h"

class PLUGINSHAREDSHARED_EXPORT CT_NormalCloudOsg : public CT_StandardCloudOsgT<CT_Normal, osg::Array::Vec4ArrayType, 4, GL_FLOAT>, public CT_AbstractNormalCloud
{
public:
    CT_NormalCloudOsg(const size_t &initialSize);

    /**
     * @brief Returns the number of colors
     */
    size_t size() const;

    /**
     * @brief Add the normal color in parameter to the cloud
     */
    void addNormal(const CT_Normal &color);

    /**
     * @brief Add a default color to the cloud and return a reference to it
     */
    CT_Normal& addNormal();

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Normal& normalAt(const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Normal& constNormalAt(const size_t &index) const;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Normal& operator[](const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Normal& operator[](const size_t &index) const;

    /**
     * @brief Replace a normal at index specified
     */
    void replaceNormal(const size_t &index, CT_Normal &normal);

    virtual CT_AbstractCloud* copy() const;

protected:
    /**
     * @brief delete all elements of the cloud between beginIndex and (beginIndex+size-1) included.
     */
    inline void erase(const size_t &beginIndex, const size_t &sizes) { CT_StandardCloudOsgT<CT_Normal, osg::Array::Vec4ArrayType, 4, GL_FLOAT>::erase(beginIndex, sizes); }

    /**
     * @brief resize the collection (Called by CT_GlobalCloudManagerT and CT_AbstractCloudSyncToGlobalCloudManager)
     */
    inline void resize(const size_t &newSize) { CT_StandardCloudOsgT<CT_Normal, osg::Array::Vec4ArrayType, 4, GL_FLOAT>::resize(newSize); }
};

#endif // CT_NORMALCLOUDOSG_H
