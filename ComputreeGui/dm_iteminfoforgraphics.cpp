#include "dm_iteminfoforgraphics.h"

DM_ItemInfoForGraphics::DM_ItemInfoForGraphics()
{
    m_color = Qt::white;
}

void DM_ItemInfoForGraphics::setColor(const QColor &color)
{
    m_color = color;
}
