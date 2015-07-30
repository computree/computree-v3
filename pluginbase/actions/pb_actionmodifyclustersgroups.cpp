#include "pb_actionmodifyclustersgroups.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <math.h>

#include "views/actions/pb_actionmodifyclustersgroupsoptions.h"

#include "ct_math/ct_mathpoint.h"

PB_ActionModifyClustersGroups::PB_ActionModifyClustersGroups(QMap<const CT_Point2D *, QPair<CT_PointCloudIndexVector *, QList<const CT_PointCluster *> *> > *map) : CT_AbstractActionForGraphicsView()
{
    _positionToCluster = map;

    m_status = 0;
    m_mousePressed = false;
    m_selectionMode = GraphicsViewInterface::SELECT_ONE;

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
    _colorB     = QColor(0  ,0  ,255); // Bleu
    _colorTmp   = QColor(255,255,255); // Blanc
    _colorTrash = QColor(125,125,125); // Grey

    _ABColors = true;
    _positionsChanged = true;
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

        document()->removeAllItemDrawable();
        document()->beginAddMultipleItemDrawable();

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
                _positionA = (CT_Point2D*) position;
            } else if (posNum == 1)
            {
                _positionB = (CT_Point2D*) position;
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

        document()->endAddMultipleItemDrawable();

        option->selectColorA(_colorA);
        option->selectColorB(_colorB);

        connect(option, SIGNAL(setColorA(QColor)), this, SLOT(setColorA(QColor)));
        connect(option, SIGNAL(setColorB(QColor)), this, SLOT(setColorB(QColor)));
        connect(option, SIGNAL(selectPositionA()), this, SLOT(selectPositionA()));
        connect(option, SIGNAL(selectPositionB()), this, SLOT(selectPositionB()));
        connect(option, SIGNAL(visibilityChanged()), this, SLOT(visibilityChanged()));
        connect(option, SIGNAL(affectClusterToA()), this, SLOT(affectClusterToA()));
        connect(option, SIGNAL(affectClusterToB()), this, SLOT(affectClusterToB()));
        connect(option, SIGNAL(affectClusterToTMP()), this, SLOT(affectClusterToTMP()));
        connect(option, SIGNAL(affectClusterToTrash()), this, SLOT(affectClusterToTrash()));
        connect(option, SIGNAL(enterLimitMode()), this, SLOT(updateLimitMode()));
        connect(option, SIGNAL(distanceChanged(int)), this, SLOT(distanceChanged(int)));

        _positionsChanged = true;
        option->selectLimitMode();

        dynamic_cast<GraphicsViewInterface*>(document()->views().first())->camera()->fitCameraToVisibleItems();
    }

}

void PB_ActionModifyClustersGroups::updateAllClustersColors()
{
    const QList<const CT_PointCluster*>* listA = _positionToCluster->value(_positionA).second;
    const QList<const CT_PointCluster*>* listB = _positionToCluster->value(_positionB).second;

    //const QList<CT_AbstractItemDrawable*>& visibleItems = document()->getItemDrawable();

    QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > it(*_positionToCluster);
    while (it.hasNext())
    {
        it.next();
        const CT_Point2D* position = it.key();
        const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = it.value();

        for (int i = 0 ; i < pair.second->size() ; i++)
        {
            CT_PointCluster* cluster = (CT_PointCluster*) pair.second->at(i);


            if (_ABColors && listA->contains(cluster))
            {
                document()->setColor(cluster, _colorA);
            } else if (_ABColors && listB->contains(cluster))
            {
                document()->setColor(cluster, _colorB);
            } else if (_trashClusterList.contains(cluster))
            {
                document()->setColor(cluster, _colorTrash);
            } else {
                document()->setColor(cluster, _positionsBaseColors.value(position, _colorTmp));
            }
        }
    }
}


void PB_ActionModifyClustersGroups::updateColorForOneCluster(const CT_Point2D* position)
{
    QColor clusterColor = _positionsBaseColors.value(position, _colorTmp);
    if (_ABColors && position == _positionA) {clusterColor = _colorA;}
    if (_ABColors && position == _positionB) {clusterColor = _colorB;}

    //const QList<CT_AbstractItemDrawable*>& visibleItems = document()->getItemDrawable();

    const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = _positionToCluster->value(position);

    for (int i = 0 ; i < pair.second->size() ; i++)
    {
        CT_PointCluster* cluster = (CT_PointCluster*) pair.second->at(i);
        document()->setColor(cluster, clusterColor);
    }
}



