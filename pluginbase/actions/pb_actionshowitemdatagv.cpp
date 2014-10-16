#include "pb_actionshowitemdatagv.h"
#include "views/actions/pb_actionselectitemdrawablegvoptions.h"

#include "ct_global/ct_context.h"

#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

PB_ActionShowItemDataGV::PB_ActionShowItemDataGV() : CT_AbstractActionForGraphicsView()
{
    m_selectAction = new PB_ActionSelectItemDrawableGV();
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
    return "Information ItemDrawable";
}

QString PB_ActionShowItemDataGV::description() const
{
    return "Une action qui affiche les données de l'ItemDrawable sélectionné.";
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

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> pcir = graphicsView()->getSelectedPoints();
    if(pcir.data() != NULL)
    {
        CT_AbstractModifiableCloudIndex *pci = pcir->abstractModifiableCloudIndex();
        size_t size = pci->size();

        if(size > 0)
        {
            CT_AbstractCloudT<CT_Point> *pc = PS_REPOSITORY->globalCloud<CT_Point>();
            const CT_Point &p = pc->constTAt(pci->indexAt(0));
            PS_LOG->addMessage(LogInterface::info, LogInterface::action, QString("(X Y Z) = \t%1\t%2\t%3").arg(p.getX()).arg(p.getY()).arg(p.getZ()));
        }
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
        QList<CT_AbstractItemAttribute*> attList = sItem->itemAttributes();

        if(!attList.isEmpty())
        {
            painter.save();
            painter.setPen(QColor(255,255,255,127));
            QListIterator<CT_AbstractItemAttribute*> itAtt(attList);

            while(itAtt.hasNext())
            {
                CT_AbstractItemAttribute *att = itAtt.next();

                QString txt = att->displayableName() + " = ";

                if(att->category()->isEquivalentTo(CT_AbstractCategory::DATA_COORDINATE))
                {
                    bool e = att->useCoordinateSystem();
                    att->setUseCoordinateSystem(true);
                    txt += att->toString(sItem, NULL);
                    att->setUseCoordinateSystem(false);
                    txt += " (" + att->toString(sItem, NULL) + ")";
                    att->setUseCoordinateSystem(e);
                }
                else
                {
                    txt += att->toString(sItem, NULL);
                }

                painter.drawText(2, y, txt);

                y += add;
            }

            painter.restore();

            y += add;
        }
    }

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> pcir = graphicsView()->getSelectedPoints();

    if(pcir.data() != NULL)
    {
        CT_AbstractModifiableCloudIndex *pci = pcir->abstractModifiableCloudIndex();
        size_t size = pci->size();

        if(size > 0)
        {
            CT_AbstractCloudT<CT_Point> *pc = PS_REPOSITORY->globalCloud<CT_Point>();
            const CT_Point &p = pc->constTAt(pci->indexAt(0));

            painter.save();
            painter.setPen(QColor(255,255,255,127));
            painter.drawText(2, y, QString("Nombre de points sélectionnés = %1").arg(size));
            y += add;
            painter.drawText(2, y, QString("P1 { %1 ; %2 ; %3 }").arg(p.getX()).arg(p.getY()).arg(p.getZ()));
            y += add;
            painter.drawText(2, y, QString("..."));
            painter.restore();
            y += (add*2);
        }
    }

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> fcir = graphicsView()->getSelectedFaces();

    if(pcir.data() != NULL)
    {
        CT_AbstractModifiableCloudIndex *fci = fcir->abstractModifiableCloudIndex();
        size_t size = fci->size();

        if(size > 0)
        {
            CT_AbstractCloudT<CT_Face> *fc = PS_REPOSITORY->globalCloud<CT_Face>();
            const CT_Face &f = fc->constTAt(fci->indexAt(0));

            CT_Point *p1 = f.pointAt(0);
            CT_Point *p2 = f.pointAt(1);
            CT_Point *p3 = f.pointAt(2);

            painter.save();
            painter.setPen(QColor(255,255,255,127));
            painter.drawText(2, y, QString("Nombre de faces sélectionnés = %1").arg(size));
            y += add;
            painter.drawText(2, y, QString("F1 { %1 ; %2 ; %3 } { %4 ; %5 ; %6 } { %7 ; %8 ; %9 }").arg(p1->getX()).arg(p1->getY()).arg(p1->getZ())
                             .arg(p2->getX()).arg(p2->getY()).arg(p2->getZ())
                             .arg(p3->getX()).arg(p3->getY()).arg(p3->getZ()));
            y += add;
            painter.drawText(2, y, QString("..."));
            painter.restore();
            y += (add*2);
        }
    }

    QSharedPointer<CT_AbstractModifiableCloudIndexRegistered> ecir = graphicsView()->getSelectedEdges();

    if(pcir.data() != NULL)
    {
        CT_AbstractModifiableCloudIndex *eci = ecir->abstractModifiableCloudIndex();
        size_t size = eci->size();

        if(size > 0)
        {
            CT_AbstractCloudT<CT_Edge> *ec = PS_REPOSITORY->globalCloud<CT_Edge>();
            const CT_Edge &e = ec->constTAt(eci->indexAt(0));

            CT_Point *p1 = e.pointAt(0);
            CT_Point *p2 = e.pointAt(1);

            painter.save();
            painter.setPen(QColor(255,255,255,127));
            painter.drawText(2, y, QString("Nombre d'edges sélectionnés = %1").arg(size));
            y += add;
            painter.drawText(2, y, QString("E1 { %1 ; %2 ; %3 } { %4 ; %5 ; %6 }").arg(p1->getX()).arg(p1->getY()).arg(p1->getZ()).arg(p2->getX()).arg(p2->getY()).arg(p2->getZ()));
            y += add;
            painter.drawText(2, y, QString("..."));
            painter.restore();
            y += (add*2);
        }
    }
}

CT_AbstractAction *PB_ActionShowItemDataGV::copy() const
{
    return new PB_ActionShowItemDataGV();
}

