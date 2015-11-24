#include "gpointsattributesmanager.h"
#include "ui_gpointsattributesmanager.h"

#include "dm_guimanager.h"
#include "gdocumentviewforgraphics.h"
#include "gminmaxattributesscalarconfiguration.h"

#include "tools/attributes/dm_attributesbuildingcollectiont.h"
#include "tools/attributes/worker/dm_attributescolort.h"
#include "tools/attributes/worker/dm_attributesnormalt.h"
#include "tools/attributes/worker/dm_attributesscalart.h"

#include <QMessageBox>
#include <QProgressDialog>

GPointsAttributesManager::GPointsAttributesManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GPointsAttributesManager)
{
    ui->setupUi(this);

    m_manager = NULL;
    m_doc = NULL;
    m_itemPointRootColor = NULL;
    m_itemPointRootScalar = NULL;
    m_itemPointRootNormal = NULL;
    m_itemFaceRootColor = NULL;
    m_itemFaceRootScalar = NULL;
    m_itemFaceRootNormal = NULL;
    m_itemEdgeRootColor = NULL;
    m_itemEdgeRootScalar = NULL;
    m_itemEdgeRootNormal = NULL;
    m_internalSetColor = false;

    ui->pushButtonGradientColorPicker->setStandardColors();
    ui->pushButtonAddColor->setEnabled(false);

    ui->pushButtonNormalsColorPicker->setStandardColors();
    ui->pushButtonNormalsColorPicker->setCurrentColor(Qt::white);
    ui->pushButtonApplyEditNormals->setEnabled(false);

    ui->treeView->setModel(&m_model);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(treeView_currentRowChanged(QModelIndex,QModelIndex)));
    connect(&m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
}

GPointsAttributesManager::~GPointsAttributesManager()
{
    delete ui;
}
void GPointsAttributesManager::setManager(const DM_AttributesManager *man)
{
    m_manager = (DM_AttributesManager*)man;

    if(m_manager != NULL)
        m_manager->clearInvalid();

    initTreeView();
}

void GPointsAttributesManager::setDocument(const GDocumentViewForGraphics *doc)
{
    m_doc = (GDocumentViewForGraphics*)doc;

    if(m_doc != NULL)
        ui->checkBoxShowNormals->setChecked(m_doc->useNormalCloud());
}

void GPointsAttributesManager::closeEvent(QCloseEvent *e)
{
    checkAndSave(dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected()));

    QDialog::closeEvent(e);
}

void GPointsAttributesManager::initTreeView()
{
    typedef DM_AttributesScalarT<CT_AbstractPointsAttributes>    PointAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractPointsAttributes>     PointAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractPointsAttributes>    PointAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractFaceAttributes>      FaceAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractFaceAttributes>       FaceAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractFaceAttributes>      FaceAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractEdgeAttributes>      EdgeAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractEdgeAttributes>       EdgeAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractEdgeAttributes>      EdgeAttributesNormal;

    m_model.clear();
    constructHeader();

    m_itemPointRoot = new QStandardItem(tr("Point"));
    m_itemPointRoot->setEditable(false);

    m_model.appendRow(m_itemPointRoot);

    m_itemFaceRoot = new QStandardItem(tr("Face"));
    m_itemFaceRoot->setEditable(false);

    m_model.appendRow(m_itemFaceRoot);

    m_itemEdgeRoot = new QStandardItem(tr("Edge"));
    m_itemEdgeRoot->setEditable(false);

    m_model.appendRow(m_itemEdgeRoot);

    QList<CT_VirtualAbstractStep*> steps = GUI_MANAGER->getStepManager()->getStepRootList();

    while(!steps.isEmpty())
    {
        CT_VirtualAbstractStep *st = steps.takeFirst();
        buildTreeViewTForStep< CT_AbstractPointsAttributes, PointAttributesScalar, PointAttributesColor, PointAttributesNormal >(st);
        buildTreeViewTForStep< CT_AbstractFaceAttributes, FaceAttributesScalar, FaceAttributesColor, FaceAttributesNormal >(st);
        buildTreeViewTForStep< CT_AbstractEdgeAttributes, EdgeAttributesScalar, EdgeAttributesColor, EdgeAttributesNormal >(st);
    }
}

