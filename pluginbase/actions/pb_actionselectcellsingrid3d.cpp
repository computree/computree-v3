#include "actions/pb_actionselectcellsingrid3d.h"
#include "ct_itemdrawable/ct_beam.h"
#include "ct_itemdrawable/tools/gridtools/ct_grid3dwootraversalalgorithm.h"
#include "ct_itemdrawable/tools/gridtools/ct_grid3dbeamvisitor_indexlist.h"
#include "ct_global/ct_context.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <QDebug>
#include <QVector3D>


PB_ActionSelectCellsInGrid3D::PB_ActionSelectCellsInGrid3D(const CT_AbstractGrid3D *refGrid, CT_Grid3D<bool> *boolGrid) : CT_AbstractActionForGraphicsView()
{
    _refGrid = refGrid;
    _boolGrid = boolGrid;
    _lastIndex = 0;
    _validLastIndex = false;
    _previewMode = PB_ActionSelectCellsInGrid3DOptions::FREEMOVE;
    _cumulMode = PB_ActionSelectCellsInGrid3DOptions::MAX;
    _refState = false;
    _active2DGrid.resize(_refGrid->xdim() * _refGrid->ydim());
    _mode3D = false;
    _reductionCoef = 0.9;
    _level = _refGrid->zdim() / 2;
    _thickness = 1;
}


QString PB_ActionSelectCellsInGrid3D::uniqueName() const
{
    return "PB_ActionSelectCellsInGrid3D";
}

QString PB_ActionSelectCellsInGrid3D::title() const
{
    return "Select In Grid3D";
}

QString PB_ActionSelectCellsInGrid3D::description() const
{
    return "Sélectionne des cases dans une grille 3D";
}

QIcon PB_ActionSelectCellsInGrid3D::icon() const
{
    return QIcon(":/icons/show_all.png");
}

QString PB_ActionSelectCellsInGrid3D::type() const
{
    return CT_AbstractAction::TYPE_MODIFICATION;
}

void PB_ActionSelectCellsInGrid3D::init()
{
    CT_AbstractActionForGraphicsView::init();

    m_status = 0;

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionSelectCellsInGrid3DOptions *option = new PB_ActionSelectCellsInGrid3DOptions(this, _refGrid->zdim() - 1);
        connect(option, SIGNAL(levelChanged(int)), this, SLOT(updateLevel(int)));
        connect(option, SIGNAL(thicknessChanged(int)), this, SLOT(updateThickness(int)));
        connect(option, SIGNAL(showModeChanged()), this, SLOT(updateDrawing()));
        connect(option, SIGNAL(gridModeChanged()), this, SLOT(updateDrawing()));
        connect(option, SIGNAL(relativeRepresentationChanged()), this, SLOT(updateDrawing()));
        connect(option, SIGNAL(cumulModeChanged(PB_ActionSelectCellsInGrid3DOptions::CumulateMode)), this, SLOT(updateCumulMode(PB_ActionSelectCellsInGrid3DOptions::CumulateMode)));
        connect(option, SIGNAL(askForUpwardsCopy()), this, SLOT(copyUpwards()));
        connect(option, SIGNAL(askForFullCopy()), this, SLOT(copyFull()));
        connect(option, SIGNAL(selectionModeChanged(PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode)), this, SLOT(updateSelectionMode(PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode)));
        connect(option, SIGNAL(mode2D3DChanged(bool)), this, SLOT(change2D3DMode(bool)));
        connect(option, SIGNAL(coefChanged(double)), this, SLOT(changeCoef(double)));
        connect(option, SIGNAL(gridColorChanged(QColor)), this, SLOT(changeGridColor(QColor)));

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        updateLevel(option->level());
        updateThickness(option->thickness());
        changeGridColor(option->gridColor());
        document()->redrawGraphics();
    }
}


