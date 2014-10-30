#include "actions/pb_actionpickitemsinlist.h"
#include "ct_global/ct_context.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <limits>
#include <QVector3D>

#include "ct_math/ct_mathpoint.h"

PB_ActionPickItemsInList::PB_ActionPickItemsInList(const QList<CT_AbstractItemDrawable *> &itemList, QList<CT_AbstractItemDrawable *> *selectedItems, float maxDist) : CT_AbstractActionForGraphicsView()
{
    _itemList.append(itemList);
    _selectedItems = selectedItems;
    _maxDist = maxDist;
}

PB_ActionPickItemsInList::~PB_ActionPickItemsInList()
{
}

QString PB_ActionPickItemsInList::uniqueName() const
{
    return "PB_ActionPickItemsInList";
}

QString PB_ActionPickItemsInList::title() const
{
    return tr("Picking d'item");
}

QString PB_ActionPickItemsInList::description() const
{
    return tr("Picking d'item");
}

QIcon PB_ActionPickItemsInList::icon() const
{
    return QIcon(":/icons/select_rectangular.png");
}

QString PB_ActionPickItemsInList::type() const
{
    return CT_AbstractAction::TYPE_INFORMATION;
}

void PB_ActionPickItemsInList::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionPickItemsInListOptions *option = new PB_ActionPickItemsInListOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(redraw()));
        connect(option, SIGNAL(allClicked()), this, SLOT(selectAll()));
        connect(option, SIGNAL(noneClicked()), this, SLOT(selectNone()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}

void PB_ActionPickItemsInList::redraw()
{
    document()->redrawGraphics();
}

void PB_ActionPickItemsInList::selectAll()
{
    _selectedItems->clear();
    _selectedItems->append(_itemList);
    redraw();
}

void PB_ActionPickItemsInList::selectNone()
{
    _selectedItems->clear();
    redraw();
}


bool PB_ActionPickItemsInList::mousePressEvent(QMouseEvent *e)
{
    if (e->modifiers() & Qt::ShiftModifier)
    {
        _buttonsPressed = e->buttons();
    } else  {
        _buttonsPressed = Qt::NoButton;
    }

    _oldPos = e->pos();
    return false;
}

bool PB_ActionPickItemsInList::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionPickItemsInList::mouseReleaseEvent(QMouseEvent *e)
{   
    QPoint point = e->pos() - _oldPos;

    if (point.manhattanLength() <= 3)
    {
        if (_buttonsPressed == Qt::LeftButton)
        {
            addItemToSelection(e->pos());
        } else if (_buttonsPressed == Qt::RightButton){
            removeItemFromSelection(e->pos());
        }
        redraw();
    }
    return false;
}

bool PB_ActionPickItemsInList::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return false;

}

bool PB_ActionPickItemsInList::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;

}

bool PB_ActionPickItemsInList::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionPickItemsInList::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    PB_ActionPickItemsInListOptions *option = (PB_ActionPickItemsInListOptions*)optionAt(0);

    painter.save();

    QListIterator<CT_AbstractItemDrawable*> it(_itemList);
    while (it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if (_selectedItems->contains(item))
        {
            painter.setColor(QColor(255,0,0));
        } else {
            painter.setColor(option->getColor());
        }
        item->draw(view, painter);
    }

    painter.restore();
}

void PB_ActionPickItemsInList::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
}

CT_AbstractAction* PB_ActionPickItemsInList::copy() const
{
    return new PB_ActionPickItemsInList(_itemList, _selectedItems, _maxDist);
}

void PB_ActionPickItemsInList::addItemToSelection(const QPoint &point)
{
    QVector3D origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    CT_AbstractItemDrawable* pickedItem = NULL;

    QListIterator<CT_AbstractItemDrawable*> it(_itemList);
    while (it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        QVector3D point(item->getCenterX(), item->getCenterY(), item->getCenterZ());
        float dist = CT_MathPoint::distancePointLine<QVector3D>(point, direction, origin);

        if (dist < minDist && dist < _maxDist)
        {
            minDist = dist;
            pickedItem = item;
        }
    }

    if (pickedItem != NULL && !_selectedItems->contains(pickedItem))
    {
        _selectedItems->append(pickedItem);
    }
}

void PB_ActionPickItemsInList::removeItemFromSelection(const QPoint &point)
{
    QVector3D origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    CT_AbstractItemDrawable* pickedItem = NULL;

    QListIterator<CT_AbstractItemDrawable*> it(*_selectedItems);
    while (it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        QVector3D point(item->getCenterX(), item->getCenterY(), item->getCenterZ());
        float dist = CT_MathPoint::distancePointLine<QVector3D>(point, direction, origin);

        if (dist < minDist && dist < _maxDist)
        {
            minDist = dist;
            pickedItem = item;
        }
    }

    if (pickedItem != NULL && _selectedItems->contains(pickedItem))
    {
        _selectedItems->removeOne(pickedItem);
    }
}