template<typename IAttributesType, typename AttributesScalarType, typename AttributesColorType, typename AttributesNormalType>
void GPointsAttributesManager::buildTreeViewTForStep(CT_VirtualAbstractStep *step)
{
    DM_AttributesBuildingCollectionT<IAttributesType> builderPoints;

    builderPoints.buildFrom(step);

    QListIterator<IAttributesType*> it(builderPoints.attributesCollection());

    while(it.hasNext())
    {
        IAttributesType *pa = it.next();

        CT_AbstractAttributesScalar *pas = dynamic_cast<CT_AbstractAttributesScalar*>(pa);

        if(pas != NULL)
        {
            AttributesScalarType *dpas = (AttributesScalarType*)m_manager->getAttributesFromInterface(pa);

            if(dpas == NULL)
            {
                dpas = new AttributesScalarType();
                dpas->setTypeAttributes(pa, pas);

                m_manager->addAttributes(dpas);
            }

            addToScalarRoot<IAttributesType>(createAttributesScalarForModel(dpas));
        }
        else
        {
            CT_AttributesColor *pac = dynamic_cast<CT_AttributesColor*>(pa);

            if(pac != NULL)
            {
                AttributesColorType *dpac = dynamic_cast<AttributesColorType*>(m_manager->getAttributesFromInterface(pa));

                if(dpac == NULL)
                {
                    dpac = new AttributesColorType();
                    dpac->setTypeAttributes(pa, pac);

                    m_manager->addAttributes(dpac);
                }

                addToColorRoot<IAttributesType>(createAttributesColorForModel(dpac));
            }
            else
            {
                CT_AttributesNormal *pan = dynamic_cast<CT_AttributesNormal*>(pa);

                if(pan != NULL)
                {
                    AttributesNormalType *dpan = dynamic_cast<AttributesNormalType*>(m_manager->getAttributesFromInterface(pa));

                    if(dpan == NULL)
                    {
                        dpan = new AttributesNormalType();
                        dpan->setTypeAttributes(pa, pan);

                        m_manager->addAttributes(dpan);
                    }

                    addToNormalRoot<IAttributesType>(createAttributesNormalForModel(dpan));
                }
            }
        }
    }
}

void GPointsAttributesManager::constructHeader()
{
    QStringList header;
    header << tr("Nom");
    header << tr("Appliquer");
    //header << tr("Utiliser seul ?");
    header << tr("Utiliser gradient partagé ?");
    header << tr("Configurer");

    m_model.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
#else
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
#endif
}

void GPointsAttributesManager::checkAndSave(DM_AbstractAttributesScalar *pas)
{
    if((pas != NULL)
            && (ui->pushButtonSave->isEnabled()))
    {
        int ret = QMessageBox::question(this, tr("Enregistrer"), tr("Vous n'avez pas enregistré le gradient. Voulez vous le faire maintenant ?"), QMessageBox::Yes | QMessageBox::No);

        if(ret == QMessageBox::Yes)
            saveCurrentGradientTo(pas);
    }
}

QList<QStandardItem*> GPointsAttributesManager::createAttributesScalarForModel(DM_AbstractAttributesScalar *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(qVariantFromValue((void*)pa));
    items << item;*/

    // CHECKBOX "UTILISER GRADIENT PARTAGE ?"
    item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isUsedSharedGradient() ? Qt::Checked : Qt::Unchecked);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // BOUTON CONFIGURER
    item = new QStandardItem(tr("Configurer"));
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    return items;
}

QList<QStandardItem*> GPointsAttributesManager::createAttributesColorForModel(DM_AbstractAttributesColor *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(qVariantFromValue((void*)pa));
    items << item;*/

    return items;
}

QList<QStandardItem *> GPointsAttributesManager::createAttributesNormalForModel(DM_AbstractAttributesNormal *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(qVariantFromValue((void*)pa));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(qVariantFromValue((void*)pa));
    items << item;*/

    return items;
}

template<>
void GPointsAttributesManager::addToScalarRoot<CT_AbstractPointsAttributes>(const QList<QStandardItem*> &items)
{
    addToScalarRoot(items, m_itemPointRootScalar, m_itemPointRoot);
}

template<>
void GPointsAttributesManager::addToScalarRoot<CT_AbstractFaceAttributes>(const QList<QStandardItem*> &items)
{
    addToScalarRoot(items, m_itemFaceRootScalar, m_itemFaceRoot);
}

