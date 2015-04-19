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

#include "ct_iterator/ct_pointiterator.h"

#include <math.h>
#include <limits>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

// Alias for indexing models
#define DEFin_mntres "mntres"
#define DEFin_mntgrp "mntgrp"
#define DEFin_mnt "mnt"

#define DEFin_scres "scres"
#define DEFin_scBase "scBase"
#define DEFin_layer "layer"
#define DEFin_cluster "cluster"
#define DEFin_circle "circle"
#define DEFin_scene "scene"
#define DEFin_positions "positions"

// Constructor : initialization of parameters
PB_StepManualInventory::PB_StepManualInventory(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);

    _paramFileName.append("../ComputreeHowTo/param_inv.txt");
    _deltaH = 0.15;
    _maxCircleDist = 2;
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
    resIn_mntres->addItemModel(DEFin_mntgrp, DEFin_mnt, CT_Grid2DXY<double>::staticGetType(), tr("MNT"));

    CT_InResultModelGroupToCopy *resIn_scres = createNewInResultModelForCopy(DEFin_scres, tr("Scènes"));
    resIn_scres->setZeroOrMoreRootGroup();
    resIn_scres->addGroupModel("", DEFin_scBase, CT_AbstractItemGroup::staticGetType(), tr("Groupe de base"));
    resIn_scres->addGroupModel(DEFin_scBase, DEFin_layer, CT_AbstractItemGroup::staticGetType(), tr("Niveau Z"));
    resIn_scres->addGroupModel(DEFin_layer, DEFin_cluster, CT_AbstractItemGroup::staticGetType(), tr("Cluster"));
    resIn_scres->addItemModel(DEFin_cluster, DEFin_circle, CT_Circle::staticGetType(), tr("Cercle"));
    resIn_scres->addItemModel(DEFin_scBase, DEFin_scene, CT_Scene::staticGetType(), tr("Scène"));
    resIn_scres->addItemModel(DEFin_scBase, DEFin_positions, CT_Point2D::staticGetType(), tr("Position2D"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepManualInventory::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice("Fichier de paramétrage", CT_FileChoiceButton::OneExistingFile, "Fichier ascii (*.txt)", _paramFileName);
    configDialog->addDouble(tr("Ne pas accepter de cercle plus loin que :"), "m", 0, 99999, 2, _maxCircleDist);
    configDialog->addDouble(tr("Choisir préférenciellement le diamètre à + ou - :"), "cm", 0, 10000, 0, _deltaH, 100);
}


// Creation and affiliation of OUT models
void PB_StepManualInventory::createOutResultModelListProtected()
{
    // Création de la liste des attributs supplémentaires
    if (_paramFileName.size() > 0)
    {
        QFile f(_paramFileName.first());

        if (f.exists() && f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);
            QString paramString = stream.readAll();

            QStringList paramList = paramString.split("\n", QString::SkipEmptyParts);
            for (int i = 0 ; i  < paramList.size() ; i++)
            {
                QStringList values = paramList.at(i).split(";");

                if (values.size() > 0)
                {
                    QStringList modalities = values.mid(1);
                    _paramData.insert(values.at(0), modalities);
                    _paramAutoRename.insert(values.at(0), CT_AutoRenameModels());
                }
            }
        }
    }

    CT_OutResultModelGroupToCopyPossibilities *resCpy_scres = createNewOutResultModelToCopy(DEFin_scres);

    resCpy_scres->addItemModel(DEFin_scBase, _dbhcircle_ModelName, new CT_Circle(), tr("Cercle du DHP"));
    resCpy_scres->addItemModel(DEFin_scBase, _attributes_ModelName, new CT_AttributesList(), tr("Attributs"));

    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_dbh_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_NUMBER),
                                        tr("DHP (cm)"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_x_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_X),
                                        tr("X"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_y_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_Y),
                                        tr("Y"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_z_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_Z),
                                        tr("Z"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_h130_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_HEIGHT),
                                        tr("H130"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_h_ModelName,
                                        new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_HEIGHT),
                                        tr("Hauteur"));

    // Ajout des modèles pour les attributs supplémentaires
    QMutableMapIterator<QString, CT_AutoRenameModels> itAutoRename(_paramAutoRename);
    while (itAutoRename.hasNext())
    {
        itAutoRename.next();

        const QString &name = itAutoRename.key();
        CT_AutoRenameModels &autoRenameModel =  itAutoRename.value();

        resCpy_scres->addItemAttributeModel(_attributes_ModelName, autoRenameModel,
                                            new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE), name);
    }

}



