#ifndef CT_ATTRIBUTESCOLOR_H
#define CT_ATTRIBUTESCOLOR_H

#include "pluginShared_global.h"
#include "interfaces.h"

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"

/**
 * @brief A cloud attributes of type "color". Colors are applied directly to the object to colorize.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AttributesColor
{
public:
    CT_AttributesColor(CT_AbstractColorCloud *cloud = NULL);
    virtual ~CT_AttributesColor();

    const CT_Color& constColorAt(const size_t &index) const;

    void setColorCloud(CT_AbstractColorCloud *cloud);
    CT_AbstractColorCloud* getColorCloud() const;

    size_t attributesSize() const;

private:
    CT_AbstractColorCloud  *m_colorCloud;
};

#endif // CT_ATTRIBUTESCOLOR_H
