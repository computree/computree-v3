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

    virtual CT_AbstractCloud* copy() const;
};

#endif // CT_NORMALCLOUDOSG_H
