#include "pb_actionmodifyclustersgroups.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <math.h>

#include "views/actions/pb_actionmodifyclustersgroupsoptions.h"

PB_ActionModifyClustersGroups::PB_ActionModifyClustersGroups(QMap<const CT_Point2D *, QPair<CT_PointCloudIndexVector *, QList<const CT_PointCluster *> *> > *map) : CT_AbstractActionForGraphicsView()
{
    _positionToCluster = map;

    m_status = 0;
    m_mousePressed = false;
    m_selectionMode = GraphicsViewInterface::SELECT_ONE;
//    m_drawMode = SELECT_CURRENT_MODE;

    _automaticColorList.append(QColor(255,255,200)); // Jaune Clair
    _automaticColorList.append(QColor(255,200,255)); // Magenta Clair
    _automaticColorList.append(QColor(200,255,255)); // Cyan Clair
    _automaticColorList.append(QColor(200,200,255)); // Mauve Clair
    _automaticColorList.append(QColor(200,255,200)); // Vert Clair
    _automaticColorList.append(QColor(255,200,200)); // Rouge Clair
    _automaticColorList.append(QColor(255,200,150)); // Orange clair
    _automaticColorList.append(QColor(150,200,255)); // Bleu Clair
    _automaticColorList.append(QColor(200,255,150)); // Vert-Jaune Clair
    _automaticColorList.append(QColor(150,255,200)); // Turquoise Clair
    _automaticColorList.append(QColor(255,150,200)); // Rose Clair
    _automaticColorList.append(QColor(200,150,255)); // Violet Clair
    _automaticColorList.append(QColor(255,255,0  )); // Jaune
    _automaticColorList.append(QColor(255,0  ,255)); // Magenta
    _automaticColorList.append(QColor(0  ,255,255)); // Cyan
    _automaticColorList.append(QColor(0  ,0  ,255)); // Mauve
    _automaticColorList.append(QColor(255,150,0  )); // Orange
    _automaticColorList.append(QColor(150,255,0  )); // Vert-Jaune
    _automaticColorList.append(QColor(0  ,255,150)); // Turquoise
    _automaticColorList.append(QColor(255,0  ,150)); // Rose
    _automaticColorList.append(QColor(150,0  ,255)); // Violet

    _colorA     = QColor(0  ,255,0  ); // Vert
    _colorB     = QColor(0  ,150,255); // Bleu
    _colorTmp   = QColor(255,255,255); // Blanc
    _colorTrash = QColor(0  ,0  ,0  ); // Noir
}

QString PB_ActionModifyClustersGroups::uniqueName() const
{
    return "PB_ActionModifyClustersGroups";
}

QString PB_ActionModifyClustersGroups::title() const
{
    return tr("Sélection");
}

QString PB_ActionModifyClustersGroups::description() const
{
    return tr("Sélection d'éléments");
}

QIcon PB_ActionModifyClustersGroups::icon() const
{
    return QIcon(":/icons/cursor.png");
}

QString PB_ActionModifyClustersGroups::type() const
{
    return CT_AbstractAction::TYPE_SELECTION;
}

void PB_ActionModifyClustersGroups::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionModifyClustersGroupsOptions *option = new PB_ActionModifyClustersGroupsOptions(this);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);
    }

    document()->removeAllItemDrawable();

    int colorNum = 0;
    int posNum = 0;
    // Initialisation de _clusterToPosition
    QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > it(*_positionToCluster);
    while (it.hasNext())
    {
        it.next();
        const CT_Point2D* position = it.key();

        // Création de la liste des couleurs de base (hors groupes A et B)
        _positionsBaseColors.insert(position, _automaticColorList.at(colorNum++));
        if (colorNum >= _automaticColorList.size()) {colorNum = 0;}

        // Choix par défaut des scènes A et B (initialisation)
        if (posNum == 0)
        {
            _positionA = position;
        } else if (posNum == 1)
        {
            _positionB = position;
        }

        const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = it.value();
        for (int i = 0 ; i < pair.second->size() ; i++)
        {
            CT_PointCluster* cluster = (CT_PointCluster*) pair.second->at(i);
            _clusterToPosition.insert(cluster, position);

            document()->addItemDrawable(*cluster);

            if (posNum == 0)
            {
                document()->setColor(cluster, _colorA);
            } else if (posNum == 1)
            {
                document()->setColor(cluster, _colorB);
            } else {
                document()->setColor(cluster, _positionsBaseColors.value(position, _colorTmp));
            }
        }

        posNum++;
    }

}

bool PB_ActionModifyClustersGroups::mousePressEvent(QMouseEvent *e)
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

bool PB_ActionModifyClustersGroups::mouseMoveEvent(QMouseEvent *e)
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

bool PB_ActionModifyClustersGroups::mouseReleaseEvent(QMouseEvent *e)
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

bool PB_ActionModifyClustersGroups::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

bool PB_ActionModifyClustersGroups::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

void PB_ActionModifyClustersGroups::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
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

CT_AbstractAction* PB_ActionModifyClustersGroups::copy() const
{
    return new PB_ActionModifyClustersGroups(_positionToCluster);
}

bool PB_ActionModifyClustersGroups::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    if(!m_mousePressed)
    {
        m_selectionMode = mode;
        return true;
    }

    return false;
}

//bool PB_ActionModifyClustersGroups::setDrawMode(PB_ActionModifyClustersGroups::SelectionDrawMode mode)
//{
//    if(!m_mousePressed)
//    {
//        m_drawMode = mode;
//        return true;
//    }

//    return false;
//}

GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroups::selectionMode() const
{
    return m_selectionMode;
}

//PB_ActionModifyClustersGroups::SelectionDrawMode PB_ActionModifyClustersGroups::drawMode() const
//{
//    return m_drawMode;
//}

//void PB_ActionModifyClustersGroups::setNewDrawMode()
//{
//    GraphicsViewInterface *view = graphicsView();
//    view->setDrawModeChangeTime(0);

//    if(drawMode() != SELECT_CURRENT_MODE)
//        view->setDrawMode((GraphicsViewInterface::DrawMode)drawMode());
//    else
//        view->setDrawMode(m_backupDrawMode);
//}

//void PB_ActionModifyClustersGroups::setBackupDrawMode()
//{
//    GraphicsViewInterface *view = graphicsView();

//    view->setDrawModeChangeTime(view->getOptions().getFastDrawTime());
//}

GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroups::selectionModeToBasic(GraphicsViewInterface::SelectionMode mode) const
{
    int m = mode;

    while(m > GraphicsViewInterface::REMOVE_ONE)
        m -= GraphicsViewInterface::REMOVE_ONE;

    return (GraphicsViewInterface::SelectionMode)m;
}
