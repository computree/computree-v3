#include "actions/pb_actionmanualinventory.h"
#include "ct_global/ct_context.h"
#include "views/actions/pb_actionmanualinventoryattributesdialog.h"


#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <limits>

#include "ct_math/ct_mathpoint.h"

PB_ActionManualInventory::PB_ActionManualInventory(QMap<const CT_Scene*, const CT_Circle*> *selectedDbh,
                                                   QMultiMap<const CT_Scene *, const CT_Circle*> *availableDbh,
                                                   QMap<QString, QStringList> *paramData,
                                                   QMap<const CT_Scene*, QMap<QString, QString> >  *suppAttributes) : CT_AbstractActionForGraphicsView()
{
    _selectedDbh = selectedDbh;
    _availableDbh = availableDbh;
    _paramData = paramData;
    _suppAttributes = suppAttributes;
    _currentCircle = NULL;
    _currentScene = NULL;

    _activeSceneCirclesColor = Qt::blue;
    _othersScenesCirclesColor = Qt::darkGray;
    _othersScenesCirclesLightColor = Qt::lightGray;
    _othersCircleColor = Qt::darkRed;
    _currentCircleColor = Qt::red;
}

PB_ActionManualInventory::~PB_ActionManualInventory()
{
}

QString PB_ActionManualInventory::uniqueName() const
{
    return "PB_ActionManualInventory";
}

QString PB_ActionManualInventory::title() const
{
    return tr("Inventaire Manuel");
}

QString PB_ActionManualInventory::description() const
{
    return tr("Inventaire Manuel");
}

QIcon PB_ActionManualInventory::icon() const
{
    return QIcon(":/icons/cursor.png");
}

QString PB_ActionManualInventory::type() const
{
    return CT_AbstractAction::TYPE_MEASURE;
}

void PB_ActionManualInventory::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionManualInventoryOptions *option = new PB_ActionManualInventoryOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(visibilityChanged()), this, SLOT(visibilityChanged()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        QMapIterator<const CT_Scene*, const CT_Circle*> it(*_selectedDbh);
        if (it.hasNext())
        {
            it.next();

            _currentScene = (CT_Scene*) it.key();
            _currentCircle = (CT_Circle*) it.value();
        }

        visibilityChanged();
        document()->fitToContent();
    }
}

bool PB_ActionManualInventory::mousePressEvent(QMouseEvent *e)
{
    _oldPos = e->pos();

    if (e->modifiers() & Qt::ShiftModifier)
    {
        _buttonsPressed = e->buttons();
        return true;
    } else  {
        _buttonsPressed = Qt::NoButton;
    }
    return false;
}

bool PB_ActionManualInventory::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionManualInventory::mouseReleaseEvent(QMouseEvent *e)
{   
    PB_ActionManualInventoryOptions *option = (PB_ActionManualInventoryOptions*)optionAt(0);

    QPoint point = e->pos() - _oldPos;

    if (point.manhattanLength() <= 3)
    {
        if (_buttonsPressed == Qt::LeftButton)
        {

            if (option->isDbhModeSelected())
            {
                chooseForDbh(e->pos());
            } else if (option->isAttributesModeSelected()){
                chooseForAttributes(e->pos());
            } else {
                selectActiveScene(e->pos());
            }

            if (_currentCircle!=NULL && option->shouldAutoCenterCamera())
            {
                graphicsView()->camera()->setCX(_currentCircle->getCenterX());
                graphicsView()->camera()->setCY(_currentCircle->getCenterY());
                graphicsView()->camera()->setCZ(_currentCircle->getCenterZ());
            }
            return true;
        }
    }
    return false;
}

bool PB_ActionManualInventory::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionManualInventory::keyPressEvent(QKeyEvent *e)
{   
    Q_UNUSED(e);
    return false;

}

bool PB_ActionManualInventory::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionManualInventory::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)
}

void PB_ActionManualInventory::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    PB_ActionManualInventoryOptions *option = (PB_ActionManualInventoryOptions*)optionAt(0);
    int add = painter.fontMetrics().height()+2;

    painter.setPen(QColor(255,255,255,127));

    if (_currentCircle != NULL && _currentScene != NULL)
    {
        int y =  add + 2;

        painter.drawText(2, y, tr("Attributs du cercle actif :"));
        y += add;
        painter.drawText(2, y, tr("Z = %1 m").arg(_currentCircle->getCenterZ()));
        y += add;

        QMap<QString, QString> attrMap = _suppAttributes->value(_currentScene);
        QMapIterator<QString, QString> it(attrMap);
        while (it.hasNext())
        {
            it.next();
            painter.drawText(2, y, tr("%1 = %2").arg(it.key()).arg(it.value()));
            y += add;
        }
    }



    painter.setPen(Qt::darkRed);

    if (option->isShowDataChecked())
    {
        QMapIterator<const CT_Scene*, const CT_Circle*> it(*_selectedDbh);
        while (it.hasNext())
        {
            it.next();
            CT_Scene* scene = (CT_Scene*) it.key();
            CT_Circle* circle = (CT_Circle*) it.value();

            QMap<QString, QString> attrMap = _suppAttributes->value(scene);

            QPoint pixel;
            graphicsView()->convert3DPositionToPixel(Eigen::Vector3d(circle->getCenterX(), circle->getCenterY(), circle->getCenterZ()), pixel);

            int y =  pixel.y() + add + 2;

            QMapIterator<QString, QString> it(attrMap);
            while (it.hasNext())
            {
                it.next();
                painter.drawText(pixel.x(), y, tr("%1 = %2").arg(it.key()).arg(it.value()));
                y += add;
            }

        }
    }
}

