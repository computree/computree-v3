#include "dm_iteminfoforgraphics.h"

DM_ItemInfoForGraphics::DM_ItemInfoForGraphics()
{
    m_color = Qt::white;
    m_colorModified = false;
}

void DM_ItemInfoForGraphics::setColor(const QColor &color)
{
    m_color = color;
    m_colorModified = true;
}

void DM_ItemInfoForGraphics::setColorModified(bool e)
{
    m_colorModified = e;
}