void PB_ActionSelectCellsInGrid3D::initActiveGridForLevel()
{

    size_t minLevel = _level;
    size_t maxLevel = _level + _thickness - 1;

    float denom = (maxLevel - minLevel + 1);

    _activeMin = 1;
    _activeMax = 0;

    for (size_t xx = 0 ; xx < _refGrid->xdim() ; xx++)
    {
        for (size_t yy = 0 ; yy < _refGrid->ydim() ; yy++)
        {
            size_t index2D = yy*_refGrid->xdim() + xx;

            size_t index;
            _refGrid->index(xx, yy, minLevel, index);

            float value = 0;

            for (size_t zz = minLevel ; zz <= maxLevel ; zz++)
            {
                _refGrid->index(xx, yy, zz, index);
                float new_value = _refGrid->ratioValueAtIndex(index);

                if (_cumulMode == PB_ActionSelectCellsInGrid3DOptions::MEAN || _cumulMode == PB_ActionSelectCellsInGrid3DOptions::SUM)
                {
                    value += new_value;
                } else if (_cumulMode == PB_ActionSelectCellsInGrid3DOptions::MAX && new_value > value)
                {
                    value = new_value;
                } else if (_cumulMode == PB_ActionSelectCellsInGrid3DOptions::NUMBER && new_value > 0)
                {
                    value += 1;
                }

            }
            if (_cumulMode == PB_ActionSelectCellsInGrid3DOptions::MEAN || _cumulMode == PB_ActionSelectCellsInGrid3DOptions::NUMBER) {value = value / denom;}
            if (value > 1) {value = 1;}

            _active2DGrid.at(index2D) = value;
            if (value > _activeMax) {_activeMax = value;}
            if (value < _activeMin && value > 0) {_activeMin = value;}
        }
    }
}

bool PB_ActionSelectCellsInGrid3D::mousePressEvent(QMouseEvent *e)
{
    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);
    PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = option->selectionMode();

    if (mode != PB_ActionSelectCellsInGrid3DOptions::FREEMOVE && (e->buttons() & Qt::LeftButton))
    {
        if (_mode3D)
        {
            size_t index;
            if (getIndexInGridForMousePosition(e, index))
            {
                if (mode == PB_ActionSelectCellsInGrid3DOptions::CHANGECENTERCELL && (e->buttons() & Qt::LeftButton))
                {
                    if (m_status == 2)
                    {
                        _candidates.clear();
                        _candidates.append(index);
                        m_status = 3;
                        document()->redrawGraphics();
                        return true;
                    }
                } else if (mode == PB_ActionSelectCellsInGrid3DOptions::EXTENDS && (e->buttons() & Qt::LeftButton))
                {
                    if (m_status != 1)
                    {
                        _candidates.clear();
                        _candidates = computeExtends3D(index);
                        if (_candidates.size() > 0) {m_status = 1;}
                        document()->redrawGraphics();
                        return true;
                    }

                } else if (mode == PB_ActionSelectCellsInGrid3DOptions::COLONIZE && (e->buttons() & Qt::LeftButton))
                {
                    if (m_status != 1)
                    {
                        _candidates.clear();
                        _candidates = computeColonize3D(index);
                        if (_candidates.size() > 0) {m_status = 1;}
                        document()->redrawGraphics();
                        return true;
                    }

                } else
                {
                    updateSelection(index);
                    document()->redrawGraphics();
                    return true;
                }
            }
        } else
        {
            float x, y, z;
            if (getCoordsInActiveLevelForMousePosition(e, x, y, z))
            {

                size_t colx, liny;
                _refGrid->colX(x, colx);
                _refGrid->linY(y, liny);

                if (m_status == 2)
                {
                    size_t index;
                    if (_refGrid->index(colx, liny, _level, index))
                    {
                        _candidates.clear();
                        _candidates.append(index);
                        m_status = 3;
                        document()->redrawGraphics();
                        return true;
                    }

                } else if (mode == PB_ActionSelectCellsInGrid3DOptions::EXTENDS && (e->buttons() & Qt::LeftButton))
                {
                    if (m_status != 1)
                    {
                        _candidates.clear();
                        _candidates = computeExtends(colx, liny);
                        if (_candidates.size() > 0) {m_status = 1;}
                        document()->redrawGraphics();
                        return true;
                    }

                } else if (mode == PB_ActionSelectCellsInGrid3DOptions::COLONIZE && (e->buttons() & Qt::LeftButton))
                {
                    if (m_status != 1)
                    {
                        _candidates.clear();
                        _candidates = computeColonize(colx, liny);
                        if (_candidates.size() > 0) {m_status = 1;}
                        document()->redrawGraphics();
                        return true;
                    }

                } else
                {
                    updateSelection(x, y, z);
                    document()->redrawGraphics();
                    return true;
                }
            }
        }
    }

    return false;
}

