#include "pb_actionselectitemdrawablegv.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <math.h>

#include "views/actions/pb_actionselectitemdrawablegvoptions.h"

PB_ActionSelectItemDrawableGV::PB_ActionSelectItemDrawableGV() : CT_AbstractActionForGraphicsView()
{
    m_status = 0;
    m_mousePressed = false;
    m_selectionMode = GraphicsViewInterface::SELECT_ONE;
//    m_drawMode = SELECT_CURRENT_MODE;
}

QString PB_ActionSelectItemDrawableGV::uniqueName() const
{
    return "PB_ActionSelectItemDrawableGV";
}

QString PB_ActionSelectItemDrawableGV::title() const
{
    return tr("Sélection");
}

QString PB_ActionSelectItemDrawableGV::description() const
{
    return tr("Sélection d'éléments");
}

QIcon PB_ActionSelectItemDrawableGV::icon() const
{
    return QIcon(":/icons/cursor.png");
}

QString PB_ActionSelectItemDrawableGV::type() const
{
    return CT_AbstractAction::TYPE_SELECTION;
}

void PB_ActionSelectItemDrawableGV::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionSelectItemDrawableGVOptions *option = new PB_ActionSelectItemDrawableGVOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);
    }
}

bool PB_ActionSelectItemDrawableGV::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
        return false;

    GraphicsViewInterface *view = graphicsView();

//    m_backupDrawMode = view->drawMode();

//    setNewDrawMode();

    view->setSelectionMode(selectionMode());

    GraphicsViewInterface::SelectionMode mode = selectionModeToBasic(view->selectionMode());

    m_mousePressed = true;
    m_status = 1;
    m_selectionRectangle.setSize(QSize(0,0));

    if(((mode == GraphicsViewInterface::SELECT)
            || (mode == GraphicsViewInterface::ADD)
            || (mode == GraphicsViewInterface::REMOVE)))
    {
        m_selectionRectangle = QRect(e->pos(), e->pos());

        size_t size = 0;

        if(view->mustSelectPoints())
            size = view->countPoints();
        else if(view->mustSelectEdges())
            size = view->countEdges();
        else if(view->mustSelectFaces())
            size = view->countFaces();
        else
            size = view->countItems();

        if(size == 0)
            size = 1;

        if(size != view->selectBufferSize())
            view->setSelectBufferSize(size*4);

        return true;
    }

    return false;
}

bool PB_ActionSelectItemDrawableGV::mouseMoveEvent(QMouseEvent *e)
{
    if(m_status > 0)
    {
        GraphicsViewInterface *view = graphicsView();

        GraphicsViewInterface::SelectionMode mode = selectionModeToBasic(view->selectionMode());

        if((mode == GraphicsViewInterface::ADD_ONE)
            || (mode == GraphicsViewInterface::REMOVE_ONE)
            || (mode == GraphicsViewInterface::SELECT_ONE))
        {
            view->setSelectionMode(GraphicsViewInterface::NONE);

            m_status = 0;
            return false;
        }

        if(mode != GraphicsViewInterface::NONE)
        {
//            setNewDrawMode();

            m_selectionRectangle.setBottomRight(e->pos());
            document()->redrawGraphics();

            return true;
        }
    }

    return false;
}

bool PB_ActionSelectItemDrawableGV::mouseReleaseEvent(QMouseEvent *e)
{
    GraphicsViewInterface *view = graphicsView();

    GraphicsViewInterface::SelectionMode mode = selectionModeToBasic(view->selectionMode());

    if(e->button() == Qt::LeftButton)
        m_mousePressed = false;

//    if(mode != GraphicsViewInterface::NONE)
//        setNewDrawMode();

    if((m_status > 0)
            && (e->button() == Qt::LeftButton))
    {
        m_status = 0;
        //m_backupDrawMode = view->drawMode();
        //view->setDrawMode(GraphicsViewInterface::NORMAL);

        if(mode != GraphicsViewInterface::NONE)
        {
            if(view->mustSelectPoints())
                document()->constructOctreeOfPoints();

            if((mode == GraphicsViewInterface::ADD_ONE)
                || (mode == GraphicsViewInterface::REMOVE_ONE)
                || (mode == GraphicsViewInterface::SELECT_ONE))
            {
                view->setSelectRegionWidth(3);
                view->setSelectRegionHeight(3);

                view->select(e->pos());

                //view->setDrawMode(m_backupDrawMode);
            }
            else
            {
                m_selectionRectangle = m_selectionRectangle.normalized();

                // Define selection window dimensions
                view->setSelectRegionWidth(m_selectionRectangle.width());
                view->setSelectRegionHeight(m_selectionRectangle.height());
                // Compute rectangle center and perform selection
                view->select(m_selectionRectangle.center());

    //            setBackupDrawMode();
                //view->setDrawMode(m_backupDrawMode);
                document()->redrawGraphics();

                return true;
            }
        }
    }

    document()->redrawGraphics();

    return false;
}

bool PB_ActionSelectItemDrawableGV::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

bool PB_ActionSelectItemDrawableGV::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

void PB_ActionSelectItemDrawableGV::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    if(m_status > 0)
    {
        painter.save();
        painter.setPen(QColor(102,102,127,127));
        painter.setBrush(QColor(0,0,73,73));
        painter.drawRect(m_selectionRectangle);
        painter.restore();
    }
}

CT_AbstractAction* PB_ActionSelectItemDrawableGV::copy() const
{
    return new PB_ActionSelectItemDrawableGV();
}

bool PB_ActionSelectItemDrawableGV::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    if(!m_mousePressed)
    {
        m_selectionMode = mode;
        return true;
    }

    return false;
}

//bool PB_ActionSelectItemDrawableGV::setDrawMode(PB_ActionSelectItemDrawableGV::SelectionDrawMode mode)
//{
//    if(!m_mousePressed)
//    {
//        m_drawMode = mode;
//        return true;
//    }

//    return false;
//}

GraphicsViewInterface::SelectionMode PB_ActionSelectItemDrawableGV::selectionMode() const
{
    return m_selectionMode;
}

//PB_ActionSelectItemDrawableGV::SelectionDrawMode PB_ActionSelectItemDrawableGV::drawMode() const
//{
//    return m_drawMode;
//}

//void PB_ActionSelectItemDrawableGV::setNewDrawMode()
//{
//    GraphicsViewInterface *view = graphicsView();
//    view->setDrawModeChangeTime(0);

//    if(drawMode() != SELECT_CURRENT_MODE)
//        view->setDrawMode((GraphicsViewInterface::DrawMode)drawMode());
//    else
//        view->setDrawMode(m_backupDrawMode);
//}

//void PB_ActionSelectItemDrawableGV::setBackupDrawMode()
//{
//    GraphicsViewInterface *view = graphicsView();

//    view->setDrawModeChangeTime(view->getOptions().getFastDrawTime());
//}

GraphicsViewInterface::SelectionMode PB_ActionSelectItemDrawableGV::selectionModeToBasic(GraphicsViewInterface::SelectionMode mode) const
{
    int m = mode;

    while(m > GraphicsViewInterface::REMOVE_ONE)
        m -= GraphicsViewInterface::REMOVE_ONE;

    return (GraphicsViewInterface::SelectionMode)m;
}
