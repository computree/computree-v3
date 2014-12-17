#include "actions/pb_actionvalidateinventory.h"
#include "ct_global/ct_context.h"
#include "views/actions/PB_ActionValidateInventoryattributesdialog.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <limits>
#include <QVector3D>

#include "ct_math/ct_mathpoint.h"

PB_ActionValidateInventory::PB_ActionValidateInventory(QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> *selectedItem,
                                                   QMultiMap<const CT_StandardItemGroup *, const CT_AbstractSingularItemDrawable*> *availableItem,
                                                   QMap<const CT_StandardItemGroup*, QString> *species,
                                                   QMap<const CT_StandardItemGroup *, QString> *ids,
                                                   const QStringList &speciesList) : CT_AbstractActionForGraphicsView()
{
    _selectedItem = selectedItem;
    _initiallySelectedItem = *selectedItem;
    _availableItem = availableItem;
    _species = species;
    _ids = ids;
    _speciesList = speciesList;
    _currentItem = NULL;
}

PB_ActionValidateInventory::~PB_ActionValidateInventory()
{
}

QString PB_ActionValidateInventory::uniqueName() const
{
    return "PB_ActionValidateInventory";
}

QString PB_ActionValidateInventory::title() const
{
    return tr("Validation Inventaire");
}

QString PB_ActionValidateInventory::description() const
{
    return tr("Validation Inventaire");
}

QIcon PB_ActionValidateInventory::icon() const
{
    return QIcon(":/icons/cursor.png");
}

QString PB_ActionValidateInventory::type() const
{
    return CT_AbstractAction::TYPE_MEASURE;
}

void PB_ActionValidateInventory::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionValidateInventoryOptions *option = new PB_ActionValidateInventoryOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        connect(option, SIGNAL(parametersChanged()), this, SLOT(redraw()));
        connect(option, SIGNAL(askForCurrentItemReset()), this, SLOT(resetCurrentItem()));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        document()->redrawGraphics();
    }
}

void PB_ActionValidateInventory::redraw()
{
    document()->redrawGraphics();
}

void PB_ActionValidateInventory::updateSpeciesList(const QString &species)
{
    if (!_speciesList.contains(species))
    {
        _speciesList.append(species);
    }
}

void PB_ActionValidateInventory::resetCurrentItem()
{
    if (_currentItem != NULL)

    {
        const CT_StandardItemGroup* group = _availableItem->key(_currentItem);
        _currentItem = _initiallySelectedItem.value(group);
        _selectedItem->insert(group, _currentItem);
        redraw();
    }
}

bool PB_ActionValidateInventory::mousePressEvent(QMouseEvent *e)
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

