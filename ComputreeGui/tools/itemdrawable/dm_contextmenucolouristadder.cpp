#include "dm_contextmenucolouristadder.h"

#include "interfaces.h"
#include "ct_point.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"
#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "qtcolorpicker/qtcolorpicker.h"
#include "dm_itemdrawablemanageroptions.h"
#include "tools/graphicsview/dm_pointscolourist.h"
#include "tools/graphicsview/dm_pointsrecoverer.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"

#include "dm_guimanager.h"

DM_ContextMenuColouristAdder::DM_ContextMenuColouristAdder(IColouristContextMenuAccess &access, QObject *parent) : m_itemAccess(access), QObject(parent)
{
    m_docManager = NULL;
    m_autoColorProducer = NULL;
    m_colorPicker = new QtColorPicker(NULL, -1, true, false);
}

DM_ContextMenuColouristAdder::~DM_ContextMenuColouristAdder()
{
    delete m_colorPicker;
}

void DM_ContextMenuColouristAdder::setDocumentManager(DocumentManagerInterface *docManager)
{
    m_docManager = docManager;
}

void DM_ContextMenuColouristAdder::setAutomaticColorProducer(DM_ItemDrawableManagerOptions *autoColorProducer)
{
    m_autoColorProducer = autoColorProducer;
}

void DM_ContextMenuColouristAdder::setGradientToUseForColorization(const QGradient &gradient)
{
    m_gradient = gradient;
}

void DM_ContextMenuColouristAdder::setLinearGradientToUseForColorization(const QLinearGradient &gradient)
{
    setGradientToUseForColorization(gradient);
}

