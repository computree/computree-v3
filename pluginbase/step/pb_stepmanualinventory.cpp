#include "pb_stepmanualinventory.h"

#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "actions/pb_actionmanualinventory.h"

#include <limits>
#include <QMessageBox>

// Alias for indexing models
#define DEFin_mntres "mntres"
#define DEFin_mntgrp "mntgrp"
#define DEFin_mnt "mnt"

#define DEFin_scres "scres"
#define DEFin_scBase "scBase"
#define DEFin_layer "layer"
#define DEFin_cluster "cluster2"
#define DEFin_circle "circle"
#define DEFin_scene "scene"

// Constructor : initialization of parameters
PB_StepManualInventory::PB_StepManualInventory(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);

    _speciesList << "Erable" << "Hêtre" << "Chêne" << "Sapin Baumier" << "Epinette Noire";
}

// Step description (tooltip of contextual menu)
QString PB_StepManualInventory::getStepDescription() const
{
    return tr("Séléction d'un dbh / scène");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepManualInventory::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepManualInventory(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepManualInventory::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_mntres = createNewInResultModel(DEFin_mntres, tr("MNT"), "", true);
    resIn_mntres->setRootGroup(DEFin_mntgrp, CT_AbstractItemGroup::staticGetType(), tr("MNT"));
    resIn_mntres->addItemModel(DEFin_mntgrp, DEFin_mnt, CT_Grid2DXY<float>::staticGetType(), tr("MNT"));

    CT_InResultModelGroupToCopy *resIn_scres = createNewInResultModelForCopy(DEFin_scres, tr("Scènes"));
    resIn_scres->setZeroOrMoreRootGroup();
    resIn_scres->addGroupModel("", DEFin_scBase, CT_AbstractItemGroup::staticGetType(), tr("Groupe de base"));
    resIn_scres->addGroupModel(DEFin_scBase, DEFin_layer, CT_AbstractItemGroup::staticGetType(), tr("Niveau Z"));
    resIn_scres->addGroupModel(DEFin_layer, DEFin_cluster, CT_AbstractItemGroup::staticGetType(), tr("Cluster"));
    resIn_scres->addItemModel(DEFin_cluster, DEFin_circle, CT_Circle::staticGetType(), tr("Cercle"));
    resIn_scres->addItemModel(DEFin_scBase, DEFin_scene, CT_Scene::staticGetType(), tr("Scène"));
}

// Creation and affiliation of OUT models
void PB_StepManualInventory::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_scres = createNewOutResultModelToCopy(DEFin_scres);

    resCpy_scres->addItemModel(DEFin_scBase, _dbhcircle_ModelName, new CT_Circle(), tr("Cercle du DHP"));
    resCpy_scres->addItemModel(DEFin_scBase, _attributes_ModelName, new CT_AttributesList(), tr("Attributs"));

    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_dbh_ModelName,
                                        new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                        tr("DHP (cm)"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_x_ModelName,
                                        new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_X),
                                        tr("X"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_y_ModelName,
                                        new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_Y),
                                        tr("Y"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_h_ModelName,
                                        new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_HEIGHT),
                                        tr("Hauteur"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_sp_ModelName,
                                        new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE),
                                        tr("Espèce"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_id_ModelName,
                                        new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID),
                                        tr("IDterrain"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepManualInventory::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice("Fichier d'espèces", CT_FileChoiceButton::OneExistingFile, "Fichier ascii (*.txt)", _speciesFileName);
}

void PB_StepManualInventory::compute()
{
    m_doc = NULL;
    m_status = 0;

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resCpy_scres = outResultList.at(0);



    // Récupération du MNT
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_mntres = inResultList.at(0);

    //CT_ResultGroup* resCpy_scres = inResultList.at(1);

    CT_ResultItemIterator itIn_mntgrp(resIn_mntres, this, DEFin_mnt);
    if (itIn_mntgrp.hasNext())
    {
        _itemIn_mnt = (CT_Grid2DXY<float>*)itIn_mntgrp.next();
    }

    if (_itemIn_mnt != NULL)
    {
        _selectedDbh = new QMap<const CT_Scene*, const CT_Circle*>();
        _availableDbh = new QMultiMap<const CT_Scene*, const CT_Circle*>();
        _species = new QMap<const CT_Scene*, QString>();
        _ids = new QMap<const CT_Scene*, QString>();

        // Boucle sur les groupes contenant ls scènes
        CT_ResultGroupIterator itCpy_scBase(resCpy_scres, this, DEFin_scBase);
        while (itCpy_scBase.hasNext() && !isStopped())
        {
            CT_StandardItemGroup* grpCpy_scBase = (CT_StandardItemGroup*) itCpy_scBase.next();
            const CT_Scene* itemCpy_scene = (const CT_Scene*)grpCpy_scBase->firstItemByINModelName(this, DEFin_scene);

            if (itemCpy_scene != NULL)
            {

                CT_GroupIterator itCpy_layer(grpCpy_scBase, this, DEFin_layer);
                while (itCpy_layer.hasNext() && !isStopped())
                {
                    CT_StandardItemGroup* grpCpy_layer = (CT_StandardItemGroup*) itCpy_layer.next();

                    CT_GroupIterator itCpy_cluster(grpCpy_layer, this, DEFin_cluster);
                    while (itCpy_cluster.hasNext() && !isStopped())
                    {
                        CT_StandardItemGroup* grpCpy_cluster = (CT_StandardItemGroup*) itCpy_cluster.next();

                        const CT_Circle* itemCpy_circle = (CT_Circle*)grpCpy_cluster->firstItemByINModelName(this, DEFin_circle);
                        if (itemCpy_circle != NULL)
                        {
                            _availableDbh->insertMulti(itemCpy_scene, itemCpy_circle);
                        }
                    }
                }
            }
        }

        // remplit _selectedDbh
        findBestCircleForEachScene();

        // request the manual mode
        requestManualMode();


        CT_ResultGroupIterator itCpy_scBaseOut(resCpy_scres, this, DEFin_scBase);
        while (itCpy_scBaseOut.hasNext() && !isStopped())
        {
            CT_StandardItemGroup* grpCpy_scBase = (CT_StandardItemGroup*) itCpy_scBaseOut.next();
            const CT_Scene* itemCpy_scene = (const CT_Scene*)grpCpy_scBase->firstItemByINModelName(this, DEFin_scene);

            CT_Circle* bestCircle = (CT_Circle*) _selectedDbh->value(itemCpy_scene, NULL);

            if (bestCircle != NULL)
            {
                CT_Circle* itemCpy_dbhcircle = (CT_Circle*) bestCircle->copy(_dbhcircle_ModelName.completeName(), resCpy_scres, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableReference);
                grpCpy_scBase->addItemDrawable(itemCpy_dbhcircle);

                float dbh = bestCircle->getRadius() * 200.0;
                float x = bestCircle->getCenterX();
                float y = bestCircle->getCenterY();
                float height = computeMaxZ(itemCpy_scene) - _itemIn_mnt->value(x, y);
                if (height < 0) {height = 0;}

                QString species = _species->value(itemCpy_scene, "");
                QString id = _ids->value(itemCpy_scene, "");

                CT_AttributesList* itemCpy_attributes = new CT_AttributesList(_attributes_ModelName.completeName(), resCpy_scres);
                grpCpy_scBase->addItemDrawable(itemCpy_attributes);

                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<float>(_attribute_dbh_ModelName.completeName(),
                                                                                     CT_AbstractCategory::DATA_NUMBER,
                                                                                     resCpy_scres, dbh));
                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<float>(_attribute_x_ModelName.completeName(),
                                                                                     CT_AbstractCategory::DATA_X,
                                                                                     resCpy_scres, x));
                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<float>(_attribute_y_ModelName.completeName(),
                                                                                     CT_AbstractCategory::DATA_Y,
                                                                                     resCpy_scres, y));
                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<float>(_attribute_h_ModelName.completeName(),
                                                                                     CT_AbstractCategory::DATA_HEIGHT,
                                                                                     resCpy_scres, height));
                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<QString>(_attribute_sp_ModelName.completeName(),
                                                                                       CT_AbstractCategory::DATA_VALUE,
                                                                                       resCpy_scres, species));
                itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<QString>(_attribute_id_ModelName.completeName(),
                                                                                       CT_AbstractCategory::DATA_ID,
                                                                                       resCpy_scres, id));

            }

        }


        m_status = 1;
        requestManualMode();

        delete _selectedDbh;
        delete _availableDbh;
        delete _species;
        delete _ids;
        qDeleteAll(_temporaryCircles);
    }


}

