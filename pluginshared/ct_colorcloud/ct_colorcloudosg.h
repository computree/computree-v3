#ifndef CT_COLORCLOUDOSG_H
#define CT_COLORCLOUDOSG_H

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_cloud/ct_standardcloudosgt.h"
#include "ct_color.h"

class PLUGINSHAREDSHARED_EXPORT CT_ColorCloudOsg : public CT_StandardCloudOsgT<CT_Color, osg::Array::Vec4ubArrayType, 4, GL_UNSIGNED_BYTE>, public CT_AbstractColorCloud
{
public:
    CT_ColorCloudOsg(const size_t &initialSize);

    /**
     * @brief Returns the number of colors
     */
    size_t size() const;

    /**
     * @brief Add the normal color in parameter to the cloud
     */
    void addColor(const CT_Color &color);

    /**
     * @brief Add a default color to the cloud and return a reference to it
     */
    CT_Color& addColor();

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Color& colorAt(const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Color& constColorAt(const size_t &index) const;

    /**
     * @brief Returns a reference to the color at index 'index'
     */
    CT_Color& operator[](const size_t &index);

    /**
     * @brief Returns a const reference to the color at index 'index'
     */
    const CT_Color& operator[](const size_t &index) const;

    virtual CT_AbstractCloud* copy() const;
};

#endif // CT_COLORCLOUDOSG_H