template<>
void GPointsAttributesManager::addToScalarRoot<CT_AbstractEdgeAttributes>(const QList<QStandardItem*> &items)
{
    addToScalarRoot(items, m_itemEdgeRootScalar, m_itemEdgeRoot);
}

void GPointsAttributesManager::addToScalarRoot(const QList<QStandardItem *> &items, QStandardItem *&root, QStandardItem *model)
{
    if(root == NULL)
    {
        root = new QStandardItem(tr("Gradients"));
        root->setEditable(false);

        model->appendRow(root);
    }

    root->appendRow(items);
    ui->treeView->expand(m_model.indexFromItem(model));
    ui->treeView->expand(m_model.indexFromItem(root));

    createWidgetForItems(items);
}

template<>
void GPointsAttributesManager::addToColorRoot<CT_AbstractPointsAttributes>(const QList<QStandardItem*> &items)
{
    addToColorRoot(items, m_itemPointRootColor, m_itemPointRoot);
}

template<>
void GPointsAttributesManager::addToColorRoot<CT_AbstractFaceAttributes>(const QList<QStandardItem*> &items)
{
    addToColorRoot(items, m_itemFaceRootColor, m_itemFaceRoot);
}

template<>
void GPointsAttributesManager::addToColorRoot<CT_AbstractEdgeAttributes>(const QList<QStandardItem*> &items)
{
    addToColorRoot(items, m_itemEdgeRootColor, m_itemEdgeRoot);
}

void GPointsAttributesManager::addToColorRoot(const QList<QStandardItem *> &items, QStandardItem *&root, QStandardItem *model)
{
    if(root == NULL)
    {
        root = new QStandardItem(tr("Couleurs"));
        root->setEditable(false);

        model->appendRow(root);
    }

    root->appendRow(items);
    ui->treeView->expand(m_model.indexFromItem(model));
    ui->treeView->expand(m_model.indexFromItem(root));

    createWidgetForItems(items);
}

template<>
void GPointsAttributesManager::addToNormalRoot<CT_AbstractPointsAttributes>(const QList<QStandardItem*> &items)
{
    addToNormalRoot(items, m_itemPointRootNormal, m_itemPointRoot);
}

template<>
void GPointsAttributesManager::addToNormalRoot<CT_AbstractFaceAttributes>(const QList<QStandardItem*> &items)
{
    addToNormalRoot(items, m_itemFaceRootNormal, m_itemFaceRoot);
}

template<>
void GPointsAttributesManager::addToNormalRoot<CT_AbstractEdgeAttributes>(const QList<QStandardItem*> &items)
{
    addToNormalRoot(items, m_itemEdgeRootNormal, m_itemEdgeRoot);
}

void GPointsAttributesManager::addToNormalRoot(const QList<QStandardItem *> &items, QStandardItem *&root, QStandardItem *model)
{
    if(root == NULL)
    {
        root = new QStandardItem(tr("Normales"));
        root->setEditable(false);

        model->appendRow(root);
    }

    root->appendRow(items);
    ui->treeView->expand(m_model.indexFromItem(model));
    ui->treeView->expand(m_model.indexFromItem(root));

    createWidgetForItems(items);
}

void GPointsAttributesManager::createWidgetForItems(const QList<QStandardItem *> &items)
{
    QStandardItem *item = items.at(1);
    QPushButton *pushButton = new QPushButton(item->text(), ui->treeView);
    pushButton->setProperty("userdata", item->data());

    ui->treeView->setIndexWidget(m_model.indexFromItem(item), pushButton);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonApplyClicked()));

    if(items.size() > 3)
    {
        item = items.at(3);
        QPushButton *pushButtonC = new QPushButton(item->text(), ui->treeView);
        pushButtonC->setProperty("userdata", item->data());

        ui->treeView->setIndexWidget(m_model.indexFromItem(item), pushButtonC);

        connect(pushButtonC, SIGNAL(clicked()), this, SLOT(pushButtonConfigureClicked()));
    }
}

void GPointsAttributesManager::editAttributesScalar(DM_AbstractAttributesScalar *pas)
{
    QGradientStops stops = pas->gradient().stops();

    QVectorIterator<QGradientStop> it(stops);

    int i = 0;

    while(it.hasNext())
    {
        QGradientStop stop = it.next();

        GradientArrow arrow;
        arrow.setHasFocus(i==0);
        arrow.setColor(stop.second);
        arrow.setPosition(stop.first);
        arrow.setIndex(i);

        ui->colorGradientView->changeArrow(arrow);
        ++i;
    }

    ui->pushButtonAddColor->setEnabled(true);
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->widgetEditGradient->setVisible(true);
}

