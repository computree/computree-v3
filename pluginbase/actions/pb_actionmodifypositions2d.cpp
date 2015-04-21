#include "actions/pb_actionmodifypositions2d.h"
#include "ct_global/ct_context.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <limits>


PB_ActionModifyPositions2D::PB_ActionModifyPositions2D(QList<CT_Point2D*> &positions, CT_OutAbstractSingularItemModel *model, CT_ResultGroup* outRes) : CT_AbstractActionForGraphicsView()
{
    _positions = &positions;
    _model = model;
    _outRes = outRes;

    _selectedColor = QColor(0, 255, 0);
    _normalColor   = QColor(255, 0, 0);

    _selectedPoint = NULL;
    _leftButton = false;
}

PB_ActionModifyPositions2D::~PB_ActionModifyPositions2D()
{
}

QString PB_ActionModifyPositions2D::uniqueName() const
{
    return "PB_ActionModifyPositions2D";
}

QString PB_ActionModifyPositions2D::title() const
{
    return tr("Modifier positions 2D");
}

QString PB_ActionModifyPositions2D::description() const
{
    return tr("Modifier positions 2D");
}

QIcon PB_ActionModifyPositions2D::icon() const
{
    return QIcon(":/icons/select_rectangular.png");
}

QString PB_ActionModifyPositions2D::type() const
{
    return CT_AbstractAction::TYPE_MODIFICATION;
}

void PB_ActionModifyPositions2D::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionModifyPositions2DOptions *option = new PB_ActionModifyPositions2DOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(zValChanged()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        _min(0) = std::numeric_limits<double>::max();
        _min(1) = std::numeric_limits<double>::max();

        _max(0) = -std::numeric_limits<double>::max();
        _max(1) = -std::numeric_limits<double>::max();

        for (int i = 0 ; i < _positions->size() ; i++)
        {
            CT_Point2D *point = _positions->at(i);
            if (point->x() < _min(0)) {_min(0) = point->x();}
            if (point->y() < _min(1)) {_min(1) = point->y();}

            if (point->x() > _max(0)) {_max(0) = point->x();}
            if (point->y() > _max(1)) {_max(1) = point->y();}

            document()->addItemDrawable(*point);
            document()->setColor(point, _normalColor);
        }

        document()->fitToContent();
    }
}

void PB_ActionModifyPositions2D::zValChanged()
{
    if (_positions->size()>0)
    {
        PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

       ((CT_StandardPoint2DDrawManager*) _positions->first()->getBaseDrawManager())->setZValue(option->getZValue());
    }

    document()->redrawGraphics();
}


bool PB_ActionModifyPositions2D::mousePressEvent(QMouseEvent *e)
{
    PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

    if (e->button() == Qt::LeftButton)
    {
        _leftButton = true;
        if (option->isMovePositionSelected() || option->isRemovePositionSelected() || option->isAddPositionSelected())
        {
            double x, y;
            if (getCoordsForMousePosition(e, x, y))
            {
                if (option->isMovePositionSelected() || option->isRemovePositionSelected())
                {
                    _selectedPoint = getNearestPosition(x, y);
                } else if (option->isAddPositionSelected())
                {
                    _selectedPoint = new CT_Point2D(_model, _outRes, new CT_Point2DData(x, y));
                    _positions->append(_selectedPoint);

                    document()->addItemDrawable(*_selectedPoint);
                }

                if (_selectedPoint != NULL)
                {
                    document()->setColor(_selectedPoint, _selectedColor);
                    document()->redrawGraphics();
                    return true;
                }
            }
        }

    }

    return false;
}

bool PB_ActionModifyPositions2D::mouseMoveEvent(QMouseEvent *e)
{
    PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

    if (_leftButton && (option->isMovePositionSelected() || option->isAddPositionSelected()))
    {
        if (_selectedPoint != NULL)
        {
            double x, y;
            if (getCoordsForMousePosition(e, x, y))
            {
                document()->lock();
                _selectedPoint->setCenterX(x);
                _selectedPoint->setCenterY(y);
                document()->unlock();

                document()->redrawGraphics();
                return true;
            }
        }
    }

    return false;
}

