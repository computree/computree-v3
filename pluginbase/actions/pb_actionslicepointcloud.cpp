#include "actions/pb_actionslicepointcloud.h"
#include "ct_global/ct_context.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>

#include "math.h"


PB_ActionSlicePointCloud_dataContainer::PB_ActionSlicePointCloud_dataContainer()
{
    _thickness = 0;
    _spacing = 0;
}

PB_ActionSlicePointCloud::PB_ActionSlicePointCloud(QList<CT_Scene *> *sceneList,
                                                   float xmin, float ymin, float zmin, float xmax, float ymax, float zmax,
                                                   PB_ActionSlicePointCloud_dataContainer *dataContainer) : CT_AbstractActionForGraphicsView()
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

    _dataContainer = dataContainer;
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

        option->setThickness(_dataContainer->_thickness);
        option->setSpacing(_dataContainer->_spacing);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(update()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);
        for (int i = 0 ; i < _sceneList->size() ; i++)
        {
            document()->addItemDrawable(*(_sceneList->at(i)));
        }
        document()->redrawGraphics(DocumentInterface::RO_WaitForConversionCompleted);
        dynamic_cast<GraphicsViewInterface*>(document()->views().first())->camera()->fitCameraToVisibleItems();
    }
}

void PB_ActionSlicePointCloud::update()
{
    PB_ActionSlicePointCloudOptions *option = (PB_ActionSlicePointCloudOptions*)optionAt(0);

    _dataContainer->_thickness = option->getThickness();
    _dataContainer->_spacing = option->getSpacing();

    redrawOverlayAnd3D();
}

void PB_ActionSlicePointCloud::redrawOverlay()
{
    document()->redrawGraphics();
}

void PB_ActionSlicePointCloud::redrawOverlayAnd3D()
{
    setDrawing3DChanged();
    document()->redrawGraphics();
}

bool PB_ActionSlicePointCloud::mousePressEvent(QMouseEvent *e)
{
    PB_ActionSlicePointCloudOptions *option = (PB_ActionSlicePointCloudOptions*)optionAt(0);

    if ((e->modifiers() & Qt::ControlModifier) || (e->modifiers()  & Qt::ShiftModifier))
    {
        if (e->buttons() & Qt::LeftButton)
        {
            option->decreaseIncrement();
            update();
            return true;
        } else if (e->buttons() & Qt::RightButton)
        {
            option->increaseIncrement();
            update();
            return true;
        }
    }

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
        if (e->delta() > 0)
        {
            option->setThickness(option->getThickness() + option->getIncrement());
        } else if (e->delta() < 0)
        {
            option->setThickness(option->getThickness() - option->getIncrement());
        }
        update();
        return true;
    } else if (e->modifiers()  & Qt::ShiftModifier)
    {
        if (e->delta() > 0)
        {
            option->setSpacing(option->getSpacing() + option->getIncrement());
        } else if (e->delta() < 0)
        {
            option->setSpacing(option->getSpacing() - option->getIncrement());
        }
        update();
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

    if (_dataContainer->_thickness == 0) {return;}

    painter.save();

    QColor oldColor = painter.getColor();
    painter.setColor(QColor(0,125,0,100));

    double z_current = _zmin;
    while (z_current <= _zmax)
    {
        painter.fillRectXY(Eigen::Vector2d(_xmin, _ymin), Eigen::Vector2d(_xmin+_xwidth, _ymin+_ywidth), z_current);
        painter.fillRectXY(Eigen::Vector2d(_xmin, _ymin), Eigen::Vector2d(_xmin+_xwidth, _ymin+_ywidth), z_current + _dataContainer->_thickness);
        painter.drawRectXZ(Eigen::Vector2d(_xmin, z_current), Eigen::Vector2d(_xmin+_xwidth, z_current+_dataContainer->_thickness), _ymin);
        painter.drawRectXZ(Eigen::Vector2d(_xmin, z_current), Eigen::Vector2d(_xmin+_xwidth, z_current+_dataContainer->_thickness), _ymax);
        painter.drawRectYZ(Eigen::Vector2d(_ymin, z_current), Eigen::Vector2d(_ymin+_ywidth, z_current+_dataContainer->_thickness), _xmin);
        painter.drawRectYZ(Eigen::Vector2d(_ymin, z_current), Eigen::Vector2d(_ymin+_ywidth, z_current+_dataContainer->_thickness), _xmax);

        z_current += _dataContainer->_thickness;
        z_current += _dataContainer->_spacing;
    }
    painter.setColor(oldColor);
    painter.restore();
}

void PB_ActionSlicePointCloud::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view);
    Q_UNUSED(painter);
}

CT_AbstractAction* PB_ActionSlicePointCloud::copy() const
{
    return new PB_ActionSlicePointCloud(_sceneList, _xmin, _ymin, _zmin, _xmax, _ymax, _zmax, _dataContainer);
}
