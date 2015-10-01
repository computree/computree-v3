#include "pb_stepvalidateinventory.h"

#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "actions/pb_actionvalidateinventory.h"

#include <limits>
#include <QMessageBox>

// Alias for indexing models
#define DEFin_scres "scres"
#define DEFin_scBase "scBase"
#define DEFin_group "group"
#define DEFin_item "item"
#define DEFin_dtmValue "dtmValue"

// Constructor : initialization of parameters
PB_StepValidateInventory::PB_StepValidateInventory(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    m_doc = NULL;
    setManual(true);

    _speciesList << "Erable" << "Hêtre" << "Chêne" << "Sapin Baumier" << "Epinette Noire";
    _hRef = 1.3;
}

// Step description (tooltip of contextual menu)
QString PB_StepValidateInventory::getStepDescription() const
{
    return tr("Validation d'inventaire");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepValidateInventory::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepValidateInventory(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepValidateInventory::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_scres = createNewInResultModelForCopy(DEFin_scres, tr("Items"));
    resIn_scres->setZeroOrMoreRootGroup();
    resIn_scres->addGroupModel("", DEFin_scBase, CT_AbstractItemGroup::staticGetType(), tr("Groupe de base"));
    resIn_scres->addItemModel(DEFin_scBase, DEFin_dtmValue, CT_ReferencePoint::staticGetType(), tr("Z MNT"));
    resIn_scres->addGroupModel(DEFin_scBase, DEFin_group, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_scres->addItemModel(DEFin_group, DEFin_item, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
}

// Creation and affiliation of OUT models
void PB_StepValidateInventory::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_scres = createNewOutResultModelToCopy(DEFin_scres);

    resCpy_scres->addItemModel(DEFin_scBase, _attributes_ModelName, new CT_ReferencePoint(), tr("Position de référence"));

    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_sp_ModelName,
                                        new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE),
                                        tr("Espèce"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_id_ModelName,
                                        new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID),
                                        tr("IDterrain"));
    resCpy_scres->addItemAttributeModel(_attributes_ModelName,_attribute_idItem_ModelName,
                                        new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID),
                                        tr("IDitem"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepValidateInventory::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();
    configDialog->addFileChoice(tr("Fichier d'espèces"), CT_FileChoiceButton::OneExistingFile, "Fichier ascii (*.txt)", _speciesFileName);
    configDialog->addDouble(tr("Hauteur de référence"), "m", 0, 9999, 2, _hRef);
}

void PB_StepValidateInventory::compute()
{
    m_doc = NULL;
    m_status = 0;

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resCpy_scres = outResultList.at(0);

        _selectedItem = new QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>();
        _availableItem = new QMultiMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>();
        _species = new QMap<const CT_StandardItemGroup*, QString>();
        _ids = new QMap<const CT_StandardItemGroup*, QString>();

        // Boucle sur les groupes racine
        CT_ResultGroupIterator itCpy_scBase(resCpy_scres, this, DEFin_scBase);
        while (itCpy_scBase.hasNext() && !isStopped())
        {
            CT_StandardItemGroup* grpCpy_scBase = (CT_StandardItemGroup*) itCpy_scBase.next();
            const CT_ReferencePoint* itemCpy_zDTM = (const CT_ReferencePoint*) grpCpy_scBase->firstItemByINModelName(this, DEFin_dtmValue);

            if (itemCpy_zDTM != NULL)
            {
                _dtmZvalues.insert(grpCpy_scBase, itemCpy_zDTM->getCenterZ());
            } else {
                _dtmZvalues.insert(grpCpy_scBase, 0);
            }

            CT_GroupIterator itCpy_cluster(grpCpy_scBase, this, DEFin_group);
            while (itCpy_cluster.hasNext() && !isStopped())
            {
                CT_StandardItemGroup* grpCpy_cluster = (CT_StandardItemGroup*) itCpy_cluster.next();

                const CT_AbstractSingularItemDrawable* itemCpy_item = grpCpy_cluster->firstItemByINModelName(this, DEFin_item);
                if (itemCpy_item != NULL)
                {
                    _availableItem->insertMulti(grpCpy_scBase, itemCpy_item);
                }
            }
        }

        // remplit _selectedDbh
        findBestItemForEachGroup();

        // request the manual mode
        requestManualMode();


        CT_ResultGroupIterator itCpy_scBaseOut(resCpy_scres, this, DEFin_scBase);
        while (itCpy_scBaseOut.hasNext() && !isStopped())
        {
            CT_StandardItemGroup* grpCpy_scBase = (CT_StandardItemGroup*) itCpy_scBaseOut.next();
            CT_AbstractSingularItemDrawable* selectedItem = (CT_AbstractSingularItemDrawable*) _selectedItem->value(grpCpy_scBase, NULL);

            if (selectedItem != NULL)
            {

                float x = selectedItem->getCenterX();
                float y = selectedItem->getCenterY();
                float z = selectedItem->getCenterZ();

                QString species = _species->value(grpCpy_scBase, "");
                QString id = _ids->value(grpCpy_scBase, "");

                CT_ReferencePoint* itemCpy_refPosition = new CT_ReferencePoint(_attributes_ModelName.completeName(), resCpy_scres, x, y, z, 0);
                grpCpy_scBase->addItemDrawable(itemCpy_refPosition);

                itemCpy_refPosition->addItemAttribute(new CT_StdItemAttributeT<QString>(_attribute_sp_ModelName.completeName(),
                                                                                       CT_AbstractCategory::DATA_VALUE,
                                                                                       resCpy_scres, species));
                itemCpy_refPosition->addItemAttribute(new CT_StdItemAttributeT<QString>(_attribute_id_ModelName.completeName(),
                                                                                       CT_AbstractCategory::DATA_ID,
                                                                                       resCpy_scres, id));
                itemCpy_refPosition->addItemAttribute(new CT_StdItemAttributeT<QString>(_attribute_idItem_ModelName.completeName(),
                                                                                       CT_AbstractCategory::DATA_ID,
                                                                                       resCpy_scres, QString("%1").arg(selectedItem->id())));

            }
        }


        m_status = 1;
        requestManualMode();

        delete _selectedItem;
        delete _availableItem;
        delete _species;
        delete _ids;


}

void PB_StepValidateInventory::initManualMode()
{
    // create a new 3D document
    if(m_doc == NULL)
        m_doc = getGuiContext()->documentManager()->new3DDocument();

    m_doc->removeAllItemDrawable();

    m_doc->setCurrentAction(new PB_ActionValidateInventory(_selectedItem, _availableItem, _species, _ids, _speciesList));


    QMessageBox::information(NULL, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette étape !"), QMessageBox::Ok);
}

void PB_StepValidateInventory::useManualMode(bool quit)
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

void PB_StepValidateInventory::findBestItemForEachGroup()
{
    QList<const CT_StandardItemGroup*> groups = _availableItem->keys();

    QListIterator<const CT_StandardItemGroup*> itGroups(groups);
    while (itGroups.hasNext())
    {
        const CT_StandardItemGroup* group = itGroups.next();
        QList<const CT_AbstractSingularItemDrawable*> items = _availableItem->values(group);

        float mindelta = std::numeric_limits<float>::max();
        const CT_AbstractSingularItemDrawable* bestItem = NULL;

        QListIterator<const CT_AbstractSingularItemDrawable*> itItems(items);
        while (itItems.hasNext())
        {
            const CT_AbstractSingularItemDrawable* currentItem = itItems.next();
            float dist = std::fabs(currentItem->getCenterZ() - (_dtmZvalues.value(group, 0) + _hRef));

            if (dist < mindelta)
            {
                mindelta = dist;
                bestItem = currentItem;
            }
        }
        if (bestItem != NULL)
        {
            _selectedItem->insert(group, bestItem);
        }
    }
}