void PB_StepManualInventory::initManualMode()
{
    if(m_doc == NULL)
    {

        QMap<QString, QVariant> param;
        param.insert("Transparency", QVariant(true));

        // create a new 3D document
        m_doc = getGuiContext()->documentManager()->new3DDocument(param);

    }
    m_doc->removeAllItemDrawable();

    m_doc->setCurrentAction(new PB_ActionManualInventory(_selectedDbh, _availableDbh, _species, _ids, _speciesList));


    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette étape !"), QMessageBox::Ok);
}

void PB_StepManualInventory::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            m_doc = NULL;
            quitManualMode();
        }
    }
}

void PB_StepManualInventory::findBestCircleForEachScene()
{
    QList<const CT_Scene*> scenes = _availableDbh->keys();

    QListIterator<const CT_Scene*> itScenes(scenes);
    while (itScenes.hasNext())
    {
        const CT_Scene* scene = itScenes.next();
        QList<const CT_Circle*> circles = _availableDbh->values(scene);

        float mindelta = std::numeric_limits<float>::max();
        const CT_Circle* bestCircle = NULL;

        QListIterator<const CT_Circle*> itCircles(circles);
        while (itCircles.hasNext())
        {
            const CT_Circle* currentCircle = itCircles.next();
            float dist = std::fabs(currentCircle->getCenterZ() - (_itemIn_mnt->valueAtXY(currentCircle->getCenterX(), currentCircle->getCenterY()) + 1.3));

            if (dist < mindelta)
            {
                mindelta = dist;
                bestCircle = currentCircle;
            }
        }
        if (bestCircle == NULL)
        {
            float x = scene->getCenterX();
            float y = scene->getCenterZ();
            float z = _itemIn_mnt->valueAtXY(x, y) + 1.3;

            CT_Circle* tmpCircle = new CT_Circle(NULL, NULL, new CT_CircleData(QVector3D(x, y, z), QVector3D(0, 0, 1), 0.05, -9999));
            _selectedDbh->insert(scene, tmpCircle);
            _temporaryCircles.append(tmpCircle);
        } else
        {
            _selectedDbh->insert(scene, bestCircle);
        }
    }
}

float PB_StepManualInventory::computeMaxZ(const CT_Scene* scene)
{
    const CT_AbstractPointCloudIndex *pointCloudIndex = scene->getPointCloudIndex();
    size_t n_points = pointCloudIndex->size();

    float zmax = -std::numeric_limits<float>::max();

    for (int i = 0 ; i < n_points; i++)
    {
        size_t index;
        const CT_Point &point = pointCloudIndex->constTAt(i, index);

        if (point(CT_Point::Z) > zmax)
        {
            zmax = point(CT_Point::Z);
        }
    }

    return zmax;
}