void PB_StepManualInventory::compute()
{
    m_doc = NULL;
    m_status = 0;

    double hminLightGray = 1.3 - _deltaH;
    double hmaxLightGray = 1.3 + _deltaH;

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resCpy_scres = outResultList.at(0);



    // Récupération du MNT
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_mntres = inResultList.at(0);

    //CT_ResultGroup* resCpy_scres = inResultList.at(1);

    CT_ResultItemIterator itIn_mntgrp(resIn_mntres, this, DEFin_mnt);
    if (itIn_mntgrp.hasNext())
    {
        _itemIn_mnt = (CT_Grid2DXY<double>*)itIn_mntgrp.next();
    }

    if (_itemIn_mnt != NULL)
    {
        _selectedDbh = new QMap<const CT_Scene*, const CT_Circle*>();
        _availableDbh = new QMap<const CT_Scene*, QMultiMap<double, const CT_Circle*> >();
        _suppAttributes = new QMap<const CT_Scene*, QMap<QString, QString> >();
        _preferredDbh = new QList<const CT_Circle*>();
        _sceneDTMValues = new QMap<const CT_Scene*, double>();
        _trashedScenes = new QList<const CT_Scene*>();

        // Boucle sur les groupes contenant ls scènes
        CT_ResultGroupIterator itCpy_scBase(resCpy_scres, this, DEFin_scBase);
        while (itCpy_scBase.hasNext() && !isStopped())
        {
            CT_StandardItemGroup* grpCpy_scBase = (CT_StandardItemGroup*) itCpy_scBase.next();
            const CT_Scene* itemCpy_scene = (const CT_Scene*)grpCpy_scBase->firstItemByINModelName(this, DEFin_scene);
            const CT_Point2D* itemCpy_position = (const CT_Point2D*)grpCpy_scBase->firstItemByINModelName(this, DEFin_positions);

            if (itemCpy_scene != NULL && itemCpy_position != NULL)
            {
                _positions.insert(itemCpy_scene, itemCpy_position);
                
                // Initialisation des attributs supplémentaires
                QMap<QString, QString> &map = _suppAttributes->insert(itemCpy_scene, QMap<QString, QString>()).value();
                QMapIterator<QString, QStringList> itPar(_paramData);
                while (itPar.hasNext())
                {
                    itPar.next();
                    map.insert(itPar.key(), QString());
                }

                double x = itemCpy_position->getCenterX(); // Use Position coordinates for Height reference
                double y = itemCpy_position->getCenterY();
                double mntZ = _itemIn_mnt->valueAtXY(x, y);

                _sceneDTMValues->insert(itemCpy_scene, mntZ);

                QMultiMap<double, const CT_Circle*> &circleList = _availableDbh->insert(itemCpy_scene, QMultiMap<double, const CT_Circle*>()).value();

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
                            double dist = sqrt(pow(x - itemCpy_circle->getCenterX(), 2) + pow(y - itemCpy_circle->getCenterY(), 2));

                            if (dist <= _maxCircleDist)
                            {
                                circleList.insertMulti(itemCpy_circle->getCenterZ() - mntZ, itemCpy_circle);

                                // Liste des cercles préférrés
                                double height = itemCpy_circle->getCenterZ() - mntZ;
                                if (height >= hminLightGray && height <= hmaxLightGray)
                                {
                                    _preferredDbh->append(itemCpy_circle);
                                }
                            }
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
            const CT_Point2D* itemCpy_position = (const CT_Point2D*)grpCpy_scBase->firstItemByINModelName(this, DEFin_positions);

            if (itemCpy_scene != NULL  && itemCpy_position != NULL && !_trashedScenes->contains(itemCpy_scene))
            {
                CT_Circle* bestCircle = (CT_Circle*) _selectedDbh->value(itemCpy_scene, NULL);

                if (bestCircle != NULL)
                {
                    CT_Circle* itemCpy_dbhcircle = (CT_Circle*) bestCircle->copy(_dbhcircle_ModelName.completeName(), resCpy_scres, CT_ResultCopyModeList() << CT_ResultCopyModeList::CopyItemDrawableReference);
                    grpCpy_scBase->addItemDrawable(itemCpy_dbhcircle);

                    double dbh = bestCircle->getRadius() * 200.0;
                    double x = itemCpy_position->getCenterX(); // Use Position coordinates for Height reference
                    double y = itemCpy_position->getCenterY();
                    double mntZ = _itemIn_mnt->valueAtXY(x, y);
                    double height = computeMaxZ(itemCpy_scene) - mntZ;

                    if ((height < 0) || (mntZ == _itemIn_mnt->NA())) {height = 0;}

                    CT_AttributesList* itemCpy_attributes = new CT_AttributesList(_attributes_ModelName.completeName(), resCpy_scres);
                    grpCpy_scBase->addItemDrawable(itemCpy_attributes);

                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_dbh_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_NUMBER,
                                                                                         resCpy_scres, dbh));
                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_x_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_X,
                                                                                         resCpy_scres, bestCircle->getCenterX()));
                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_y_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_Y,
                                                                                         resCpy_scres, bestCircle->getCenterY()));
                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_z_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_Z,
                                                                                         resCpy_scres, bestCircle->getCenterZ()));
                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_h130_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_HEIGHT,
                                                                                         resCpy_scres, bestCircle->getCenterZ() - mntZ));
                    itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<double>(_attribute_h_ModelName.completeName(),
                                                                                         CT_AbstractCategory::DATA_HEIGHT,
                                                                                         resCpy_scres, height));


                    // Initialisation des attributs supplémentaires
                    const QMap<QString, QString> &map = _suppAttributes->value(itemCpy_scene, QMap<QString, QString>());
                    QMutableMapIterator<QString, CT_AutoRenameModels> itmap(_paramAutoRename);
                    while (itmap.hasNext())
                    {
                        itmap.next();

                        const QString &name = itmap.key();
                        const QString &value = map.value(name, "");
                        CT_AutoRenameModels &autoRename = itmap.value();

                        itemCpy_attributes->addItemAttribute(new CT_StdItemAttributeT<QString>(autoRename.completeName(),
                                                                                               CT_AbstractCategory::DATA_VALUE,
                                                                                               resCpy_scres, value));
                    }
                }
            }
        }


        m_status = 1;
        requestManualMode();

        delete _selectedDbh;
        delete _availableDbh;
        delete _suppAttributes;
        delete _preferredDbh;
        delete _sceneDTMValues;
        delete _trashedScenes;
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

    m_doc->setCurrentAction(new PB_ActionManualInventory(_selectedDbh, _availableDbh, _preferredDbh, _trashedScenes, _sceneDTMValues, &_paramData, _suppAttributes));


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
    QMapIterator<const CT_Scene*, const CT_Point2D*>  itScenes(_positions);
    while (itScenes.hasNext())
    {
        itScenes.next();
        const CT_Scene* scene = itScenes.key();
        const CT_Point2D* pos = itScenes.value();
        double x = pos->getCenterX();
        double y = pos->getCenterY();
        double z = _itemIn_mnt->valueAtXY(x, y) + 1.3;
        
        QList<const CT_Circle*> circles = (_availableDbh->value(scene)).values();

        double mindelta = std::numeric_limits<double>::max();
        const CT_Circle* bestCircle = NULL;

        QListIterator<const CT_Circle*> itCircles(circles);
        while (itCircles.hasNext())
        {
            const CT_Circle* currentCircle = itCircles.next();
            double dist = std::fabs(currentCircle->getCenterZ() - z);

            if (dist < mindelta)
            {
                mindelta = dist;
                bestCircle = currentCircle;
            }
        }
        if (bestCircle == NULL)
        {

            CT_Circle* tmpCircle = new CT_Circle(NULL, NULL, new CT_CircleData(Eigen::Vector3d(x, y, z), Eigen::Vector3d(0, 0, 1), 0.05, -9999));
            _selectedDbh->insert(scene, tmpCircle);
            _temporaryCircles.append(tmpCircle);
        } else
        {
            _selectedDbh->insert(scene, bestCircle);
        }
    }
}

double PB_StepManualInventory::computeMaxZ(const CT_Scene* scene)
{
    CT_PointIterator it(scene->getPointCloudIndex());

    double zmax = -std::numeric_limits<double>::max();

    while(it.hasNext())
    {
        const CT_Point &point = it.next().currentPoint();

        if (point(2) > zmax)
            zmax = point(2);
    }

    return zmax;
}