bool PB_ActionModifyClustersGroups::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
        return false;

    GraphicsViewInterface *view = graphicsView();

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

    if((m_status > 0)
            && (e->button() == Qt::LeftButton))
    {
        m_status = 0;

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
            }
            else
            {
                m_selectionRectangle = m_selectionRectangle.normalized();

                // Define selection window dimensions
                view->setSelectRegionWidth(m_selectionRectangle.width());
                view->setSelectRegionHeight(m_selectionRectangle.height());
                // Compute rectangle center and perform selection
                view->select(m_selectionRectangle.center());

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
    PB_ActionModifyClustersGroupsOptions *option = (PB_ActionModifyClustersGroupsOptions*)optionAt(0);

    if(option->isInSceneMode() && (e->key() == Qt::Key_Control) && !e->isAutoRepeat())
    {
        option->setMultiSelect(true);
        setSelectionMode(option->selectionMode());
        return true;
    }

    return false;
}

bool PB_ActionModifyClustersGroups::keyReleaseEvent(QKeyEvent *e)
{
    PB_ActionModifyClustersGroupsOptions *option = (PB_ActionModifyClustersGroupsOptions*)optionAt(0);

    if(option->isInSceneMode() && (e->key() == Qt::Key_Control) && !e->isAutoRepeat())
    {
        option->setMultiSelect(false);
        setSelectionMode(option->selectionMode());
        return true;
    }
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

GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroups::selectionMode() const
{
    return m_selectionMode;
}

void PB_ActionModifyClustersGroups::setColorA(QColor color)
{
    _colorA = color;
    updateColorForOneCluster(_positionA);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::setColorB(QColor color)
{
    _colorB = color;
    updateColorForOneCluster(_positionB);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::selectPositionA()
{
    _positionsChanged = true;
    QList<CT_AbstractItemDrawable*> selectedItems = document()->getSelectedItemDrawable();

    int cpt = 0;
    while (selectedItems.size() > cpt)
    {
        CT_AbstractItemDrawable* item = selectedItems.at(cpt++);
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(item);

        if (cluster != NULL)
        {
            const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);

            if (position != NULL)
            {
                if (position == _positionB)
                {
                    swapAandB();
                    cpt = selectedItems.size();
                } else
                {
                    CT_Point2D* oldPositionA = _positionA;
                    _positionA = (CT_Point2D*) position;

                    updateColorForOneCluster(oldPositionA);
                    updateColorForOneCluster(_positionA);
                    document()->redrawGraphics();
                    cpt = selectedItems.size();
                }
            }
        }
    }
    updateLimitMode();
}

void PB_ActionModifyClustersGroups::selectPositionB()
{
    _positionsChanged = true;
    QList<CT_AbstractItemDrawable*> selectedItems = document()->getSelectedItemDrawable();

    int cpt = 0;
    while (selectedItems.size() > cpt)
    {
        CT_AbstractItemDrawable* item = selectedItems.at(cpt++);
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(item);

        if (cluster != NULL)
        {
            const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);

            if (position != NULL)
            {
                if (position == _positionA)
                {
                    swapAandB();
                    cpt = selectedItems.size();
                } else
                {
                    CT_Point2D* oldPositionB = _positionB;
                    _positionB = (CT_Point2D*) position;

                    updateColorForOneCluster(oldPositionB);
                    updateColorForOneCluster(_positionB);
                    document()->redrawGraphics();
                    cpt = selectedItems.size();
                }
            }
        }
    }
    updateLimitMode();
}

void PB_ActionModifyClustersGroups::swapAandB()
{
    CT_Point2D* oldPositionB = _positionB;
    _positionB = _positionA;
    _positionA = oldPositionB;
    updateColorForOneCluster(_positionA);
    updateColorForOneCluster(_positionB);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::visibilityChanged()
{
    updateVisiblePositions();
}

void PB_ActionModifyClustersGroups::addToA(CT_PointCluster* cluster)
{
    const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);

    if (position != _positionA)
    {
        // Suppression
        if (position != NULL)
        {
            QList<const CT_PointCluster*> *list = _positionToCluster->value(position).second;
            if (list != NULL)
            {
                list->removeOne(cluster);
            } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}
            _clusterToPosition.remove(cluster);
        }

        // Ajout
        QList<const CT_PointCluster*> *list2 = _positionToCluster->value(_positionA).second;
        if (list2 != NULL)
        {
            list2->append(cluster);
        } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}

        // Suppression
        _temporaryClusterList.removeOne(cluster);
        _trashClusterList.removeOne(cluster);

        // Ajout
        _clusterToPosition.insert(cluster, _positionA);

        // Couleur
        if (_ABColors)
        {
            document()->setColor(cluster, _colorA);
        } else {
            document()->setColor(cluster, _positionsBaseColors.value(_positionA, _colorTmp));
        }
    }

}

