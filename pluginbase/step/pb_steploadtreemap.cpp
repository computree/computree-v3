#include "pb_steploadtreemap.h"

#include "ct_itemdrawable/ct_circle2d.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_view/ct_asciifilechoicebutton.h"
#include "ct_view/ct_combobox.h"

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
PB_StepLoadTreeMap::PB_StepLoadTreeMap(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
    _neededFields.append(CT_TextFileConfigurationFields("ID_Plot", QRegExp("([iI][dD]|[nN][uU][mM]|[pP][lL][oO][tT]|[pP][lL][aA][cC][eE][tT][tT][eE])"), false));
    _neededFields.append(CT_TextFileConfigurationFields("ID_Tree", QRegExp("([iI][dD]|[nN][uU][mM])"), false));
    _neededFields.append(CT_TextFileConfigurationFields("X", QRegExp("[xX]"), false));
    _neededFields.append(CT_TextFileConfigurationFields("Y", QRegExp("[yY]"), false));
    _neededFields.append(CT_TextFileConfigurationFields("DBH (cm)", QRegExp("([dD][bB][hH]|[dD][iI][aA][mM]|[D])"), false));

    _refFileName = "";
    _refHeader = true;
    _refSeparator = "\t";
    _refDecimal = ".";
    _refSkip = 0;

    _plotID = "";
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
void PB_StepLoadTreeMap::createOutResultModelListProtected()
{
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

    CT_AsciiFileChoiceButton *fileChoice = configDialog->addAsciiFileChoice("Fichier des arbres", "Fichier ASCII (*.txt ; *.asc)", true, _neededFields, _refFileName, _refHeader, _refSeparator, _refDecimal, _refLocale, _refSkip, _refColumns);
    CT_ComboBox *cbox = configDialog->addStringChoice(tr("Choix de la placette"), "", QStringList(), _plotID);

    connect(configDialog, SIGNAL(openned()), this, SLOT(fileChanged()));
    connect(fileChoice, SIGNAL(fileChanged()), this, SLOT(fileChanged()));
    connect(this, SIGNAL(updateComboBox(QStringList, QString)), cbox, SLOT(changeValues(QStringList, QString)));
}

void PB_StepLoadTreeMap::fileChanged()
{
    _plotsIds.clear();

    int colIDplot_ref  = _refColumns.value("ID_Plot", -1);

    if (colIDplot_ref < 0) {_plotID = ""; return;}

    QFile fRef(_refFileName);
    if (fRef.exists() && fRef.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&fRef);
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
        fRef.close();
    }

    QString val = "";
    if (_plotsIds.size() > 0) {val = _plotsIds.first();}
    emit updateComboBox(_plotsIds, val);
}

void PB_StepLoadTreeMap::compute()
{  

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_refRes = outResultList.at(0);

    PS_LOG->addMessage(LogInterface::info, LogInterface::step, QString(tr("Placette en cours de traitement : %1")).arg(_plotID));

    int colIDplot_ref  = _refColumns.value("ID_Plot", -1);
    if (colIDplot_ref < 0) {_plotID = "";}

    QFile fRef(_refFileName);
    if (fRef.exists() && fRef.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&fRef);
        stream.setLocale(_refLocale);

        int colID  = _refColumns.value("ID_Tree", -1);
        int colX   = _refColumns.value("X", -1);
        int colY   = _refColumns.value("Y", -1);
        int colVal = _refColumns.value("DBH (cm)", -1);

        if (colID < 0) {PS_LOG->addMessage(LogInterface::error, LogInterface::step, QString(tr("Champ IDtree non défini")));}
        if (colX < 0) {PS_LOG->addMessage(LogInterface::error, LogInterface::step, QString(tr("Champ X non défini")));}
        if (colY < 0) {PS_LOG->addMessage(LogInterface::error, LogInterface::step, QString(tr("Champ Y non défini")));}
        if (colVal < 0) {PS_LOG->addMessage(LogInterface::error, LogInterface::step, QString(tr("Champ DBH non défini")));}


        if (colID >=0 && colX >= 0 && colY >= 0 && colVal >= 0)
        {

            int colMax = colID;
            if (colX   > colMax) {colMax = colX;}
            if (colY   > colMax) {colMax = colY;}
            if (colVal > colMax) {colMax = colVal;}
            if (colIDplot_ref > colMax) {colMax = colIDplot_ref;}

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
                        if (colIDplot_ref >= 0)
                        {
                            plot =  values.at(colIDplot_ref);
                        }

                        if (plot == _plotID)
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
