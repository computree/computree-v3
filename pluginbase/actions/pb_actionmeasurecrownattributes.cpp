#include "actions/pb_actionmeasurecrownattributes.h"
#include "ct_global/ct_context.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <QDebug>
#include <QVector3D>

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

PB_ActionMeasureCrownAttributes::PB_ActionMeasureCrownAttributes(CT_Grid2DXY<int> *grid, const QList<CT_Scene *> &list, float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) : CT_AbstractActionForGraphicsView()
{
    m_status = 0;

    _grid = grid;
    _sceneList.append(list);

    _xmin = xmin;
    _ymin = ymin;
    _zmin = zmin;
    _xmax = xmax;
    _ymax = ymax;
    _zmax = zmax;

    _xrange = _xmax - _xmin;
    _yrange = _ymax - _ymin;
    _projectionLevel = _zmin;
}


QString PB_ActionMeasureCrownAttributes::uniqueName() const
{
    return "PB_ActionMeasureCrownAttributes";
}

QString PB_ActionMeasureCrownAttributes::title() const
{
    return "Measure Crown";
}

QString PB_ActionMeasureCrownAttributes::description() const
{
    return "Permet de mesurer des attributs d'un houppier";
}

QIcon PB_ActionMeasureCrownAttributes::icon() const
{
    return QIcon(":/icons/show_all.png");
}

QString PB_ActionMeasureCrownAttributes::type() const
{
    return CT_AbstractAction::TYPE_MEASURE;
}

void PB_ActionMeasureCrownAttributes::init()
{
    CT_AbstractActionForGraphicsView::init();

    m_status = 0;

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionMeasureCrownAttributesOptions *option = new PB_ActionMeasureCrownAttributesOptions(this, _zmin, _zmax);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(redrawNeeded()), this, SLOT(updateGraphics()));
        connect(option, SIGNAL(askForSideView()), this, SLOT(setSideView()));
        connect(option, SIGNAL(askForCrownProjectionComputing(float,float)), this, SLOT(computeCrownProjection(float,float)));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        _projectionLevel = option->getGroundZValue();

        document()->redrawGraphics();
    }
}


bool PB_ActionMeasureCrownAttributes::mousePressEvent(QMouseEvent *e)
{
    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);

    if ((e->buttons() & Qt::LeftButton) && (e->modifiers()  & Qt::ControlModifier))
    {
            option->increaseSingleStep();
            return true;
    } else if ((e->buttons() & Qt::RightButton) && (e->modifiers()  & Qt::ControlModifier))
    {
        option->decreaseSingleStep();
        return true;
    }

    return false;
}

bool PB_ActionMeasureCrownAttributes::mouseMoveEvent(QMouseEvent *e)
{
    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);

//    if (e->buttons() & Qt::LeftButton)
//    {
//        document()->redrawGraphics();
//        return true;
//    }

    return false;
}


bool PB_ActionMeasureCrownAttributes::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionMeasureCrownAttributes::wheelEvent(QWheelEvent *e)
{
    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);

    if (e->modifiers()  & Qt::ControlModifier)
    {
        if (e->delta()>0)
        {
            option->increaseHValue();
        } else if (e->delta()<0)
        {
            option->decreaseHValue();
        }
        return true;
    }

    return false;
}

bool PB_ActionMeasureCrownAttributes::keyPressEvent(QKeyEvent *e)
{
    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);

    // Key A : pass in "Add" mode
//    if((e->key() == Qt::Key_A)
//            && !e->isAutoRepeat())
//    {
//        return true;
//    }
    return false;
}

bool PB_ActionMeasureCrownAttributes::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionMeasureCrownAttributes::updateGraphics()
{
    document()->redrawGraphics();
}

void PB_ActionMeasureCrownAttributes::setSideView()
{
    CameraInterface* camera = graphicsView()->camera();

    camera->setRZ(0);

    document()->redrawGraphics();
}

void PB_ActionMeasureCrownAttributes::computeCrownProjection(float zmin, float zmax)
{
    _grid->initGridWithValue(0);

    int size = _sceneList.size();
    for (int s = 0 ; s < size ; s++)
    {
        const CT_AbstractPointCloudIndex *pointCloudIndex = _sceneList.at(s)->getPointCloudIndex();
        size_t n_points = pointCloudIndex->size();

        for (size_t i = 0 ; i < n_points; i++)
        {
            const CT_Point &point = pointCloudIndex->constTAt(i);

            if (point.z >= zmin && point.z <= zmax)
                _grid->addValueAtXY(point.x, point.y, 1);
        }
    }
    _grid->computeMinMax();
    _grid->setlevel(zmin);
    _projectionLevel = zmin;

    document()->redrawGraphics();
}



void PB_ActionMeasureCrownAttributes::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);


    float z = option->getCurrentZValue();
    float zGround = option->getGroundZValue();

    painter.save();

    painter.setColor(QColor(125, 0, 0));
    painter.fillRectXY(QRectF(_xmin, _ymin, _xrange, _yrange), z);
    painter.drawRectXY(QRectF(_xmin, _ymin, _xrange, _yrange), z);

    if (_projectionLevel > zGround)
    {
        painter.setColor(QColor(0, 125, 0));
        painter.fillRectXY(QRectF(_xmin, _ymin, _xrange, _yrange), zGround);
        painter.drawRectXY(QRectF(_xmin, _ymin, _xrange, _yrange), zGround);
    }

    _grid->draw(view, painter);

    painter.restore();
}

void PB_ActionMeasureCrownAttributes::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    PB_ActionMeasureCrownAttributesOptions *option = (PB_ActionMeasureCrownAttributesOptions*)optionAt(0);

    if (m_status == 1)
    {
        painter.save();
        painter.setPen(QColor(255,255,0));
        painter.drawText(2, painter.fontMetrics().height()+2, tr("Press \"y\" to apply, or \"n\" to cancel !"));
        painter.restore();
    }
}

CT_AbstractAction *PB_ActionMeasureCrownAttributes::copy() const
{
    return new PB_ActionMeasureCrownAttributes(_grid, _sceneList, _xmin, _ymin, _zmin, _xmax, _ymax, _zmax);
}