void PB_ActionModifyClustersGroups::addToB(CT_PointCluster* cluster)
{
    const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);

    if (position != _positionB)
    {
        // Suppression
        if (position != NULL)
        {
            QList<const CT_PointCluster*> *list = _positionToCluster->value(position).second;
            if (list != NULL)
            {
                list->removeOne(cluster);
            } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}
            _clusterToPosition.remove(cluster);
        }

        // Ajout
        QList<const CT_PointCluster*> *list2 = _positionToCluster->value(_positionB).second;
        if (list2 != NULL)
        {
            list2->append(cluster);
        } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}

        // Suppression
        _temporaryClusterList.removeOne(cluster);
        _trashClusterList.removeOne(cluster);

        // Ajout
        _clusterToPosition.insert(cluster, _positionB);

        // Couleur
        if (_ABColors)
        {
            document()->setColor(cluster, _colorB);
        } else {
            document()->setColor(cluster, _positionsBaseColors.value(_positionB, _colorTmp));
        }
    }

}

void PB_ActionModifyClustersGroups::affectClusterToA()
{
    QList<CT_AbstractItemDrawable*> selected = document()->getSelectedItemDrawable();

    for (int i = 0 ; i < selected.size() ; i++)
    {
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(selected[i]);

        if (cluster  != NULL)
        {
            addToA(cluster);
        }
    }
    document()->setSelectAllItemDrawable(false);
    document()->redrawGraphics();
}


