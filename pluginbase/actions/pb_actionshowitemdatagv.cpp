#include "pb_actionshowitemdatagv.h"
#include "views/actions/pb_actionselectitemdrawablegvoptions.h"

#include "ct_global/ct_context.h"

#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"


#include "ct_iterator/ct_pointiterator.h"
#include "ct_iterator/ct_faceiterator.h"
#include "ct_iterator/ct_edgeiterator.h"
#include "ct_accessor/ct_pointaccessor.h"

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

PB_ActionShowItemDataGV::PB_ActionShowItemDataGV() : CT_AbstractActionForGraphicsView()
{
    m_selectAction = new PB_ActionSelectItemDrawableGV();
    _previousSelectedItem = NULL;
}

PB_ActionShowItemDataGV::~PB_ActionShowItemDataGV()
{
    delete m_selectAction;
}

void PB_ActionShowItemDataGV::init()
{
    CT_AbstractActionForGraphicsView::init();

    m_selectAction->setGraphicsView(graphicsView());
    m_selectAction->setDocument(document());

    if(nOptions() == 0)
    {
        // create the option widget if it was not already created
        PB_ActionSelectItemDrawableGVOptions *option = new PB_ActionSelectItemDrawableGVOptions(m_selectAction);

        // add the options to the graphics view
        graphicsView()->addActionOptions(option);

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);
    }
}

QString PB_ActionShowItemDataGV::uniqueName() const
{
    return "PB_ActionShowItemDataGV";
}

QString PB_ActionShowItemDataGV::title() const
{
    return tr("Information Item");
}

QString PB_ActionShowItemDataGV::description() const
{
    return tr("Action qui affiche les données de l'Item sélectionné.");
}

QIcon PB_ActionShowItemDataGV::icon() const
{
    return QIcon(":/icons/info.png");
}

QString PB_ActionShowItemDataGV::type() const
{
    return CT_AbstractAction::TYPE_INFORMATION;
}

bool PB_ActionShowItemDataGV::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    m_use = true;

    return m_selectAction->mousePressEvent(e);
}

bool PB_ActionShowItemDataGV::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    m_use = false;

    return m_selectAction->mouseMoveEvent(e);
}

bool PB_ActionShowItemDataGV::mouseReleaseEvent(QMouseEvent *e)
{
    bool result = m_selectAction->mouseReleaseEvent(e);

    CT_CIR pcir = graphicsView()->getSelectedPoints();
    CT_PointIterator it(pcir);

    if(it.hasNext())
    {
        it.next();

        const CT_Point &p = it.currentPoint();
        PS_LOG->addMessage(LogInterface::info, LogInterface::action, QString("(X Y Z) = \t%1\t%2\t%3").arg(QString::number(p(0), 'f', 2)).arg(QString::number(p(1), 'f', 2)).arg(QString::number(p(2), 'f', 2)));
    }

    return result;
}

