#include "pb_stepexportattributesinloop.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"

#include "ct_itemdrawable/ct_profile.h"
#include "exporters/profile/pb_profileexporter.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_itemWithAttribute "itemWithAttribute"
#define DEFin_attribute "attribute"

#define DEFout_res "res"
#define DEFout_grp "grp"


// Constructor : initialization of parameters
PB_StepExportAttributesInLoop::PB_StepExportAttributesInLoop(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _fileNameBase = "export";
    _asciiExport = true;
    _binaryExport = true;
}

PB_StepExportAttributesInLoop::~PB_StepExportAttributesInLoop()
{
}


// Step description (tooltip of contextual menu)
QString PB_StepExportAttributesInLoop::getStepDescription() const
{
    return tr("Export d'attributs dans une boucle");
}

// Step detailled description
QString PB_StepExportAttributesInLoop::getStepDetailledDescription() const
{
    return tr("");
}

// Step URL
QString PB_StepExportAttributesInLoop::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepExportAttributesInLoop::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepExportAttributesInLoop(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepExportAttributesInLoop::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn->addItemModel(DEFin_grp, DEFin_itemWithAttribute, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple);
    resIn->addItemAttributeModel(DEFin_itemWithAttribute, DEFin_attribute, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple);
}

// Creation and affiliation of OUT models
void PB_StepExportAttributesInLoop::createOutResultModelListProtected()
{
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepExportAttributesInLoop::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = newStandardPostConfigurationDialog();

    configDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outFolder);
    configDialog->addString(tr("Nom de base pour le(s) fichier(s) d'export"), "", _fileNameBase);
    configDialog->addEmpty();
    configDialog->addBool(tr("Export ASCII (1 seul fichier)"), "", "", _asciiExport);
    configDialog->addEmpty();
    configDialog->addBool(tr("Export binaire (1 fichier par tour de boucle)"), "", "", _binaryExport);

}

void PB_StepExportAttributesInLoop::compute()
{
    bool firstTurn = true;
    _modelsKeys.clear();
    _names.clear();

    if (_fileNameBase.isEmpty()) {_fileNameBase = "export";}

    QFile* file = NULL;
    QTextStream* stream = NULL;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn = inResultList.at(0);

    CT_ModelSearchHelper::SplitHash hash = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_attribute, DEFin_itemWithAttribute, resIn->model(), this);

    if (firstTurn)
    {
        QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> itModels(hash);
        while (itModels.hasNext())
        {
            itModels.next();

            CT_OutAbstractSingularItemModel  *itemModel = itModels.key();
            CT_OutAbstractItemAttributeModel *attrModel = itModels.value();

            QString itemDN = itemModel->displayableName();
            QString itemUN = itemModel->uniqueName();

            QString attrDN = attrModel->displayableName();
            QString attrUN = attrModel->uniqueName();

            if (attrModel->isADefaultItemAttributeModel() && attrModel->originalModel() != NULL) {attrUN = attrModel->originalModel()->uniqueName();}

            QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);
            _modelsKeys.append(key);

            _names.insert(key, QString("%1_%2").arg(itemDN).arg(attrDN));
        }
        qSort(_modelsKeys.begin(), _modelsKeys.end());


        if (_asciiExport && _outFolder.size() > 0)
        {
            QFileInfo fileInfo(QString("%1/%2.txt").arg(_outFolder.first()).arg(_fileNameBase));
            if (fileInfo.exists())
            {
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Fichier d'export ASCII spécifié déjà existant. Arrêt des traitements."));
                stop();
                return;
            }

            file = new QFile(fileInfo.filePath());
            stream = new QTextStream(file);

            if (file->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    (*stream) << _names.value(_modelsKeys.at(i));
                    if (i < _modelsKeys.size() - 1) {(*stream) << "\t";} else {(*stream) << "\n";}
                }

            } else {
                delete stream; stream = NULL;
                delete file; file = NULL;
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Impossible de créer le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return;
            }
        }
    }

    // IN results browsing
    CT_ResultGroupIterator itIn_grp(resIn, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grp = (CT_AbstractItemGroup*) itIn_grp.next();

        QMap<QString, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> > indexedAttributes;

        CT_ItemIterator itItem(grp, this, DEFin_itemWithAttribute);
        while (itItem.hasNext())
        {
            CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itItem.next();

            if (item != NULL)
            {
                CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*)item->model();
                QList<CT_OutAbstractItemAttributeModel *> attributesModel = hash.values(itemModel);
                QList<CT_AbstractItemAttribute *> attributes = item->itemAttributes(attributesModel);

                for (int i = 0 ; i < attributes.size() ; i++)
                {
                    CT_AbstractItemAttribute* attribute = attributes.at(i);
                    CT_OutAbstractItemAttributeModel* attrModel = (CT_OutAbstractItemAttributeModel*) attribute->model();

                    QString attrUN = attrModel->uniqueName();
                    indexedAttributes.insert(QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrUN), QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*>(item, attribute));
                }
            }
        }

        for (int i = 0 ; i < _modelsKeys.size() ; i++)
        {

            const QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> &pair = indexedAttributes.value(_modelsKeys.at(i));
            if (pair.first != NULL && pair.second != NULL)
            {
                if (_asciiExport && stream != NULL)
                {
                    (*stream) << pair.second->toString(pair.first, NULL);
                }
            }

            if (_asciiExport && stream != NULL)
            {
                if(i < _modelsKeys.size() - 1) {(*stream) << "\t";} else {(*stream) << "\n";}
            }
        }
    }

    if (file != NULL) {file->close(); delete file;}
    if (stream != NULL) {delete stream;}

}