CT_AbstractAction* PB_ActionManualInventory::copy() const
{
    return new PB_ActionManualInventory(_selectedDbh, _availableDbh, _paramData, _suppAttributes);
}

void PB_ActionManualInventory::chooseForDbh(const QPoint &point)
{
    Eigen::Vector3d origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    double minDist = std::numeric_limits<double>::max();
    CT_Circle* pickedItem = NULL;
    CT_Scene*  pickedItemScene = NULL;

    QMapIterator<const CT_Scene*, const CT_Circle*> it(*_availableDbh);
    while (it.hasNext())
    {
        it.next();
        CT_Scene* scene = (CT_Scene*) it.key();
        CT_Circle* circle = (CT_Circle*) it.value();

        double dist = CT_MathPoint::distancePointLine(circle->getCenterCoordinate(), direction, origin);

        if (dist < minDist)
        {
            minDist = dist;
            pickedItem = circle;
            pickedItemScene = scene;
        }
    }

    if (pickedItem != NULL && pickedItemScene != NULL)
    {
        _selectedDbh->insert(pickedItemScene, pickedItem);
    }

    if (pickedItemScene != _currentScene)
    {
        _currentCircle = pickedItem;
        _currentScene = pickedItemScene;
        visibilityChanged();

    } else if (pickedItem != _currentCircle)
    {
        CT_Circle* oldCircle = _currentCircle;
        _currentCircle = pickedItem;

        document()->removeItemDrawable(*oldCircle);
        document()->removeItemDrawable(*_currentCircle);
        updateVisibility(_currentScene, oldCircle);
        updateVisibility(_currentScene, _currentCircle);
        document()->redrawGraphics();
    }
}

void PB_ActionManualInventory::chooseForAttributes(const QPoint &point)
{
    selectActiveScene(point);

    setAttributes(_currentScene);
    document()->redrawGraphics();
}

void PB_ActionManualInventory::selectActiveScene(const QPoint &point)
{
    Eigen::Vector3d origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    double minDist = std::numeric_limits<double>::max();
    CT_Circle* pickedItem = NULL;
    CT_Scene*  pickedItemScene = NULL;

    QMapIterator<const CT_Scene*, const CT_Circle*> it(*_selectedDbh);
    while (it.hasNext())
    {
        it.next();
        CT_Scene* scene = (CT_Scene*) it.key();
        CT_Circle* circle = (CT_Circle*) it.value();

        double dist = CT_MathPoint::distancePointLine(circle->getCenterCoordinate(), direction, origin);

        if (dist < minDist)
        {
            minDist = dist;
            pickedItem = circle;
            pickedItemScene = scene;
        }
    }

    if (pickedItemScene != _currentScene)
    {
        _currentCircle = pickedItem;
        _currentScene = pickedItemScene;
        visibilityChanged();
    }
}


void PB_ActionManualInventory::setAttributes(const CT_Scene* scene)
{
    QMap<QString, QString> &attrMap = (QMap<QString, QString>&) _suppAttributes->value(scene);

    PB_ActionManualInventoryAttributesDialog dialog(_paramData, attrMap);
    if (dialog.exec() == QDialog::Accepted)
    {        
        QMutableMapIterator<QString, QString>it(attrMap);
        while (it.hasNext())
        {
            it.next();
            const QString &name = it.key();
            QString &value  = it.value();

            value = dialog.getValueForAttr(name);
        }
    }
}

void PB_ActionManualInventory::visibilityChanged()
{

    document()->removeAllItemDrawable();

    QMapIterator<const CT_Scene*, const CT_Circle*> it(*_availableDbh);
    while (it.hasNext())
    {
        it.next();
        CT_Scene* scene = (CT_Scene*)it.key();
        CT_Circle* circle = (CT_Circle*) it.value();

        updateVisibility(scene, circle);
    }
    document()->redrawGraphics();
}

void PB_ActionManualInventory::updateVisibility(CT_Scene* scene, CT_Circle* circle)
{
    PB_ActionManualInventoryOptions *option = (PB_ActionManualInventoryOptions*)optionAt(0);
    if (scene == _currentScene)
    {
        if (option->isShowActiveCirclesChecked() || circle == _currentCircle)
        {
            document()->addItemDrawable(*circle);

            if (circle == _currentCircle)
            {
                document()->setColor(circle, _currentCircleColor);
            } else {
                document()->setColor(circle, _activeSceneCirclesColor);
            }
        }

        if (option->isShowActiveSceneChecked())
        {
            document()->addItemDrawable(*scene);
        }
    } else {
        if (option->isShowOtherCirclesChecked())
        {
            document()->addItemDrawable(*circle);
            if (_selectedDbh->values().contains(circle))
            {
                document()->setColor(circle, _othersCircleColor);
            } else {
                document()->setColor(circle, _othersScenesCirclesColor);
            }
        }

        if (option->isShowOtherScenesChecked())
        {
            document()->addItemDrawable(*scene);
        }
    }
}