bool PB_ActionValidateInventory::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionValidateInventory::mouseReleaseEvent(QMouseEvent *e)
{   
    PB_ActionValidateInventoryOptions *option = (PB_ActionValidateInventoryOptions*)optionAt(0);

    QPoint point = e->pos() - _oldPos;

    if (point.manhattanLength() <= 3)
    {
        if (_buttonsPressed == Qt::LeftButton)
        {

            if (option->getMode() == PB_ActionValidateInventoryOptions::Mode_Item)
            {
                _currentItem = chooseForItem(e->pos());
            } else {
                _currentItem = chooseForAttributes(e->pos());
            }

            if (_currentItem!=NULL && option->shouldAutoCenterCamera())
            {
                graphicsView()->camera()->setCX(_currentItem->getCenterX());
                graphicsView()->camera()->setCY(_currentItem->getCenterY());
                graphicsView()->camera()->setCZ(_currentItem->getCenterZ());
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

bool PB_ActionValidateInventory::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return false;

}

bool PB_ActionValidateInventory::keyPressEvent(QKeyEvent *e)
{   
    Q_UNUSED(e);
    return false;

}

bool PB_ActionValidateInventory::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionValidateInventory::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    PB_ActionValidateInventoryOptions *option = (PB_ActionValidateInventoryOptions*)optionAt(0);

    painter.save();

    // Draw Circles
    QList<const CT_AbstractSingularItemDrawable*> seletedCircles = _selectedItem->values();
    QMapIterator<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> itItems(*_availableItem);
    while (itItems.hasNext())
    {
        itItems.next();
        CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itItems.value();

        if (item != NULL)
        {
            bool selected = false;
            if (item == _currentItem) {
                painter.setColor(Qt::red);
                selected = true;
            } else if (seletedCircles.contains(item)) {
                painter.setColor(Qt::darkRed);
                selected = true;
            } else {
                painter.setColor(Qt::blue);
            }
            if (selected || option->shouldShowCandidateItems())
            {
                item->draw(view, painter);
            }
        }
    }

    painter.restore();
}

void PB_ActionValidateInventory::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    PB_ActionValidateInventoryOptions *option = (PB_ActionValidateInventoryOptions*)optionAt(0);

    if (option->shouldShowData())
    {
        int add = painter.fontMetrics().height()+2;

        QMapIterator<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> it(*_selectedItem);
        while (it.hasNext())
        {
            it.next();
            CT_StandardItemGroup* scene = (CT_StandardItemGroup*) it.key();
            CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) it.value();
            QString id = _ids->value(scene);
            QString species = _species->value(scene);

            QPoint pixel;
            graphicsView()->convert3DPositionToPixel(QVector3D(item->getCenterX(), item->getCenterY(), item->getCenterZ()), pixel);

            painter.setPen(Qt::darkRed);
            int y =  pixel.y() + add + 2;
            painter.drawText(pixel.x(), y, tr("ID = %1").arg(id));
            y += add;
            painter.drawText(pixel.x(), y, tr("Espèce = %1").arg(species));

        }
    }
}

CT_AbstractAction* PB_ActionValidateInventory::copy() const
{
    return new PB_ActionValidateInventory(_selectedItem, _availableItem, _species, _ids, _speciesList);
}

const CT_AbstractSingularItemDrawable* PB_ActionValidateInventory::chooseForItem(const QPoint &point)
{
    QVector3D origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    const CT_AbstractSingularItemDrawable* pickedItem = NULL;
    const CT_StandardItemGroup*  pickedItemScene = NULL;

    QMapIterator<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> it(*_availableItem);
    while (it.hasNext())
    {
        it.next();
        const CT_StandardItemGroup* scene = it.key();
        const CT_AbstractSingularItemDrawable* item = it.value();

        QVector3D point(item->getCenterX(), item->getCenterY(), item->getCenterZ());
        float dist = CT_MathPoint::distancePointLine<QVector3D>(point, direction, origin);

        if (dist < minDist)
        {
            minDist = dist;
            pickedItem = item;
            pickedItemScene = scene;
        }
    }

    if (pickedItem != NULL && pickedItemScene != NULL)
    {
        _selectedItem->insert(pickedItemScene, pickedItem);
    }

    return pickedItem;
}

const CT_AbstractSingularItemDrawable* PB_ActionValidateInventory::chooseForAttributes(const QPoint &point)
{
    QVector3D origin, direction;
    graphicsView()->convertClickToLine(point, origin, direction);

    float minDist = std::numeric_limits<float>::max();
    const CT_AbstractSingularItemDrawable* pickedItem = NULL;
    const CT_StandardItemGroup*  pickedItemScene = NULL;

    QMapIterator<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> it(*_selectedItem);
    while (it.hasNext())
    {
        it.next();
        const CT_StandardItemGroup* scene = it.key();
        const CT_AbstractSingularItemDrawable* circle = it.value();

        QVector3D point(circle->getCenterX(), circle->getCenterY(), circle->getCenterZ());
        float dist = CT_MathPoint::distancePointLine<QVector3D>(point, direction, origin);

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

void PB_ActionValidateInventory::setAttributes(const CT_StandardItemGroup* scene)
{
    QString currentSpecies = _species->value(scene);
    QString currentId = _ids->value(scene);

    PB_ActionValidateInventoryAttributesDialog dialog(_speciesList, currentSpecies, currentId);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newSpecies = dialog.getSpecies();
        _species->insert(scene, newSpecies);
        _ids->insert(scene, dialog.getId());

        updateSpeciesList(newSpecies);
    }

}