bool PB_ActionSelectCellsInGrid3D::mouseMoveEvent(QMouseEvent *e)
{
    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);
    PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = option->selectionMode();

    if (!_mode3D && (mode == PB_ActionSelectCellsInGrid3DOptions::ADD || mode == PB_ActionSelectCellsInGrid3DOptions::REMOVE) && (e->buttons() & Qt::LeftButton))
    {
        float x, y, z;
        if (getCoordsInActiveLevelForMousePosition(e, x, y, z))
        {
            updateSelection(x, y, z);
            document()->redrawGraphics();
        }
        return true;
    }

    return false;
}

bool PB_ActionSelectCellsInGrid3D::getCoordsInActiveLevelForMousePosition(const QMouseEvent *e, float &x, float &y, float &z)
{
    QVector3D origin, direction;
    GraphicsViewInterface *view = graphicsView();
    view->convertClickToLine(e->pos(), origin, direction);

    z = _refGrid->getCellCenterZ(_level);

    if (direction.z() ==0) {return false;}

    float coef = (z - _refGrid->resolution()/2.0 - origin.z())/direction.z();

    x = origin.x() + coef*direction.x();
    y = origin.y() + coef*direction.y();

    size_t index;
    return _refGrid->indexAtXYZ(x, y, z, index);
}

bool PB_ActionSelectCellsInGrid3D::getIndexInGridForMousePosition(const QMouseEvent *e, size_t &index)
{
    QVector3D origin, direction, nearP, farP, bot, top;
    GraphicsViewInterface *view = graphicsView();
    view->convertClickToLine(e->pos(), origin, direction);

    CT_Beam beam(NULL, NULL, origin, direction);

    _refGrid->getBoundingBox(bot, top);

    if (beam.intersect(bot, top, nearP, farP))
    {
        QList<CT_AbstractGrid3DBeamVisitor*> liste;
        liste.append(new CT_Grid3DBeamVisitor_IndexList());
        CT_Grid3DWooTraversalAlgorithm algo(_refGrid, false, liste);

        algo.compute(beam);

        QList<size_t> indicesList = ((CT_Grid3DBeamVisitor_IndexList*) liste.first())->getIndices();

        for (int i = 0 ; i < indicesList.size() ; i++)
        {
            index = indicesList.at(i);
            if (_refGrid->ratioValueAtIndex(index) > 0 || _boolGrid->valueAtIndex(index))
            {                
                return true;
            }
        }
    }

    return false;
}


void PB_ActionSelectCellsInGrid3D::updateSelection(const float x, const float y, const float z)
{
    size_t index;
    _refGrid->indexAtXYZ(x, y, z, index);
    updateSelection(index);
}

void PB_ActionSelectCellsInGrid3D::updateSelection(const size_t index)
{
    PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = ((PB_ActionSelectCellsInGrid3DOptions*)optionAt(0))->selectionMode();

    if (index != _lastIndex || !_validLastIndex)
    {
        if (mode == PB_ActionSelectCellsInGrid3DOptions::ADD)
        {
            _boolGrid->setValueAtIndex(index, true);
        }
        else if (mode == PB_ActionSelectCellsInGrid3DOptions::REMOVE)
        {
            _boolGrid->setValueAtIndex(index, false);
        }
        _lastIndex = index;
        _validLastIndex = true;
    }
}


bool PB_ActionSelectCellsInGrid3D::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PB_ActionSelectCellsInGrid3D::wheelEvent(QWheelEvent *e)
{
    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

    if (!_mode3D)
    {
        if (e->modifiers()  & Qt::ControlModifier)
        {
            if (e->delta()>0)
            {
                option->modifyLevel(+1);
            } else {
                option->modifyLevel(-1);
            }
            return true;
        } else if (e->modifiers()  & Qt::ShiftModifier)
        {
            if (e->delta()>0)
            {
                option->modifyThickness(+1);
            } else {
                option->modifyThickness(-1);
            }
            return true;
        }
    }

    return false;
}

