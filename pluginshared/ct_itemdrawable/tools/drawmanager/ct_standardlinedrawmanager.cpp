#include "ct_standardlinedrawmanager.h"

#include "ct_line.h"

const QString CT_StandardLineDrawManager::INDEX_CONFIG_LINE_SIZE = CT_StandardLineDrawManager::staticInitConfigLineSize();
const QString CT_StandardLineDrawManager::INDEX_CONFIG_LINE_STYLE = CT_StandardLineDrawManager::staticInitConfigLineStyle();

CT_StandardLineDrawManager::CT_StandardLineDrawManager(QString drawConfigurationName) : CT_StandardAbstractShapeDrawManager(drawConfigurationName.isEmpty() ? "CT_Line" : drawConfigurationName)
{
    _forcedStyle = false;
}

CT_StandardLineDrawManager::~CT_StandardLineDrawManager()
{
}

void CT_StandardLineDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    CT_StandardAbstractShapeDrawManager::draw(view, painter, itemDrawable);

    const CT_Line &item = dynamic_cast<const CT_Line&>(itemDrawable);
    const CT_LineData &data = (const CT_LineData &)item.getData();

    QPen pen = _pen;

    if (!_forcedStyle)
    {
        int lineSize = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LINE_SIZE).toInt();
        int lineStyle = getDrawConfiguration()->getVariableValue(INDEX_CONFIG_LINE_STYLE).toInt();

        if (lineSize <= 0) {lineSize = 1;}
        if (lineStyle < 0) {lineSize = 0;}
        if (lineStyle > 5) {lineSize = 5;}

        pen.setWidth(lineSize);
        pen.setStyle((Qt::PenStyle) lineStyle);
        pen.setColor(item.color());
    }

    painter.setPen(pen);
    painter.drawLine(data.x1(), data.y1(), data.z1(), data.x2(), data.y2(), data.z2());
    painter.restoreDefaultPen();
}

void CT_StandardLineDrawManager::configure(const QPen &pen)
{
    _pen = pen;
    _forcedStyle = true;
}

CT_ItemDrawableConfiguration CT_StandardLineDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardLineDrawManager::staticInitConfigLineSize(), "Taille de Ligne", CT_ItemDrawableConfiguration::Int, 1);
    item.addNewConfiguration(CT_StandardLineDrawManager::staticInitConfigLineStyle(), "Style de Ligne", CT_ItemDrawableConfiguration::Int, 1);

    return item;
}

// PROTECTED //

QString CT_StandardLineDrawManager::staticInitConfigLineSize()
{
    return "LN_SZ";
}
QString CT_StandardLineDrawManager::staticInitConfigLineStyle()
{
    return "LN_ST";
}
