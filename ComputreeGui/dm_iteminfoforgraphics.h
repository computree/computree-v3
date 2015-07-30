#ifndef DM_ITEMINFOFORGRAPHICS_H
#define DM_ITEMINFOFORGRAPHICS_H

#include "dm_abstractinfo.h"

#include <QColor>

/**
 * @brief Represent a class that store information about an item in a graphics view (like color, etc..)
 */
class DM_ItemInfoForGraphics : public DM_AbstractInfo
{
public:
    DM_ItemInfoForGraphics();

    void setColor(const QColor &color);
    inline QColor& color() { return m_color; }

    inline bool isColorModified() { return m_colorModified; }
    void setColorModified(bool e);

private:
    QColor  m_color;
    bool    m_colorModified;
};

#endif // DM_ITEMINFOFORGRAPHICS_H
