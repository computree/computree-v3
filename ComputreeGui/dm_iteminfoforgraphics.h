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

private:
    QColor  m_color;
};

#endif // DM_ITEMINFOFORGRAPHICS_H
