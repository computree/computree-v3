#include "actions/pb_actionmanualinventory.h"
#include "ct_global/ct_context.h"
#include "views/actions/pb_actionmanualinventoryattributesdialog.h"


#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <limits>
#include <QVector3D>

#include "ct_math/ct_mathpoint.h"

PB_ActionManualInventory::PB_ActionManualInventory(QMap<const CT_Scene*,
                                                   const CT_Circle*> *selectedDbh,
                                                   QMultiMap<const CT_Scene *, const CT_Circle*> *availableDbh,
                                                   QMap<const CT_Scene*, QString> *species,
                                                   QMap<const CT_Scene *, QString> *ids,
                                                   const QStringList &speciesList) : CT_AbstractActionForGraphicsView()
{
    _selectedDbh = selectedDbh;
    _availableDbh = availableDbh;
    _species = species;
    _ids = ids;
    _speciesList = speciesList;
    _currentCircle = NULL;
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

        connect(option, SIGNAL(parametersChanged()), this, SLOT(redraw()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}

void PB_ActionManualInventory::redraw()
{
    document()->redrawGraphics();
}

void PB_ActionManualInventory::updateSpeciesList(const QString &species)
{
    if (!_speciesList.contains(species))
    {
        _speciesList.append(species);
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

            if (option->getMode() == PB_ActionManualInventoryOptions::Mode_dbh)
            {
                _currentCircle = chooseForDbh(e->pos());
            } else {
                _currentCircle = chooseForAttributes(e->pos());
            }

            if (_currentCircle!=NULL && option->shouldAutoCenterCamera())
            {
                graphicsView()->camera()->setCX(_currentCircle->getCenterX());
                graphicsView()->camera()->setCY(_currentCircle->getCenterY());
                graphicsView()->camera()->setCZ(_currentCircle->getCenterZ());
            }
            redraw();
            return true;
        } else if (_buttonsPressed == Qt::RightButton)
        {
            option->toggleMode();
            redraw();
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

    PB_ActionManualInventoryOptions *option = (PB_ActionManualInventoryOptions*)optionAt(0);

    painter.save();

    // Draw Scenes if needed
    if (option->shouldShowScenes())
    {
        QMapIterator<const CT_Scene*, const CT_Circle*> itScenes(*_selectedDbh);
        while (itScenes.hasNext())
        {
            itScenes.next();
            CT_Scene* scene = (CT_Scene*) itScenes.key();

            if (scene != NULL)
            {
                scene->draw(view, painter);
            }
        }
    }

    // Draw Circles
    QList<const CT_Circle*> seletedCircles = _selectedDbh->values();
    QMapIterator<const CT_Scene*, const CT_Circle*> itCircles(*_availableDbh);
    while (itCircles.hasNext())
    {
        itCircles.next();
        const CT_Circle* circle = itCircles.value();

        if (circle != NULL)
        {
            if (circle == _currentCircle) {
                painter.setColor(Qt::red);
            } else if (seletedCircles.contains(circle)) {
                painter.setColor(Qt::darkRed);
            } else {
                painter.setColor(Qt::blue);
            }

            const QVector3D &cc = circle->getCenter();
            const QVector3D &dd = circle->getDirection();

            Eigen::Vector3d center(cc.x(), cc.y(), cc.z());
            Eigen::Vector3d direction(dd.x(), dd.y(), dd.z());

            painter.drawCircle3D(center,direction, circle->getRadius());
        }
    }

    painter.restore();
}

void PB_ActionManualInventory::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    PB_ActionManualInventoryOptions *option = (PB_ActionManualInventoryOptions*)optionAt(0);

    if (option->shouldShowData())
    {
        int add = painter.fontMetrics().height()+2;

        QMapIterator<const CT_Scene*, const CT_Circle*> it(*_selectedDbh);
        while (it.hasNext())
        {
            it.next();
            CT_Scene* scene = (CT_Scene*) it.key();
            CT_Circle* circle = (CT_Circle*) it.value();
            QString id = _ids->value(scene);
            QString species = _species->value(scene);

            QPoint pixel;
            graphicsView()->convert3DPositionToPixel(QVector3D(circle->getCenterX(), circle->getCenterY(), circle->getCenterZ()), pixel);

            painter.setPen(Qt::darkRed);
            int y =  pixel.y() + add + 2;
            painter.drawText(pixel.x(), y, tr("ID = %1").arg(id));
            y += add;
            painter.drawText(pixel.x(), y, tr("Espèce = %1").arg(species));

        }
    }
}

CT_AbstractAction* PB_ActionManualInventory::copy() const
{
    return new PB_ActionManualInventory(_selectedDbh, _availableDbh, _species, _ids, _speciesList);
}

const CT_Circle* PB_ActionManualInventory::chooseForDbh(const QPoint &point)
{
    Eigen::Vector3d origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    const CT_Circle* pickedItem = NULL;
    const CT_Scene*  pickedItemScene = NULL;

    QMapIterator<const CT_Scene*, const CT_Circle*> it(*_availableDbh);
    while (it.hasNext())
    {
        it.next();
        const CT_Scene* scene = it.key();
        const CT_Circle* circle = it.value();

        float dist = CT_MathPoint::distancePointLine(circle->getCenterCoordinate(), direction, origin);

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

    return pickedItem;
}

const CT_Circle* PB_ActionManualInventory::chooseForAttributes(const QPoint &point)
{
    Eigen::Vector3d origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    const CT_Circle* pickedItem = NULL;
    const CT_Scene*  pickedItemScene = NULL;

    QMapIterator<const CT_Scene*, const CT_Circle*> it(*_selectedDbh);
    while (it.hasNext())
    {
        it.next();
        const CT_Scene* scene = it.key();
        const CT_Circle* circle = it.value();

        float dist = CT_MathPoint::distancePointLine(circle->getCenterCoordinate(), direction, origin);

        if (dist < minDist)
        {
            minDist = dist;
            pickedItem = circle;
            pickedItemScene = scene;
        }
    }

    redraw();
    setAttributes(pickedItemScene);

    return pickedItem;
}

void PB_ActionManualInventory::setAttributes(const CT_Scene* scene)
{
    QString currentSpecies = _species->value(scene);
    QString currentId = _ids->value(scene);

    PB_ActionManualInventoryAttributesDialog dialog(_speciesList, currentSpecies, currentId);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newSpecies = dialog.getSpecies();
        _species->insert(scene, newSpecies);
        _ids->insert(scene, dialog.getId());

        updateSpeciesList(newSpecies);
    }

}