void PB_ActionShowItemDataGV::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)

    m_selectAction->drawOverlay(view, painter);

    // draw information about the first item selected
    QList<CT_AbstractItemDrawable*> selected = graphicsView()->getSelectedItems();
    QListIterator<CT_AbstractItemDrawable*> itS(selected);

    int add = painter.fontMetrics().height()+2;
    int y = add;

    CT_AbstractSingularItemDrawable *sItem = NULL;

    while(itS.hasNext() && (sItem == NULL))
        sItem = dynamic_cast<CT_AbstractSingularItemDrawable*>(itS.next());


    if(sItem != NULL)
    {
        bool showLog = (sItem != _previousSelectedItem);
        QString logMessage = "";
        if (showLog)
        {
            logMessage.append("<br>-------------------------------------<br>");
            logMessage.append(tr("Informations sur l'item sélectionné :<br>"));
        }

        QList<CT_AbstractItemAttribute*> attList = sItem->itemAttributes();

        if(!attList.isEmpty())
        {
            painter.save();
            painter.setPen(QColor(255,255,255));
            QListIterator<CT_AbstractItemAttribute*> itAtt(attList);

            while(itAtt.hasNext())
            {
                CT_AbstractItemAttribute *att = itAtt.next();


                QString txt = att->displayableName() + " = " + att->toString(sItem, NULL);
                painter.drawText(2, y, txt);
                y += add;

                if (showLog)
                {
                    logMessage.append(QString("%1<br>").arg(txt));
                }
            }

            painter.restore();

            y += add;
        }

        if (showLog)
        {
            PS_LOG->addMessage(LogInterface::info, LogInterface::action, logMessage);
        }
    }
    _previousSelectedItem = sItem;

    CT_SPCIR pcir = graphicsView()->getSelectedPoints();

    CT_PointIterator itp(pcir);

    if(itp.hasNext())
    {
        itp.next();

        const CT_Point &p = itp.currentPoint();

        painter.save();
        painter.setPen(QColor(255,255,255));
        painter.drawText(2, y, tr("Nombre de points sélectionnés = %1").arg(itp.size()));
        y += add;
        painter.drawText(2, y, QString("P1 { %1 ; %2 ; %3 }").arg(p(0)).arg(p(1)).arg(p(2)));
        y += add;
        painter.drawText(2, y, QString("..."));
        painter.restore();
        y += (add*2);
    }

    if (itp.size() == 2)
    {
        painter.save();
        painter.setPen(QColor(255,255,255));

        itp.toFront();

        itp.next();
        CT_Point p1 = itp.currentPoint();
        itp.next();
        CT_Point p2 = itp.currentPoint();
        double dist3D = sqrt(pow(p1(0) - p2(0), 2) + pow(p1(1) - p2(1), 2) + pow(p1(2) - p2(2), 2));
        double dist2D = sqrt(pow(p1(0) - p2(0), 2) + pow(p1(1) - p2(1), 2));
        painter.drawText(2, y, tr("Distance 3D      = %1").arg(dist3D));
        y += add;
        painter.drawText(2, y, tr("Distance 2D (XY) = %1").arg(dist2D));
        y += add;
        painter.restore();
    }

    CT_SFCIR fcir = graphicsView()->getSelectedFaces();

    CT_FaceIterator itf(fcir);
    CT_PointAccessor pAccess;

    if(itf.hasNext())
    {
        itf.next();

        const CT_Face &f = itf.cT();

        const CT_Point &p1 = pAccess.constPointAt(f.iPointAt(0));
        const CT_Point &p2 = pAccess.constPointAt(f.iPointAt(1));
        const CT_Point &p3 = pAccess.constPointAt(f.iPointAt(2));

        painter.save();
        painter.setPen(QColor(255,255,255));
        painter.drawText(2, y, tr("Nombre de faces sélectionnés = %1").arg(itf.size()));
        y += add;
        painter.drawText(2, y, QString("F1 { %1 ; %2 ; %3 } { %4 ; %5 ; %6 } { %7 ; %8 ; %9 }").arg(p1(0)).arg(p1(1)).arg(p1(2))
                         .arg(p2(0)).arg(p2(1)).arg(p2(2))
                         .arg(p3(0)).arg(p3(1)).arg(p3(2)));
        y += add;
        painter.drawText(2, y, QString("..."));
        painter.restore();
        y += (add*2);
    }

    CT_SECIR ecir = graphicsView()->getSelectedEdges();
    CT_EdgeIterator ite(ecir);

    if(ite.hasNext())
    {
        ite.next();

        const CT_Edge &e = ite.cT();
        const CT_Point &p1 = pAccess.constPointAt(e.iPointAt(0));
        const CT_Point &p2 = pAccess.constPointAt(e.iPointAt(1));

        painter.save();
        painter.setPen(QColor(255,255,255));
        painter.drawText(2, y, tr("Nombre d'arêtes sélectionnés = %1").arg(ite.size()));
        y += add;
        painter.drawText(2, y, QString("E1 { %1 ; %2 ; %3 } { %4 ; %5 ; %6 }").arg(p1(0)).arg(p1(1)).arg(p1(2))
                                                                              .arg(p2(0)).arg(p2(1)).arg(p2(2)));
        y += add;
        painter.drawText(2, y, QString("..."));
        painter.restore();
        y += (add*2);
    }
}

CT_AbstractAction *PB_ActionShowItemDataGV::copy() const
{
    return new PB_ActionShowItemDataGV();
}