void DM_ContextMenuColouristAdder::initContextMenu(QMenu *contextMenu)
{
    if(m_docManager == NULL)
        return;

    QMenu *menu = contextMenu->addMenu(tr("Couleur uni"));

    int nDocs = m_docManager->nDocuments();

    for(int i=0; i<nDocs; ++i)
    {
        QAction *action = menu->addAction(tr("%1").arg(m_docManager->documentAt(i)->getNumber()), this, SLOT(colorByUniqueColor()));
        action->setData(i);
    }

    if(m_autoColorProducer != NULL)
    {
        menu = contextMenu->addMenu(tr("Couleur automatique"));

        for(int i=0; i<nDocs; ++i)
        {
            QAction *action = menu->addAction(tr("%1").arg(m_docManager->documentAt(i)->getNumber()), this, SLOT(colorByAutomaticColor()));
            action->setData(i);
        }
    }

    QList<CT_OutAbstractSingularItemModel*> finalModels;
    QList<CT_AbstractModel*> list = m_itemAccess.getSelectedModelsToUseInColorizerMenu();

    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
    {
        CT_OutAbstractSingularItemModel *model = dynamic_cast<CT_OutAbstractSingularItemModel*>(it.next());

        if((model != NULL) && !model->itemAttributes().isEmpty())
        {
            if(!finalModels.contains((CT_OutAbstractSingularItemModel*)model->lastOriginalModelWithAStep()))
                finalModels.append(model);
        }
    }

    QListIterator<CT_OutAbstractSingularItemModel*> itF(finalModels);

    while(itF.hasNext())
    {
        CT_OutAbstractSingularItemModel *model = itF.next();

        menu = contextMenu->addMenu(tr("Colorier par..."));

        for(int i=0; i<nDocs; ++i)
        {
            QMenu *subMenu = menu->addMenu(tr("%1").arg(m_docManager->documentAt(i)->getNumber()));

            foreach (CT_OutAbstractItemAttributeModel *attModel, model->itemAttributes()) {
                QAction *action = subMenu->addAction(attModel->displayableName(), this, SLOT(colorByAttributeAndGradient()));
                action->setData(qVariantFromValue(DM_ContextMenuColouristAdder::ActionSetColorByAttribute(attModel, i)));
            }
        }
    }

    menu = contextMenu->addMenu(tr("Colorier les points de chaque éléments par"));

    for(int i=0; i<nDocs; ++i)
    {
        GDocumentViewForGraphics *graphicsDoc = dynamic_cast<GDocumentViewForGraphics*>(m_docManager->documentAt(i));

        if(graphicsDoc != NULL) {
            QMenu *subMenu = menu->addMenu(tr("%1").arg(graphicsDoc->getNumber()));

            QAction *action = subMenu->addAction(tr("X"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;1").arg(i).arg(CT_Point::X));

            action = subMenu->addAction(tr("Y"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;1").arg(i).arg(CT_Point::Y));

            action = subMenu->addAction(tr("Z"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;1").arg(i).arg(CT_Point::Z));
        }
    }

    menu = contextMenu->addMenu(tr("Colorier les points de tous les éléments par"));

    for(int i=0; i<nDocs; ++i)
    {
        GDocumentViewForGraphics *graphicsDoc = dynamic_cast<GDocumentViewForGraphics*>(m_docManager->documentAt(i));

        if(graphicsDoc != NULL) {
            QMenu *subMenu = menu->addMenu(tr("%1").arg(graphicsDoc->getNumber()));

            QAction *action = subMenu->addAction(tr("X"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;0").arg(i).arg(CT_Point::X));

            action = subMenu->addAction(tr("Y"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;0").arg(i).arg(CT_Point::Y));

            action = subMenu->addAction(tr("Z"), this, SLOT(colorByPointsCoordinate()));
            action->setData(QString("%1;%2;0").arg(i).arg(CT_Point::Z));
        }
    }
}

void DM_ContextMenuColouristAdder::colorByUniqueColor()
{
    m_colorPicker->showColorDialog();

    if(!m_colorPicker->isDialogCanceled())
    {
        QColor newColor = m_colorPicker->currentColor();

        int docIndex = ((QAction*)sender())->data().toInt();
        DocumentInterface *doc = m_docManager->documentAt(docIndex);

        QList<CT_AbstractItemDrawable*> items = m_itemAccess.getItemDrawableToColorize();
        QListIterator<CT_AbstractItemDrawable*> it(items);

        while(it.hasNext())
            doc->setColor((CT_AbstractItemDrawable*)it.next(), newColor);
    }
}

void DM_ContextMenuColouristAdder::colorByAutomaticColor()
{
    int docIndex = ((QAction*)sender())->data().toInt();
    DocumentInterface *doc = m_docManager->documentAt(docIndex);

    QList<CT_AbstractItemDrawable*> items = m_itemAccess.getItemDrawableToColorize();
    QListIterator<CT_AbstractItemDrawable*> it(items);

    while(it.hasNext()) {
        QColor keepAlphaColor = m_autoColorProducer->getNextColor();
        keepAlphaColor.setAlpha(KEEP_ALPHA_COLOR);

        doc->setColor((CT_AbstractItemDrawable*)it.next(), keepAlphaColor);
    }
}

void DM_ContextMenuColouristAdder::colorByAttributeAndGradient()
{
    DM_ContextMenuColouristAdder::ActionSetColorByAttribute info = ((QAction*)sender())->data().value<DM_ContextMenuColouristAdder::ActionSetColorByAttribute>();

    DocumentInterface *doc = m_docManager->documentAt(info.m_docIndex);

    DM_ColorLinearInterpolator interpolator;
    interpolator.constructFromQGradient(m_gradient);

    QList<CT_AbstractItemDrawable*> items = m_itemAccess.getItemDrawableToColorize();
    QListIterator<CT_AbstractItemDrawable*> it(items);

    QList<CT_AbstractSingularItemDrawable*> itemsOK;

    double minValue = std::numeric_limits<double>::max();
    double maxValue = -minValue;
    bool ok;

    while(it.hasNext()) {
        CT_AbstractSingularItemDrawable *item = dynamic_cast<CT_AbstractSingularItemDrawable*>(it.next());

        if(item != NULL) {
            CT_AbstractItemAttribute *att = item->itemAttribute(info.m_model);

            if(att != NULL) {
                ok = true;

                double value = 0;

                if(att->type() == CT_AbstractCategory::BOOLEAN)
                    value = att->toBool(item, &ok);
                else
                    value = att->toDouble(item, &ok);

                if(!ok) {
                    GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible de convertir l'attribut %1 en valeur double ou booléenne").arg(att->displayableName()));
                    return;
                }

                if(ok) {
                    minValue = qMin(minValue, value);
                    maxValue = qMax(maxValue, value);

                    itemsOK.append(item);
                }
            }
        }
    }

    if(!itemsOK.isEmpty()) {
        double range = maxValue-minValue;

        if(range == 0)
            range = 1;

        QListIterator<CT_AbstractSingularItemDrawable*> itOk(itemsOK);

        while(itOk.hasNext()) {
            CT_AbstractSingularItemDrawable *item = (CT_AbstractSingularItemDrawable*)itOk.next();
            CT_AbstractItemAttribute *att = item->itemAttribute(info.m_model);
            double vv = att->toDouble(item, &ok);

            // convert the value to be between 0 and 1
            double key = (vv-minValue)/range;

            // get the intermediate color and set it to document
            doc->setColor(item, interpolator.intermediateColor(key));
        }
    }
}

void DM_ContextMenuColouristAdder::colorByPointsCoordinate()
{
    QStringList info = ((QAction*)sender())->data().toString().split(";");

    if(info.size() != 3)
        return;

    bool ok;
    int docIndex = info.first().toInt(&ok);

    if(!ok)
        return;

    int coordinateIndex = info.at(1).toInt(&ok);

    if(!ok)
        return;

    bool oneByOne = (info.last().toInt(&ok) == 1);

    if(!ok)
        return;

    GDocumentViewForGraphics *graphicsDoc = dynamic_cast<GDocumentViewForGraphics*>(m_docManager->documentAt(docIndex));

    if(graphicsDoc == NULL)
        return;

    CT_CCR ccr = graphicsDoc->getGlobalColorArrayRegisteredForPoints();

    if((ccr.data() == NULL) || (ccr.data()->abstractColorCloud() == NULL))
        return;

    CT_AbstractColorCloud *colorCloud = ccr.data()->abstractColorCloud();

    QList<GGraphicsView*> graphics = graphicsDoc->getGraphicsList();

    if(graphics.isEmpty())
        return;

    GGraphicsView *view = graphics.first();

    QList<CT_AbstractItemDrawable*> items = m_itemAccess.getItemDrawableToColorize();
    QListIterator<CT_AbstractItemDrawable*> it(items);

    if(oneByOne) {
        while(it.hasNext()) {

            DM_PointsRecoverer recover;
            QList<CT_AbstractCloudIndex*> indexes = recover.recoverDrawnGlobalPointsInItemDrawable(QList<CT_AbstractItemDrawable*>() << it.next(),
                                                                                                   *view);


            DM_PointsColourist::colorGlobalPointsByCoordinate(colorCloud, indexes, m_gradient, coordinateIndex);
        }
    } else {
        DM_PointsRecoverer recover;
        QList<CT_AbstractCloudIndex*> indexes;
        CT_PointCloudIndexVector backup;

        while(it.hasNext()) {
            indexes.append(recover.recoverDrawnGlobalPointsInItemDrawable(QList<CT_AbstractItemDrawable*>() << it.next(),
                                                                          *view,
                                                                          &backup));


        }
        indexes.removeAll(&backup);
        indexes.append(&backup);

        DM_PointsColourist::colorGlobalPointsByCoordinate(colorCloud, indexes, m_gradient, coordinateIndex);
    }

    graphicsDoc->dirtyColorsOfPoints();
}