bool PB_ActionSelectCellsInGrid3D::keyPressEvent(QKeyEvent *e)
{
    // Key A : pass in "Add" mode
    if((e->key() == Qt::Key_A)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::ADD);
        return true;
    }
    // Key R : pass in "Remove" mode
    else if((e->key() == Qt::Key_R)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::REMOVE);
        return true;
    }
    // Key F : pass in "Free move" mode
    else if((e->key() == Qt::Key_F)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::FREEMOVE);
        return true;
    }
    // Key E : pass in "Extends" mode
    else if((e->key() == Qt::Key_E)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::EXTENDS);
        return true;
    }
    // Key Z : pass in "Colonize" mode
    else if((e->key() == Qt::Key_Z)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::COLONIZE);
        return true;
    }
    // Key D : Switch between 2D / 3D modes
    else if((e->key() == Qt::Key_D)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

        option->modify2D3DMode();

        return true;
    }
    // Key Y after extend preselection (validate)
    else if((e->key() == Qt::Key_Y)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);
        PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = option->selectionMode();

        if (mode == PB_ActionSelectCellsInGrid3DOptions::EXTENDS && m_status == 1)
        {
            m_status = 0;
            int size = _candidates.size();
            for (int i = 0 ; i < size ; i++)
            {
                _boolGrid->setValueAtIndex(_candidates.at(i), !_refState);
            }
            _candidates.clear();
            document()->redrawGraphics();
            PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Extension validée"));
            return true;
        } else if (mode == PB_ActionSelectCellsInGrid3DOptions::COLONIZE && m_status == 1)
        {
            m_status = 0;
            int size = _candidates.size();
            for (int i = 0 ; i < size ; i++)
            {
                _boolGrid->setValueAtIndex(_candidates.at(i), true);
            }
            _candidates.clear();
            document()->redrawGraphics();
            PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Colonization validée"));
            return true;
        } else if (mode == PB_ActionSelectCellsInGrid3DOptions::CHANGECENTERCELL && m_status == 3)
        {
            if (_candidates.size()>0)
            {
                size_t colx, liny, levz;
                _refGrid->indexToGrid(_candidates.first(), colx, liny, levz);
                double cx = _refGrid->getCellCenterX(colx);
                double cy = _refGrid->getCellCenterY(liny);
                double cz = _refGrid->getCellCenterZ(levz);

                CameraInterface* camera = graphicsView()->camera();
                camera->setCX(cx);
                camera->setCY(cy);
                camera->setCZ(cz);

                QString baseStr = tr("Caméra centrée en");
                PS_LOG->addMessage(LogInterface::info, LogInterface::action, QString("%1 x=%2, y=%3, z=%4").arg(baseStr).arg(cx).arg(cy).arg(cz));
            }
            m_status = 0;
            _candidates.clear();
            option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::FREEMOVE);
            document()->redrawGraphics();
            return true;
        }
        return true;
    }
    // Key N after extend preselection (cancel)
    else if((e->key() == Qt::Key_N)
            && !e->isAutoRepeat())
    {
        PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);
        PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = option->selectionMode();

        if ((mode == PB_ActionSelectCellsInGrid3DOptions::EXTENDS) ||
                (mode == PB_ActionSelectCellsInGrid3DOptions::COLONIZE && m_status == 1) ||
                (mode == PB_ActionSelectCellsInGrid3DOptions::CHANGECENTERCELL && m_status == 3))
        {
            m_status = 0;
            option->setSelectionMode(PB_ActionSelectCellsInGrid3DOptions::FREEMOVE);
            _candidates.clear();
            document()->redrawGraphics();
            PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Opération annulée"));
            return true;
        }
        return true;
    }
    return false;
}

bool PB_ActionSelectCellsInGrid3D::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PB_ActionSelectCellsInGrid3D::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    if (_mode3D)
    {
        draw3D(view, painter);
    } else {
        draw2D(view, painter);
    }
}

