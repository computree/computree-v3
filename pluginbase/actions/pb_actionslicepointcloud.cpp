#include "actions/pb_actionslicepointcloud.h"
#include "ct_global/ct_context.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>

#include "math.h"

PB_ActionSlicePointCloud::PB_ActionSlicePointCloud(QList<CT_Scene *> *sceneList,
                                                   float xmin, float ymin, float zmin, float xmax, float ymax, float zmax,
                                                   double thickness, double spacing) : CT_AbstractActionForGraphicsView()
{
    _sceneList = sceneList;
    _xmin = xmin;
    _ymin = ymin;
    _zmin = zmin;

    _xmax = xmax;
    _ymax = ymax;
    _zmax = zmax;

    _xwidth = std::abs(_xmax - _xmin);
    _ywidth = std::abs(_ymax - _ymin);

    _thickness = thickness;
    _spacing = spacing;
}

PB_ActionSlicePointCloud::~PB_ActionSlicePointCloud()
{
}

QString PB_ActionSlicePointCloud::uniqueName() const
{
    return "PB_ActionSlicePointCloud";
}

QString PB_ActionSlicePointCloud::title() const
{
    return "Action Slicing";
}

QString PB_ActionSlicePointCloud::description() const
{
    return "Action Slicing";
}

QIcon PB_ActionSlicePointCloud::icon() const
{
    return QIcon(":/icons/select_rectangular.png");
}

QString PB_ActionSlicePointCloud::type() const
{
    return CT_AbstractAction::TYPE_MODIFICATION;
}

void PB_ActionSlicePointCloud::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionSlicePointCloudOptions *option = new PB_ActionSlicePointCloudOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(redraw()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}



void PB_ActionSlicePointCloud::redraw()
{
    document()->redrawGraphics();
}

bool PB_ActionSlicePointCloud::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionSlicePointCloud::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionSlicePointCloud::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionSlicePointCloud::wheelEvent(QWheelEvent *e)
{
    PB_ActionSlicePointCloudOptions *option = (PB_ActionSlicePointCloudOptions*)optionAt(0);

    if (e->modifiers()  & Qt::ControlModifier)
    {
        if (e->delta()>0)
        {
            option->setThickness(option->getThickness() + option->getIncrement());
        } else if (e->delta()>0)
        {
            option->setThickness(option->getThickness() - option->getIncrement());
        }
        redraw();
        return true;
    } else if (e->modifiers()  & Qt::ShiftModifier)
    {
        if (e->delta()>0)
        {
            option->setSpacing(option->getSpacing() + option->getIncrement());
        } else if (e->delta()>0)
        {
            option->setSpacing(option->getSpacing() - option->getIncrement());
        }
        redraw();
        return true;
    }

    return false;
}

bool PB_ActionSlicePointCloud::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;

}

bool PB_ActionSlicePointCloud::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionSlicePointCloud::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    PB_ActionSlicePointCloudOptions *option = (PB_ActionSlicePointCloudOptions*)optionAt(0);

    painter.save();

    for (int i = 0 ; i < _sceneList->size() ; i++)
    {
        _sceneList->at(i)->draw(view, painter);
    }

    double z_current = _zmin;
    while (z_current <= _zmax)
    {
        painter.drawRectXY(QRectF(_xmin, _ymin, _xwidth, _ywidth), z_current);
        painter.drawRectXY(QRectF(_xmin, _ymin, _xwidth, _ywidth), z_current + _thickness);
        painter.drawRectXZ(QRectF(_xmin, z_current, _xwidth, (float)_thickness), _ymin);
        painter.drawRectXZ(QRectF(_xmin, z_current, _xwidth, (float)_thickness), _ymax);
        painter.drawRectYZ(QRectF(_ymin, z_current, _ywidth, (float)_thickness), _xmin);
        painter.drawRectYZ(QRectF(_ymin, z_current, _ywidth, (float)_thickness), _xmax);

        z_current += _thickness;
        z_current += _spacing;
    }

    painter.restore();
}

void PB_ActionSlicePointCloud::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view);
    Q_UNUSED(painter);
}

CT_AbstractAction* PB_ActionSlicePointCloud::copy() const
{
    return new PB_ActionSlicePointCloud(_sceneList, _xmin, _ymin, _zmin, _xmax, _ymax, _zmax, _thickness, _spacing);
}