void GPointsAttributesManager::clearEditGradient()
{
    ui->colorGradientView->clearArrows();
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonDeleteColor->setEnabled(false);
    ui->pushButtonAddColor->setEnabled(false);
}

void GPointsAttributesManager::saveCurrentGradientTo(DM_AbstractAttributesScalar *pas)
{
    QList<GradientArrow> arrows = ui->colorGradientView->orderedArrows();

    if(arrows.size() > 1)
    {
        QLinearGradient gradient;
        QGradientStops stops;

        QListIterator<GradientArrow> it(arrows);

        while(it.hasNext())
        {
            const GradientArrow &gr = it.next();

            stops.append(QGradientStop(gr.position(), gr.color()));
        }

        gradient.setStops(stops);
        pas->setGradient(gradient);

        // si il utilise un gradient partagé
        if(pas->isUsedSharedGradient())
        {
            // on applique le gradient à tous ceux qui utilise un gradient partagé
            QListIterator<DM_AbstractAttributes*> it(m_manager->attributes());

            while(it.hasNext())
            {
                DM_AbstractAttributesScalar *pass = dynamic_cast<DM_AbstractAttributesScalar*>(it.next());

                if((pass != NULL)
                        && (pass != pas)
                        && (pass->isUsedSharedGradient()))
                {
                    pass->setGradient(gradient);
                }
            }
        }
    }

    ui->pushButtonSave->setEnabled(false);
}

void GPointsAttributesManager::editAttributesNormal(DM_AbstractAttributesNormal *pan)
{
    Q_UNUSED(pan)

    GDocumentViewForGraphics::NormalsConfiguration c = m_doc->getNormalsConfiguration();

    ui->pushButtonNormalsColorPicker->setCurrentColor(c.normalColor);
    ui->doubleSpinBoxNormalsLength->setValue(c.normalLength);

    ui->widgetEditNormals->setVisible(true);
}

void GPointsAttributesManager::clearEditNormal()
{
    ui->pushButtonApplyEditNormals->setEnabled(false);
}

void GPointsAttributesManager::applyAndSaveNormal()
{
    GDocumentViewForGraphics::NormalsConfiguration c;
    c.normalColor = ui->pushButtonNormalsColorPicker->currentColor();
    c.normalLength = ui->doubleSpinBoxNormalsLength->value();

    m_doc->applyNormalsConfiguration(c);
}

DM_AbstractAttributes *GPointsAttributesManager::attributesSelected() const
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();

    if(list.isEmpty())
        return NULL;

    return (DM_AbstractAttributes*)m_model.itemFromIndex(list.first())->data().value<void*>();
}

void GPointsAttributesManager::on_pushButtonAddColor_clicked()
{
    GradientArrow newArrow;
    newArrow.setIndex(ui->colorGradientView->indexUnused());
    newArrow.setHasFocus(true);
    newArrow.setPosition(ui->colorGradientView->positionUnused());
    newArrow.setColor(Qt::white);

    ui->colorGradientView->changeArrow(newArrow);
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_pushButtonDeleteColor_clicked()
{
    ui->colorGradientView->removeArrow(ui->colorGradientView->currentArrow().index());
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_pushButtonSave_clicked()
{
    saveCurrentGradientTo(dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected()));
}

void GPointsAttributesManager::pushButtonApplyClicked()
{
    QPushButton *pushButton = (QPushButton*)sender();
    DM_AbstractAttributes *dpa = (DM_AbstractAttributes*)pushButton->property("userdata").value<void*>();

    DM_AbstractAttributesScalar *as = dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected());

    if(dpa == as)
        saveCurrentGradientTo((DM_AbstractAttributesScalar*)dpa);

    m_doc->applyAttributes(dpa);
}

void GPointsAttributesManager::pushButtonConfigureClicked()
{
    QPushButton *pushButton = (QPushButton*)sender();
    DM_AbstractAttributes *dpa = (DM_AbstractAttributes*)pushButton->property("userdata").value<void*>();
    DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(dpa);

    if(pas != NULL)
    {
        GMinMaxAttributesScalarConfiguration dialog(this);
        dialog.setAttributes(pas);

        dialog.exec();
    }
}