void PB_ActionSelectCellsInGrid3D::draw2D(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

    painter.save();

    const int EMPTY_LEVEL = 60;
    const int MIN_LEVEL = 90;

    float xmin, ymin;
    float res = _refGrid->resolution();
    float demiRes = res / 2.0;
    float lengthX = res*_refGrid->xdim();
    float lengthY = res*_refGrid->ydim();

    float zmin = _refGrid->getCellCenterZ(_level) - demiRes;
    float zmax = _refGrid->getCellCenterZ(_level + _thickness - 1) + demiRes;

    for (size_t xx = 0 ; xx < _refGrid->xdim() ; xx++)
    {
        for (size_t yy = 0 ; yy < _refGrid->ydim() ; yy++)
        {
            size_t index;
            _refGrid->index(xx, yy, _level, index);
            size_t index2D = yy*_refGrid->xdim() + xx;

            float ratio = _active2DGrid.at(index2D);
            //float ratio = _refGrid->ratioValueAtIndex(index);
            bool selected = _boolGrid->valueAtIndex(index);

            if (selected || !option->showSeletedOnly() || (m_status == 3 && _candidates.contains(index)))
            {
                if (m_status == 3 && _candidates.contains(index))
                {
                    painter.setColor(QColor(0, 255, 0));
                } else if (ratio < 0 || ratio > 1) // NA values
                {
                    if (selected)
                    {
                        painter.setColor(QColor(125, 0, 125));
                    } else {
                        painter.setColor(QColor(0, 0, 125));
                    }
                    // Valid values
                } else {
                    int colorLevel = MIN_LEVEL + ratio*(255-MIN_LEVEL);
                    if (option->relativeRepresentation())
                    {
                        colorLevel = MIN_LEVEL + ((ratio - _activeMin)/(_activeMax - _activeMin))*(255-MIN_LEVEL);
                    }
                    if (selected)
                    {
                        if (_candidates.contains(index))
                        {
                            // Cells to remove from selection => blue
                            if (ratio == 0)
                            {
                                painter.setColor(QColor(0, 0, EMPTY_LEVEL));
                            } else {
                                painter.setColor(QColor(0, 0, colorLevel));
                            }
                        } else {
                            // Cells to keep in selection => red
                            if (ratio == 0)
                            {
                                painter.setColor(QColor(EMPTY_LEVEL, 0, 0));
                            } else {
                                painter.setColor(QColor(colorLevel, 0, 0));
                            }
                        }
                    } else {
                        if (_candidates.contains(index))
                        {
                            // Cells to add to selection => yellow
                            if (ratio == 0)
                            {
                                painter.setColor(QColor(EMPTY_LEVEL, EMPTY_LEVEL, 0));
                            } else {
                                painter.setColor(QColor(colorLevel, colorLevel, 0));
                            }
                        } else {
                            // Not selected empty Cells => black
                            if (ratio == 0)
                            {
                                painter.setColor(QColor(0, 0, 0));
                            } else {
                                // Not selected not empty cells => grey level
                                painter.setColor(QColor(colorLevel, colorLevel, colorLevel));
                            }
                        }
                    }
                }

                xmin = _refGrid->getCellCenterX(xx) - demiRes;
                ymin = _refGrid->getCellCenterY(yy) - demiRes;

                painter.fillRectXY(QRectF(xmin, ymin, res, res), zmin);


                if (option->isGridOn())
                {
                    painter.setColor(QColor(255, 255, 255));
                    painter.drawRectXY(QRectF(xmin, ymin, res, res), zmin + 0.0001);
                }
            }
        }
    }

    painter.setColor(QColor(255, 255, 255));
    painter.drawRectXY(QRectF(_refGrid->minX(), _refGrid->minY(), lengthX, lengthY), zmax);

    painter.drawRectXZ(QRectF(_refGrid->minX(), zmin, lengthX, _thickness*res), _refGrid->minY());
    painter.drawRectXZ(QRectF(_refGrid->minX(), zmin, lengthX, _thickness*res), _refGrid->maxY());
    painter.drawRectYZ(QRectF(_refGrid->minY(), zmin, lengthY, _thickness*res), _refGrid->minX());
    painter.drawRectYZ(QRectF(_refGrid->minY(), zmin, lengthY, _thickness*res), _refGrid->maxX());


    painter.restore();
}