bool PB_ActionModifyPositions2D::mouseReleaseEvent(QMouseEvent *e)
{   
    PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

    if (e->button() == Qt::LeftButton)
    {
        _leftButton = false;
        if (_selectedPoint != NULL)
        {
            if (option->isMovePositionSelected() || option->isAddPositionSelected())
            {
                double x, y;
                if (getCoordsForMousePosition(e, x, y))
                {
                    document()->lock();
                    _selectedPoint->setCenterX(x);
                    _selectedPoint->setCenterY(y);
                    document()->unlock();

                    document()->setColor(_selectedPoint, _normalColor);
                    document()->redrawGraphics();
                }
            } else if (option->isRemovePositionSelected())
            {
                document()->removeItemDrawable(*_selectedPoint);

                _positions->removeOne(_selectedPoint);
                delete _selectedPoint;
                _selectedPoint = NULL;
                option->selectFreeMove();
                document()->redrawGraphics();
            }

            _selectedPoint = NULL;
            return true;
        }
    }

    return false;
}

bool PB_ActionModifyPositions2D::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return false;

}

bool PB_ActionModifyPositions2D::keyPressEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_Control) && !e->isAutoRepeat())
    {
        PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);
        option->selectMovePosition();
        return true;
    }

    if((e->key() == Qt::Key_Shift) && !e->isAutoRepeat())
    {
        PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);
        option->selectAddPosition();
        return true;
    }

    if((e->key() == Qt::Key_Delete) && !e->isAutoRepeat())
    {
        PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);
        if (option->isRemovePositionSelected())
        {
            if (_selectedPoint != NULL)
            {
                document()->setColor(_selectedPoint, _normalColor);
                _selectedPoint = NULL;
            }
            option->selectFreeMove();
        } else  {
            option->selectRemovePosition();
        }
        return true;
    }

    return false;

}

bool PB_ActionModifyPositions2D::keyReleaseEvent(QKeyEvent *e)
{
    if (_selectedPoint == NULL)
    {
        if(((e->key() == Qt::Key_Control) || (e->key() == Qt::Key_Shift)) && !e->isAutoRepeat())
        {
            PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);
            option->selectFreeMove();
            return true;
        }
    }

    return false;
}

void PB_ActionModifyPositions2D::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

    painter.save();

    painter.setColor(QColor(75, 75, 75, 125));
    if (option->isDrawPlaneSelected()) {painter.fillRectXY(_min, _max, option->getZValue() - 0.10);}

    painter.restore();
}

void PB_ActionModifyPositions2D::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
}

CT_AbstractAction* PB_ActionModifyPositions2D::copy() const
{
    return new PB_ActionModifyPositions2D(*_positions, _model, _outRes);
}

bool PB_ActionModifyPositions2D::getCoordsForMousePosition(const QMouseEvent *e, double &x, double &y)
{
    PB_ActionModifyPositions2DOptions *option = (PB_ActionModifyPositions2DOptions*)optionAt(0);

    Eigen::Vector3d origin, direction;
    GraphicsViewInterface *view = graphicsView();
    view->convertClickToLine(e->pos(), origin, direction);

    if (direction.z() == 0) {return false;}

    double coef = (option->getZValue() - origin.z())/direction.z();

    x = origin.x() + coef*direction.x();
    y = origin.y() + coef*direction.y();

    return true;
}

CT_Point2D *PB_ActionModifyPositions2D::getNearestPosition(double x, double y)
{
    double minDist = std::numeric_limits<double>::max();
    CT_Point2D* nearestPoint = NULL;

    for (int i = 0 ; i < _positions->size() ; i++)
    {
        CT_Point2D *point = _positions->at(i);
        double dist = pow(point->x() - x, 2) + pow(point->y() - y, 2);

        if (dist < minDist)
        {
            minDist = dist;
            nearestPoint = point;
        }
    }

    return nearestPoint;
}
