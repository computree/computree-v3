#include "pb_steploadtreemap.h"

#include "ct_itemdrawable/ct_circle2d.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

// Alias for indexing models
#define DEFout_refRes "refRes"
#define DEFout_grpRef "grpRef"
#define DEFout_ref "ref"
#define DEFout_refDbh "refval"
#define DEFout_refID "refID"
#define DEFout_refIDplot "refIDplot"

#include <QFile>
#include <QTextStream>


// Constructor : initialization of parameters
PB_StepLoadTreeMap::PB_StepLoadTreeMap(CT_StepInitializeData &dataInit) : CT_StepBeginLoop(dataInit)
{
    _neededFields << "ID_Plot" << "ID" << "X" << "Y" << "DBH (cm)";

    _refFileName = "";
    _refHeader = true;
    _refSeparator = "\t";
    _refDecimal = ".";
    _refSkip = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepLoadTreeMap::getStepDescription() const
{
    return tr("Charge une tree map");
}

// Step detailled description
QString PB_StepLoadTreeMap::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepLoadTreeMap::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepLoadTreeMap::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadTreeMap(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepLoadTreeMap::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

// Creation and affiliation of OUT models
void PB_StepLoadTreeMap::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);

    CT_OutResultModelGroup *res_refRes = createNewOutResultModel(DEFout_refRes, tr("Positions de référence"));
    res_refRes->setRootGroup(DEFout_grpRef, new CT_StandardItemGroup(), tr("Groupe"));
    res_refRes->addItemModel(DEFout_grpRef, DEFout_ref, new CT_Circle2D(), tr("Position de référence"));
    res_refRes->addItemAttributeModel(DEFout_ref, DEFout_refDbh, new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER), tr("DBH"));
    res_refRes->addItemAttributeModel(DEFout_ref, DEFout_refID, new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID), tr("IDtree"));
    res_refRes->addItemAttributeModel(DEFout_ref, DEFout_refIDplot, new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID), tr("IDplot"));
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepLoadTreeMap::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addAsciiFileChoice("Fichier des arbres", "Fichier ASCII (*.txt ; *.asc)", true, _neededFields, _refFileName, _refHeader, _refSeparator, _refDecimal, _refLocale, _refSkip, _refColumns);
}

void PB_StepLoadTreeMap::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_refRes = outResultList.at(1);


    int colIDplot_ref  = _refColumns.value("ID_Plot", -1);
    bool multiPlots = (colIDplot_ref >= 0);

    QSharedPointer<CT_Counter> counter = getCounter();
    size_t  currentTurn = counter->getCurrentTurn();


    // Au premier tour : création de la liste des placettes
    if (multiPlots && currentTurn == 1)
    {
        _plotsIds.clear();

        QFile fRef00(_refFileName);
        if (fRef00.exists() && fRef00.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&fRef00);
            stream.setLocale(_refLocale);

            for (int i = 0 ; i < _refSkip ; i++) {stream.readLine();}
            if (_refHeader) {stream.readLine();}

            while (!stream.atEnd())
            {
                QString line = stream.readLine();
                if (!line.isEmpty())
                {
                    QStringList values = line.split(_refSeparator);

                    if (values.size() > colIDplot_ref)
                    {
                        const QString &val = values.at(colIDplot_ref);
                        if (!_plotsIds.contains(val)) {_plotsIds.append(val);}
                    }
                }
            }
            fRef00.close();
        }
        counter->setNTurns(_plotsIds.size());
        if (_plotsIds.size() <= 0) {return;}
    }


    QString currentPlot = "";
    if (multiPlots) {currentPlot = _plotsIds.at(currentTurn - 1);}
    NTurnsSelected();
    PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Placette en cours de traitement : %1")).arg(currentPlot));



    QFile fRef(_refFileName);
    if (fRef.exists() && fRef.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&fRef);
        stream.setLocale(_refLocale);

        int colID  = _refColumns.value("ID", -1);
        int colX   = _refColumns.value("X", -1);
        int colY   = _refColumns.value("Y", -1);
        int colVal = _refColumns.value("DBH (cm)", -1);

        if (colID >=0 && colX >= 0 && colY >= 0 && colVal >= 0)
        {

            int colMax = colID;
            if (colX   > colMax) {colMax = colX;}
            if (colY   > colMax) {colMax = colY;}
            if (colVal > colMax) {colMax = colVal;}

            for (int i = 0 ; i < _refSkip ; i++) {stream.readLine();}
            if (_refHeader) {stream.readLine();}

            size_t cpt = 1;
            while (!stream.atEnd())
            {
                QString line = stream.readLine();
                cpt++;
                if (!line.isEmpty())
                {
                    QStringList values = line.split(_refSeparator);
                    if (values.size() >= colMax)
                    {
                        QString plot = "";
                        if (multiPlots) {plot = values.at(colIDplot_ref);}

                        if (plot == currentPlot)
                        {
                            bool okX, okY, okVal;
                            double x = values.at(colX).toDouble(&okX);
                            double y = values.at(colY).toDouble(&okY);
                            float val = values.at(colVal).toFloat(&okVal);
                            QString id = values.at(colID);

                            if (okX && okY && okVal)
                            {
                                CT_StandardItemGroup* grp_grpRef= new CT_StandardItemGroup(DEFout_grpRef, res_refRes);
                                res_refRes->addGroup(grp_grpRef);

                                CT_Circle2D* item_ref = new CT_Circle2D(DEFout_ref, res_refRes, new CT_Circle2DData(Eigen::Vector2d(x,y), val/200.0));
                                grp_grpRef->addItemDrawable(item_ref);

                                item_ref->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_refDbh, CT_AbstractCategory::DATA_NUMBER, res_refRes, val));
                                item_ref->addItemAttribute(new CT_StdItemAttributeT<QString>(DEFout_refID, CT_AbstractCategory::DATA_ID, res_refRes, id));
                                item_ref->addItemAttribute(new CT_StdItemAttributeT<QString>(DEFout_refIDplot, CT_AbstractCategory::DATA_ID, res_refRes, plot));
                            } else {
                                PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Ligne %1 du fichier REF non valide")).arg(cpt));
                            }
                        }
                    }
                }
            }
        }
        fRef.close();
    }

}