void PB_ActionSelectCellsInGrid3D::draw3D(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    const int EMPTY_LEVEL = 60;

    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

    painter.save();

    bool wireMode = true;
    GLenum drawingMode;
    if (wireMode) {drawingMode = GL_LINE;}
    else          {drawingMode = GL_FILL;}

    float demiRes = _reductionCoef*_refGrid->resolution() / 2.0;

    size_t xdim = _refGrid->xdim();
    size_t ydim = _refGrid->ydim();
    size_t zdim = _refGrid->zdim();

    float xmin, ymin, zmin, xmax, ymax, zmax;

    // For each voxel of the grid
    for (size_t xx = 0 ; xx < xdim ; xx++)
    {
        for (size_t yy = 0 ; yy < ydim ; yy++)
        {
            for (size_t zz = 0 ; zz < zdim; zz++)
            {
                size_t index;
                if (_refGrid->index(xx, yy, zz, index))
                {
                    float data = _refGrid->ratioValueAtIndex(index);
                    bool selected = _boolGrid->valueAtIndex(index);

                    if (data > 0 || selected || (m_status == 3 && _candidates.contains(index)))
                    {
                        if (m_status == 3 && _candidates.contains(index))
                        {
                            painter.setColor(QColor(0, 255, 0));
                        } else if (selected)
                        {
                            if (_candidates.contains(index))
                            {
                                // Cells to remove from selection => blue
                                if (data == 0)
                                {
                                    painter.setColor(QColor(0, 0, EMPTY_LEVEL));
                                } else {
                                    painter.setColor(QColor(0, 0, 255));
                                }
                            } else {
                                // Cells to keep in selection => red
                                if (data == 0)
                                {
                                    painter.setColor(QColor(EMPTY_LEVEL, 0, 0));
                                } else {
                                    painter.setColor(QColor(255, 0, 0));
                                }
                            }
                        } else {
                            if (_candidates.contains(index))
                            {
                                // Cells to add to selection => yellow
                                painter.setColor(QColor(255, 255, 0));
                            } else {
                                // Not selected not empty cells => grey level
                                painter.setColor(_gridColor);
                            }
                        }

                        if (selected || !option->showSeletedOnly())
                        {
                            xmin = _refGrid->getCellCenterX(xx) - demiRes;
                            ymin = _refGrid->getCellCenterY(yy) - demiRes;
                            zmin = _refGrid->getCellCenterZ(zz) - demiRes;
                            xmax = _refGrid->getCellCenterX(xx) + demiRes;
                            ymax = _refGrid->getCellCenterY(yy) + demiRes;
                            zmax = _refGrid->getCellCenterZ(zz) + demiRes;

                            painter.drawCube( xmin, ymin, zmin, xmax, ymax, zmax, GL_FRONT_AND_BACK, drawingMode);

                        }
                    }
                } else {
                    qDebug() << "Problème d'index (PB_ActionSelectCellsInGrid3D)";
                }
            }
        }
    }
    painter.restore();
}

void PB_ActionSelectCellsInGrid3D::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);
    PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode = option->selectionMode();

    if (m_status == 1)
    {
        painter.save();
        painter.setPen(QColor(255,255,0));
        painter.drawText(2, painter.fontMetrics().height()+2, tr("Press \"y\" to apply, or \"n\" to cancel !"));
        painter.restore();
    } else if (mode == PB_ActionSelectCellsInGrid3DOptions::CHANGECENTERCELL)
    {
        painter.save();
        painter.setPen(QColor(0,255,0));
        if (m_status == 2)
        {
            painter.drawText(2, painter.fontMetrics().height()+2, tr("Select a cell as new view center !"));
        } else if (m_status == 3)
        {
            painter.drawText(2, painter.fontMetrics().height()+2, tr("Press \"y\" to apply, or \"n\" to cancel !"));
        }
        painter.restore();
    }
}

CT_AbstractAction *PB_ActionSelectCellsInGrid3D::copy() const
{
    return new PB_ActionSelectCellsInGrid3D(_refGrid, _boolGrid);
}

