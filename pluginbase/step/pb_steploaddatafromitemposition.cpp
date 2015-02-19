#include "pb_steploaddatafromitemposition.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/tools/iterator/ct_itemiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_itemdrawable/ct_datasourcegeo.h"

// Alias for indexing models
#define DEFin_resultItems "resultItems"
#define DEFin_grpItems "grpItems"
#define DEFin_item "item"

#define DEFin_resultDataSource "resultDataSource"
#define DEFin_grpDataSource "grpDataSource"
#define DEFin_dataSource "dataSource"

#define DEFout_result "result"
#define DEFout_grp "grp"


// Constructor : initialization of parameters
PB_StepLoadDataFromItemPosition::PB_StepLoadDataFromItemPosition(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _buffer = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepLoadDataFromItemPosition::getStepDescription() const
{
    return tr("Charge des fichiers d'un DataSource intersectant la BoudingBox d'items");
}

// Step detailled description
QString PB_StepLoadDataFromItemPosition::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepLoadDataFromItemPosition::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepLoadDataFromItemPosition::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadDataFromItemPosition(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepLoadDataFromItemPosition::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_dataSource = createNewInResultModel(DEFin_resultDataSource, tr("Source de données géographique"), "", true);
    resIn_dataSource->setZeroOrMoreRootGroup();
    resIn_dataSource->addGroupModel("", DEFin_grpDataSource, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_dataSource->addItemModel(DEFin_grpDataSource, DEFin_dataSource, CT_DataSourceGeo::staticGetType(), tr("Source de données géographique"));

    CT_InResultModelGroup *resIn_Items = createNewInResultModel(DEFin_resultItems, tr("Items"));
    resIn_Items->setZeroOrMoreRootGroup();
    resIn_Items->addGroupModel("", DEFin_grpItems, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_Items->addItemModel(DEFin_grpItems, DEFin_item, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));

}

// Creation and affiliation of OUT models
void PB_StepLoadDataFromItemPosition::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_result = createNewOutResultModel(DEFout_result, tr("Données chargées"));
    res_result->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));


//    QList<CT_ResultGroup*> inResultList = getInputResults();
//    CT_ResultGroup* resIn_dataSource = inResultList.at(0);

//    CT_ResultItemIterator itIn_dataSource(resIn_dataSource, this, DEFin_dataSource);
//    if (itIn_dataSource.hasNext())
//    {
//        const CT_DataSourceGeo *dataSource = (const CT_DataSourceGeo*) itIn_dataSource.next();

//        if (dataSource != NULL && dataSource->getNumberOfReader() > 0)
//        {
//            dataSource->init();
//            QSharedPointer<CT_AbstractReader> firstReader = dataSource->getActiveReader();

//            _itemModels.clear();
//            _groupModels.clear();

//            _itemModels.append(firstReader->outItemDrawableModels());
//            _groupModels.append(firstReader->outGroupsModel());

//            QListIterator<CT_OutStdSingularItemModel*> itIM(_itemModels);
//            while (itIM.hasNext())
//            {
//                CT_OutStdSingularItemModel* itemModel = itIM.next();
//            }



//            QListIterator<CT_OutStdGroupModel*> itGM(_groupModels);
//            while (itGM.hasNext())
//            {
//                CT_OutStdGroupModel* groupModel = itGM.next();
//            }

//        }
//    }

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepLoadDataFromItemPosition::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Epaisseur de la zone tampon", "m", -1e+09, 1e+09, 3, _buffer, 1);
}

void PB_StepLoadDataFromItemPosition::compute()
{
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_dataSource = inResultList.at(0);
    CT_ResultGroup* resIn_items = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_result = outResultList.at(0);


    CT_ResultItemIterator itIn_dataSource(resIn_dataSource, this, DEFin_dataSource);
    if (itIn_dataSource.hasNext())
    {
        const CT_DataSourceGeo *dataSource = (CT_DataSourceGeo*) itIn_dataSource.next();

        if (dataSource != NULL && dataSource->getNumberOfReader() > 0)
        {
            CT_ResultItemIterator itIn_items(resIn_items, this, DEFin_item);
            while (itIn_items.hasNext() && !isStopped())
            {
                const CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itIn_items.next();

                if (item != NULL)
                {
                    Eigen::Vector3d min, max;
                    item->getBoundingBox(min, max);

                    QList<QSharedPointer<CT_AbstractReader> > readers = dataSource->getReadersIntersecting(min, max);

                    for (int i = 0 ; i < readers.size() ; i++)
                    {
                        QSharedPointer<CT_AbstractReader> reader = readers.at(i);

                        if (reader->readFile())
                        {
                            // TO DO
                        }
                    }
                }
            }
        }
    }


    // OUT results creation (move it to the appropried place in the code)
    CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_result);
    res_result->addGroup(grp_grp);
    


}