void PB_ActionModifyClustersGroups::affectClusterToB()
{
    QList<CT_AbstractItemDrawable*> selected = document()->getSelectedItemDrawable();

    for (int i = 0 ; i < selected.size() ; i++)
    {
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(selected[i]);

        if (cluster  != NULL)
        {
            addToB(cluster);
        }
    }
    document()->setSelectAllItemDrawable(false);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::affectClusterToTMP()
{
    QList<CT_AbstractItemDrawable*> selected = document()->getSelectedItemDrawable();

    for (int i = 0 ; i < selected.size() ; i++)
    {
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(selected[i]);

        if (cluster  != NULL)
        {
            // Suppression
            const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);
            if (position != NULL)
            {
                QList<const CT_PointCluster*> *list = _positionToCluster->value(position).second;
                if (list != NULL)
                {
                    list->removeOne(cluster);
                } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}
                _clusterToPosition.remove(cluster);
            }
            _trashClusterList.removeOne(cluster);

            // Ajout
            if (!_temporaryClusterList.contains(cluster))
            {
                _temporaryClusterList.append(cluster);
                document()->setColor(cluster, _colorTmp);
            }
        }
    }
    document()->setSelectAllItemDrawable(false);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::affectClusterToTrash()
{
    QList<CT_AbstractItemDrawable*> selected = document()->getSelectedItemDrawable();

    for (int i = 0 ; i < selected.size() ; i++)
    {
        CT_PointCluster* cluster = dynamic_cast<CT_PointCluster*>(selected[i]);

        if (cluster  != NULL)
        {
            // Suppression
            const CT_Point2D* position = _clusterToPosition.value(cluster, NULL);
            if (position != NULL)
            {
                QList<const CT_PointCluster*> *list = _positionToCluster->value(position).second;
                if (list != NULL)
                {
                    list->removeOne(cluster);
                } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Pas de liste pour la position"));}
                _clusterToPosition.remove(cluster);
            }
            _temporaryClusterList.removeOne(cluster);

            // Ajout
            if (!_trashClusterList.contains(cluster))
            {
                _trashClusterList.append(cluster);
                document()->setColor(cluster, _colorTrash);
            }
        }
    }
    document()->setSelectAllItemDrawable(false);
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::updateLimitMode()
{
    if (_positionsChanged)
    {
        Eigen::Vector3d origin = _positionA->getCenterCoordinate();
        Eigen::Vector3d direction = _positionB->getCenterCoordinate() - _positionA->getCenterCoordinate();
        direction.normalize();

        QList<const CT_PointCluster*>* listA = _positionToCluster->value(_positionA).second;
        QList<const CT_PointCluster*>* listB = _positionToCluster->value(_positionB).second;

        if (listA != NULL && listB != NULL)
        {
            _positionsChanged = false;
            _clustersOrdered.clear();

            // Scene A
            QMultiMap<float, CT_PointCluster*> map;
            for (int i = 0 ; i < listA->size() ; i++)
            {
                CT_PointCluster* clusterA = (CT_PointCluster*) (listA->at(i));
                Eigen::Vector3d point = clusterA->getCenterCoordinate();

                float distance = direction(0)*(point(0) - origin(0)) + direction(1)*(point(1) - origin(1));
                map.insert(distance, clusterA);
            }
            _currentLastA = map.size() - 1;

            // Scene B
            for (int i = 0 ; i < listB->size() ; i++)
            {
                CT_PointCluster* clusterB = (CT_PointCluster*) (listB->at(i));
                Eigen::Vector3d point = clusterB->getCenterCoordinate();

                float distance = direction(0)*(point(0) - origin(0)) + direction(1)*(point(1) - origin(1));
                map.insert(distance, clusterB);
            }
            _clustersOrdered.append(map.values());
        } else {PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Positions invalides")); return;}
    }

    PB_ActionModifyClustersGroupsOptions *option = (PB_ActionModifyClustersGroupsOptions*)optionAt(0);
    option->setMaxDistance(_clustersOrdered.size());
    option->setDistance(_currentLastA);
}

void PB_ActionModifyClustersGroups::distanceChanged(int val)
{
    if (val > _currentLastA)
    {
        for (int i = _currentLastA + 1 ; i <= val ; i++)
        {
            CT_PointCluster* cluster = _clustersOrdered.at(i - 1);
            addToA(cluster);
        }
    } else if (val < _currentLastA)
    {
        for (int i = _currentLastA ; i > val ; i--)
        {
            CT_PointCluster* cluster = _clustersOrdered.at(i - 1);
            addToB(cluster);
        }
    }
    _currentLastA = val;
    document()->redrawGraphics();
}

void PB_ActionModifyClustersGroups::updateVisiblePositions()
{
    PB_ActionModifyClustersGroupsOptions *option = (PB_ActionModifyClustersGroupsOptions*)optionAt(0);


    document()->removeAllItemDrawable();
    document()->beginAddMultipleItemDrawable();

    if (option->isOthersVisible())
    {
        QMapIterator<const CT_Point2D*, QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > > it(*_positionToCluster);
        while (it.hasNext())
        {
            it.next();

            const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pair = it.value();
            for (int i = 0 ; i < pair.second->size() ; i++)
            {
                CT_PointCluster* cluster = (CT_PointCluster*) pair.second->at(i);
                document()->addItemDrawable(*cluster);
            }
        }
    } else {
        if (option->isAVisible())
        {
            const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pairA = _positionToCluster->value(_positionA);
            for (int i = 0 ; i < pairA.second->size() ; i++)
            {
                CT_PointCluster* cluster = (CT_PointCluster*) pairA.second->at(i);
                document()->addItemDrawable(*cluster);
            }
        }

        if (option->isBVisible())
        {
            const QPair<CT_PointCloudIndexVector*, QList<const CT_PointCluster*>* > &pairB = _positionToCluster->value(_positionB);
            for (int i = 0 ; i < pairB.second->size() ; i++)
            {
                CT_PointCluster* cluster = (CT_PointCluster*) pairB.second->at(i);
                document()->addItemDrawable(*cluster);
            }
        }
    }

    if (option->isTMPVisible())
    {
        for (int i = 0 ; i < _temporaryClusterList.size() ; i++)
        {
            document()->addItemDrawable(*(_temporaryClusterList.at(i)));
        }
    }

    if (option->isTrashVisible())
    {
        for (int i = 0 ; i < _trashClusterList.size() ; i++)
        {
            document()->addItemDrawable(*(_trashClusterList.at(i)));
        }
    }

    document()->endAddMultipleItemDrawable();
    document()->redrawGraphics();
}



GraphicsViewInterface::SelectionMode PB_ActionModifyClustersGroups::selectionModeToBasic(GraphicsViewInterface::SelectionMode mode) const
{
    int m = mode;

    while(m > GraphicsViewInterface::REMOVE_ONE)
        m -= GraphicsViewInterface::REMOVE_ONE;

    return (GraphicsViewInterface::SelectionMode)m;
}