void PB_ActionSelectCellsInGrid3D::updateLevel(int level)
{
    _level = level;
    initActiveGridForLevel();
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::updateThickness(int thickness)
{
    _thickness = thickness;
    initActiveGridForLevel();
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::updateDrawing()
{
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::updateCumulMode(PB_ActionSelectCellsInGrid3DOptions::CumulateMode mode)
{
    _cumulMode = mode;
    initActiveGridForLevel();
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Changement de mode de cumul pris en compte"));
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::updateSelectionMode(PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode)
{
    if (mode != _previewMode)
    {
        if (mode == PB_ActionSelectCellsInGrid3DOptions::CHANGECENTERCELL)
        {
            m_status = 2;
        } else {
            m_status = 0;
        }
        _validLastIndex = false;
        _lastIndex = 0;
        _candidates.clear();
        _previewMode = mode;
    }

    document()->redrawGraphics();
}


void PB_ActionSelectCellsInGrid3D::copyUpwards()
{
    copyLevel(_level, _level + 1, _level + _thickness - 1);
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Niveau en cours propagé sur l'épaisseur (thickness)"));

}

void PB_ActionSelectCellsInGrid3D::copyFull()
{
    copyLevel(_level, 0, _boolGrid->zdim());
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Niveau en cours propagé sur TOUTE la grille"));
}

void PB_ActionSelectCellsInGrid3D::change2D3DMode(bool mode3D)
{
    _mode3D = mode3D;
    document()->redrawGraphics();
}


void PB_ActionSelectCellsInGrid3D::changeCoef(double coef)
{
    _reductionCoef = coef;
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::changeGridColor(QColor color)
{
    _gridColor = color;
    document()->redrawGraphics();
}

void PB_ActionSelectCellsInGrid3D::copyLevel(size_t refLevel, size_t first, size_t last)
{
    for (size_t xx = 0; xx < _boolGrid->xdim() ; xx++)
    {
        for (size_t yy = 0; yy < _boolGrid->ydim() ; yy++)
        {
            size_t index;
            _boolGrid->index(xx, yy, refLevel, index);
            bool value = _boolGrid->valueAtIndex(index);

            for (size_t zz = first ; zz <= last ; zz++)
            {
                if (zz != refLevel)
                {
                    size_t indexz;
                    _boolGrid->index(xx, yy, zz, indexz);
                    _boolGrid->setValueAtIndex(indexz, value);
                }
            }
        }
    }
}

QList<size_t> PB_ActionSelectCellsInGrid3D::computeExtends(size_t originColumn, size_t originRow)
{
    QList<size_t> result;
    size_t index;

    if (!_refGrid->index(originColumn, originRow, _level, index)) {return result;}

    _refState = _boolGrid->valueAtIndex(index);
    result.append(index);

    int i = 0;
    while (i < result.size())
    {
        size_t current_col, current_row, lev;
        _refGrid->indexToGrid(result.at(i), current_col, current_row, lev);

        appendIfCoherentRefState(result, current_col - 1, current_row, _level);
        appendIfCoherentRefState(result, current_col, current_row - 1, _level);
        appendIfCoherentRefState(result, current_col + 1, current_row, _level);
        appendIfCoherentRefState(result, current_col, current_row + 1, _level);

        ++i;
    }

    return result;
}

void PB_ActionSelectCellsInGrid3D::appendIfCoherentRefState(QList<size_t> &result, size_t col, size_t lin, size_t lev)
{
    size_t index;
    if (_refGrid->index(col, lin, lev, index))
    {
        if (_boolGrid->valueAtIndex(index) == _refState && !result.contains(index))
        {
            result.append(index);
        }
    }
}

QList<size_t> PB_ActionSelectCellsInGrid3D::computeColonize(size_t originColumn, size_t originRow)
{    
    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

    QList<size_t> result;
    size_t index;

    if (!_refGrid->index(originColumn, originRow, _level, index))
    {
        return result;
    }

    size_t baseIndex = originRow*_refGrid->xdim() + originColumn;
    if (_active2DGrid.at(baseIndex) > 0) {result.append(index);}

    int i = 0;
    while (i < result.size())
    {
        size_t current_col, current_row, lev;
        _refGrid->indexToGrid(result.at(i), current_col, current_row, lev);

        if (option->isXmColonizationAllowed()) {appendIfNotNulValue2D(result, current_col - 1, current_row);}
        if (option->isYmColonizationAllowed()) {appendIfNotNulValue2D(result, current_col, current_row - 1);}
        if (option->isXpColonizationAllowed()) {appendIfNotNulValue2D(result, current_col + 1, current_row);}
        if (option->isYpColonizationAllowed()) {appendIfNotNulValue2D(result, current_col, current_row + 1);}

        ++i;
    }

    return result;
}

void PB_ActionSelectCellsInGrid3D::appendIfNotNulValue2D(QList<size_t> &result, size_t col, size_t lin)
{
    size_t index;
    if (_refGrid->index(col, lin, _level, index))
    {
        size_t index2D = lin*_refGrid->xdim() + col;
        if (_active2DGrid.at(index2D) > 0 && !result.contains(index))
        {
            result.append(index);
        }
    }
}

QList<size_t> PB_ActionSelectCellsInGrid3D::computeExtends3D(size_t index)
{
    QList<size_t> result;
    size_t originColumn, originRow, originLevel;

    if (!_boolGrid->indexToGrid(index, originColumn, originRow, originLevel)) {return result;}

    _refState = _boolGrid->valueAtIndex(index);
    if (!_refState) {return result;}

    appendIfCoherentRefState(result, originColumn, originRow, originLevel);

    int i = 0;
    while (i < result.size())
    {
        size_t current_col, current_row, current_lev;

        _refGrid->indexToGrid(result.at(i), current_col, current_row, current_lev);

        appendIfCoherentRefState(result, current_col - 1, current_row, current_lev);
        appendIfCoherentRefState(result, current_col, current_row - 1, current_lev);
        appendIfCoherentRefState(result, current_col, current_row, current_lev - 1);
        appendIfCoherentRefState(result, current_col + 1, current_row, current_lev);
        appendIfCoherentRefState(result, current_col, current_row + 1, current_lev);
        appendIfCoherentRefState(result, current_col, current_row, current_lev + 1);

        ++i;
    }

    return result;
}

QList<size_t> PB_ActionSelectCellsInGrid3D::computeColonize3D(size_t index)
{
    PB_ActionSelectCellsInGrid3DOptions *option = (PB_ActionSelectCellsInGrid3DOptions*)optionAt(0);

    QList<size_t> result;
    size_t originColumn, originRow, originLevel;

    if (!_refGrid->indexToGrid(index, originColumn, originRow, originLevel)) {return result;}

    appendIfNotNulValue3D(result, originColumn, originRow, originLevel);

    int i = 0;
    while (i < result.size())
    {
        size_t current_col, current_row, current_lev;

        _refGrid->indexToGrid(result.at(i), current_col, current_row, current_lev);

        if (option->isXmColonizationAllowed()) {appendIfNotNulValue3D(result, current_col - 1, current_row, current_lev);}
        if (option->isYmColonizationAllowed()) {appendIfNotNulValue3D(result, current_col, current_row - 1, current_lev);}
        if (option->isZmColonizationAllowed()) {appendIfNotNulValue3D(result, current_col, current_row, current_lev - 1);}
        if (option->isXpColonizationAllowed()) {appendIfNotNulValue3D(result, current_col + 1, current_row, current_lev);}
        if (option->isYpColonizationAllowed()) {appendIfNotNulValue3D(result, current_col, current_row + 1, current_lev);}
        if (option->isZpColonizationAllowed()) {appendIfNotNulValue3D(result, current_col, current_row, current_lev + 1);}

        ++i;
    }

    return result;
}

void PB_ActionSelectCellsInGrid3D::appendIfNotNulValue3D(QList<size_t> &result, size_t col, size_t lin, size_t lev)
{
    size_t index;
    if (_refGrid->index(col, lin, lev, index))
    {
        if (_refGrid->ratioValueAtIndex(index) > 0 && !result.contains(index))
        {
            result.append(index);
        }
    }
}