void GPointsAttributesManager::itemChanged(QStandardItem *item)
{
    DM_AbstractAttributes *pa = (DM_AbstractAttributes*)item->data().value<void*>();

    if(pa != NULL)
    {
        // use shared gradient
        if(item->column() == 2)
        {
            DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(pa);

            if(pas != NULL)
            {
                pas->setUseSharedGradient(item->checkState() == Qt::Checked);

                if(pas->isUsedSharedGradient())
                {
                    // recherche d'un autre qui utilise le gradient partagé
                    QListIterator<DM_AbstractAttributes*> it(m_manager->attributes());

                    while(it.hasNext())
                    {
                        DM_AbstractAttributesScalar *pass = dynamic_cast<DM_AbstractAttributesScalar*>(it.next());

                        if((pass != NULL)
                                && (pass != pas)
                                && (pass->isUsedSharedGradient()))
                        {
                            // on applique le gradient partagé a l'attribut de points courant
                            pas->setGradient(pass->gradient());

                            if(attributesSelected() == pa)
                                editAttributesScalar(pas);

                            return;
                        }
                    }
                }
            }
        }
    }
}

void GPointsAttributesManager::on_pushButtonGradientColorPicker_colorChanged(const QColor &color)
{
    if(!m_internalSetColor)
    {
        GradientArrow arrow = ui->colorGradientView->currentArrow();
        arrow.setColor(color);

        ui->colorGradientView->changeArrow(arrow);
        ui->pushButtonSave->setEnabled(true);
    }

    m_internalSetColor = false;
}

void GPointsAttributesManager::on_colorGradientView_newFocusColor(const QColor &color, int arrowIndex)
{
    Q_UNUSED(arrowIndex)

    if(ui->pushButtonGradientColorPicker->currentColor() != color)
    {
        m_internalSetColor = true;
        ui->pushButtonGradientColorPicker->setCurrentColor(color);
    }
}

void GPointsAttributesManager::on_colorGradientView_arrowMove(qreal lastPos, const GradientArrow &arrow)
{
    Q_UNUSED(lastPos)
    Q_UNUSED(arrow)

    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_checkBoxShowNormals_stateChanged(int state)
{
    m_doc->setUseNormalCloud(state == Qt::Checked);
}

void GPointsAttributesManager::on_doubleSpinBoxNormalsLength_valueChanged(double v)
{
    if(m_doc != NULL) {
        GDocumentViewForGraphics::NormalsConfiguration c = m_doc->getNormalsConfiguration();

        if(v != c.normalLength)
            ui->pushButtonApplyEditNormals->setEnabled(true);
    }
}

void GPointsAttributesManager::on_pushButtonNormalsColorPicker_colorChanged(QColor c)
{
    if(m_doc != NULL) {
        GDocumentViewForGraphics::NormalsConfiguration co = m_doc->getNormalsConfiguration();

        if(c != co.normalColor)
            ui->pushButtonApplyEditNormals->setEnabled(true);
    }
}

void GPointsAttributesManager::on_pushButtonApplyEditNormals_clicked()
{
    ui->pushButtonApplyEditNormals->setEnabled(false);

    applyAndSaveNormal();
}

void GPointsAttributesManager::treeView_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QStandardItem *pItem = m_model.itemFromIndex(previous);

    if((pItem != NULL)
            && !pItem->data().isNull())
    {
        DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>((DM_AbstractAttributes*)pItem->data().value<void*>());

        checkAndSave(pas);
    }

    clearEditGradient();
    ui->widgetEditGradient->setVisible(false);
    ui->widgetEditNormals->setVisible(false);

    QStandardItem *cItem = m_model.itemFromIndex(current);

    if((cItem != NULL)
            && !cItem->data().isNull())
    {
        DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>((DM_AbstractAttributes*)cItem->data().value<void*>());

        if(pas != NULL)
        {
            editAttributesScalar(pas);
            return;
        }

        DM_AbstractAttributesNormal *pan = dynamic_cast<DM_AbstractAttributesNormal*>((DM_AbstractAttributes*)cItem->data().value<void*>());

        if(pan != NULL)
        {
            editAttributesNormal(pan);
            return;
        }
    }
}
