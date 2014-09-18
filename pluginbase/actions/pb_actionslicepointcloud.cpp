#include "actions/pb_actionslicepointcloud.h"
#include "ct_global/ct_context.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>

PB_ActionSlicePointCloud::PB_ActionSlicePointCloud(QList<CT_Scene *> *sceneList) : CT_AbstractActionForGraphicsView()
{
    _value = 0;
    _sceneList = sceneList;
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
    return "Action Exemple";
}

QString PB_ActionSlicePointCloud::description() const
{
    return "Exemple d'action";
}

QIcon PB_ActionSlicePointCloud::icon() const
{
    return QIcon(":/icons/select_rectangular.png");
}

QString PB_ActionSlicePointCloud::type() const
{
    return CT_AbstractAction::TYPE_INFORMATION;
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

        connect(option, SIGNAL(buttonPushed()), this, SLOT(toDoIfButtonPushed()));
        connect(option, SIGNAL(parametersChanged()), this, SLOT(redraw()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}

void PB_ActionSlicePointCloud::toDoIfButtonPushed()
{
    _value++;
}

void PB_ActionSlicePointCloud::redraw()
{
    document()->redrawGraphics();
}

bool PB_ActionSlicePointCloud::mousePressEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && (e->modifiers()  & Qt::ControlModifier))
    {
        _value++;
        redraw();
        return true;
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
    if (e->modifiers()  & Qt::ControlModifier)
    {
        if (e->delta()>0)
        {
            _value++;
        }
        redraw();
        return true;
    }

    return false;
}

bool PB_ActionSlicePointCloud::keyPressEvent(QKeyEvent *e)
{
    if((e->key() == Qt::Key_A) && !e->isAutoRepeat())
    {
        _value++;
        redraw();
        return true;
    }
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

    if (option->shouldDrawScenes())
    {
        for (int i = 0 ; i < _sceneList->size() ; i++)
        {
            _sceneList->at(i)->draw(view, painter);
        }
    }

    painter.restore();
}

void PB_ActionSlicePointCloud::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    int y = painter.fontMetrics().height()+2;

    painter.save();

    painter.setPen(QColor(255,255,255,127));
    painter.drawText(2, y, QString("Valeur = %1").arg(_value));

    painter.restore();

}

CT_AbstractAction* PB_ActionSlicePointCloud::copy() const
{
    return new PB_ActionSlicePointCloud(_sceneList);
}
