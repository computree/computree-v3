#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qfiledialog.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qset.h"
#include "tools.h"
#include "models/abstractcopymodel.h"
#include "qtextcodec.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _inModelDialog = new INModelDialog();
    _outModelDialog = new OUTModelDialog();
    _copyModelDialog = new COPYModelDialog(_inModelDialog->getModel());
    _paramModelDialog = new ParameterModelDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->log->setText("");
    QString s = QFileDialog::getExistingDirectory(this, "Dossier où ajouter les fichiers de l'étape", "");
    ui->directory->setText(s);
}

void MainWindow::on_pushButton_2_clicked()
{
    if ((ui->directory->text()=="") || (!QDir(ui->directory->text()).exists())) {ui->statusBar->showMessage("Vous devez choisir un répertoire valide pour les fichiers de l'étape");return;}
    if (ui->code->text().size() < 2) {ui->statusBar->showMessage("Vous devez définir un nom pour l'étape");return;}
    if (ui->description->text().size() <= 0) {ui->statusBar->showMessage("Vous devez donner une description pour l'étape");return;}

    if (createFiles(ui->directory->text(), ui->code->text()))
    {
        ui->statusBar->showMessage("CREATION REUSSIE !!!");
        ui->log->clear();
        ui->log->append(QString("L'étape %1 a été créée\nLes fichiers :").arg(ui->code->text()));
        ui->log->append(QString("     - %1.h").arg(ui->code->text().toLower()));
        ui->log->append(QString("     - %1.cpp").arg(ui->code->text().toLower()));
        ui->log->append(QString("Ont été créés dans le répertoire %1\n\n").arg(ui->directory->text()));
        ui->log->append("Vous devez maintenant ajouter ces fichiers dans le projet Qt de votre plugin");
        ui->log->append("(Clic droit sur le projet / Ajouter des fichiers existants)\n\n");
        ui->log->append("N'OUBLIEZ PAS D'AJOUTER L'ETAPE A VOTRE StepPluginManager !!!");


    } else {
        ui->statusBar->showMessage("LA CREATION DES FICHIERS DE L'ETAPE A ECHOUE");
        ui->log->clear();
        ui->log->append("LA CREATION DES FICHIERS DE L'ETAPE A ECHOUE");
    }
}

bool MainWindow::createFiles(QString directory, QString stepName)
{
    QString str;

    QFile stepFileh(QString("%1/%2.h").arg(directory).arg(stepName.toLower()));
    QFile stepFilecpp(QString("%1/%2.cpp").arg(directory).arg(stepName.toLower()));

    QStringList splitted = stepName.split("_");

    QString parentClass = "CT_AbstractStep";

    if (stepFileh.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&stepFileh);
        stream.setCodec("Latin-1");

        stream << "#ifndef " << stepName.toUpper() << "_H\n";
        stream << "#define " << stepName.toUpper() << "_H\n";
        stream << "\n";
        stream << "#include \"ct_step/abstract/" << parentClass.toLower() << ".h\"\n";
        stream << "\n";

        if (_copyModelDialog->copyIncludesNeeded())
        {
            stream << "// Inclusion of auto-indexation system\n";
            stream << "#include \"ct_tools/model/ct_autorenamemodels.h\"\n";
            stream << "\n";
        }

        stream << "/*!\n";
        stream << " * \\class " << stepName << "\n";
        stream << " * \\ingroup Steps_" << splitted.at(0) << "\n";
        stream << " * \\brief <b>" + ui->description->text() + ".</b>\n";
        stream << " *\n";
        stream << " * Detailed decription of step purpose.\n";
        stream << " * Please also give a general view of the algorithm.\n";
        stream << " *\n";
        stream << _paramModelDialog->getParamatersDoc();
        stream << " *\n";
        stream << " *\n";
        stream << " * <b>Input Models:</b>\n";
        stream << " *\n";

        str = _inModelDialog->getInModelDoc();
        if (str == "")
        {
            stream << " * No input model defined\\n\n";
            stream << " *\n";
        } else {
            stream << str;
        }

        stream << " * <b>Output Models:</b>\n";
        stream << " *\n";

        str = _copyModelDialog->getCopyModelDoc();
        str += _outModelDialog->getOutModelDoc();
        if (str == "")
        {
            stream << " * No output model defined\\n\n";
            stream << " *\n";
        } else {
            stream << str;
        }

        stream << " */\n";
        stream << "\n";
        stream << "class " << stepName << ": public " << parentClass << "\n";
        stream << "{\n";
        stream << "    Q_OBJECT\n";
        stream << "\n";
        stream << "public:\n";
        stream << "\n";
        stream << "    /*! \\brief Step constructor\n";
        stream << "     * \n";
        stream << "     * Create a new instance of the step\n";
        stream << "     * \n";
        stream << "     * \\param dataInit Step parameters object\n";
        stream << "     */\n";
        stream << "    " << stepName << "(CT_StepInitializeData &dataInit);\n";
        stream << "\n";
        stream << "    /*! \\brief Step description\n";
        stream << "     * \n";
        stream << "     * Return a description of the step function\n";
        stream << "     */\n";
        stream << "    QString getStepDescription() const;\n";
        stream << "\n";
        stream << "    /*! \\brief Step copy\n";
        stream << "     * \n";
        stream << "     * Step copy, used when a step is added by step contextual menu\n";
        stream << "     */\n";
        stream << "    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);\n";
        stream << "\n";
        stream << "protected:\n";
        stream << "\n";
        stream << "    /*! \\brief Input results specification\n";
        stream << "     * \n";
        stream << "     * Specification of input results models needed by the step (IN)\n";
        stream << "     */\n";
        stream << "    void createInResultModelListProtected();\n";
        stream << "\n";
        stream << "    /*! \\brief Parameters DialogBox\n";
        stream << "     * \n";
        stream << "     * DialogBox asking for step parameters\n";
        stream << "     */\n";
        stream << "    void createPostConfigurationDialog();\n";
        stream << "\n";
        stream << "    /*! \\brief Output results specification\n";
        stream << "     * \n";
        stream << "     * Specification of output results models created by the step (OUT)\n";
        stream << "     */\n";
        stream << "    void createOutResultModelListProtected();\n";
        stream << "\n";
        stream << "    /*! \\brief Algorithm of the step\n";
        stream << "     * \n";
        stream << "     * Step computation, using input results, and creating output results\n";
        stream << "     */\n";
        stream << "    void compute();\n";
        stream << "\n";
        stream << "private:\n";

        str = _copyModelDialog->getAutoRenamesDeclarations();
        if (str != "")
        {
            stream << "\n";
            stream << Tools::getIndentation(1) << "// Declaration of autoRenames Variables (groups or items addes to In models copies)\n";
            stream << str;
        }

        stream << "\n";

        str = _copyModelDialog->getAutoRenamesDeclarations();
        if (str!="")
        {

        }


        stream << "    // Step parameters\n";        
        str = _paramModelDialog->getParametersDeclaration();
        if (str == "")
        {
            stream << "    // No parameter for this step";
        } else {
            stream << str;
        }

        stream << "\n";
        stream << "};\n";
        stream << "\n";
        stream << "#endif // " << stepName.toUpper() << "_H\n";

        stepFileh.close();
    } else {return false;}

    if (stepFilecpp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&stepFilecpp);
        stream.setCodec("Latin-1");

        stream << "#include \"" << stepName.toLower() << ".h\"\n";
        stream << "\n";

        stream << "// Inclusion of in models\n";
        str = _inModelDialog->getInIncludes();
        if (str!="")
        {
            stream << str;
        } else {
            stream << "#include \"ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h\"\n";
        }
        stream << "\n";

        stream << "// Inclusion of out models\n";
        stream << "#include \"ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h\"\n";
        str = _outModelDialog->getOutIncludes();
        if (str!="")
        {
            stream << str;
        }
        stream << "\n";

        stream << "// Inclusion of standard result class\n";
        stream << "#include \"ct_result/ct_resultgroup.h\"\n";
        stream << "\n";

        QSet<QString> listAction;
        _copyModelDialog->getActionsIncludes(listAction);
        if (listAction.size()>0)
        {
            stream << "// Inclusion of actions methods\n";
            stream << Tools::getQStringListConcat(listAction);
            stream << "\n";
        }


        QSet<QString> list;
        _inModelDialog->getInItemTypesIncludes(list);
        _outModelDialog->getOutItemTypesIncludes(list);

        if (list.size()>0)
        {
            stream << "// Inclusion of used ItemDrawable classes\n";
            stream << Tools::getQStringListConcat(list);
            stream << "\n";
        }

        str = _inModelDialog->getInDefines();
        if (str!="")
        {
            stream << "// Alias for indexing in models\n";
            stream << str;
            stream << "\n";
        }

        str = _outModelDialog->getOutDefines();
        if (str!="")
        {
            stream << "// Alias for indexing out models\n";
            stream << str;
            stream << "\n";
        }

        stream << "// Constructor : initialization of parameters\n";
        stream << stepName << "::" << stepName << "(CT_StepInitializeData &dataInit) : " << parentClass << "(dataInit)\n";
        stream << "{\n";
        stream << _paramModelDialog->getParametersInitialization();
        stream << "}\n";
        stream << "\n";
        stream << "// Step description (tooltip of contextual menu)\n";
        stream << "QString " << stepName << "::getStepDescription() const\n";
        stream << "{\n";
        stream << "    return \"" << ui->description->text() << "\";\n";
        stream << "}\n";
        stream << "\n";
        stream << "// Step copy method\n";
        stream << "CT_VirtualAbstractStep* " << stepName << "::createNewInstance(CT_StepInitializeData &dataInit)\n";
        stream << "{\n";
        stream << "    return new " << stepName << "(dataInit);\n";
        stream << "}\n";
        stream << "\n";
        stream << "//////////////////// PROTECTED METHODS //////////////////\n";
        stream << "\n";
        stream << "// Creation and affiliation of IN models\n";
        stream << "void " << stepName << "::createInResultModelListProtected()\n";
        stream << "{\n";

        QString str = _inModelDialog->getInModelsDefinitions();
        if (str == "")
        {
            stream << "    // No in result is needed\n";
            stream << "    CT_InResultModelNotNeedInputResult *resultModel = new CT_InResultModelNotNeedInputResult();\n";
            stream << "    addInResultModel(resultModel);\n";
        } else {
            stream << str;
        }
        stream << "}\n";
        stream << "\n";
        stream << "// Creation and affiliation of OUT models\n";
        stream << "void " << stepName << "::createOutResultModelListProtected()\n";
        stream << "{\n";

        str = _copyModelDialog->getCopyModelsDefinitions();
        QString str2 = _outModelDialog->getOutModelsDefinitions();

        if (str!="" || str2!="")
        {
            stream << str;
            stream << "\n";
            stream << "\n";
            stream << str2;
        } else {
            stream << "    // No OUT model definition => create an empty result\n";
            stream << "    CT_OutStandardGroupModel *groupModel = new CT_OutStandardGroupModel(\"r\");\n";
            stream << "    CT_OutResultModelGroup *resultModel = new CT_OutResultModelGroup(\"r\", groupModel, tr(\"Empty Result\"));\n";
            stream << "    addOutResultModel(resultModel);\n";
        }

        stream << "}\n";
        stream << "\n";
        stream << "// Semi-automatic creation of step parameters DialogBox\n";
        stream << "void " << stepName << "::createPostConfigurationDialog()\n";
        stream << "{\n";

        str = _paramModelDialog->getParametersDialogCommands();
        if (str == "")
        {
            stream << "    // No parameter dialog for this step";
        } else {
            stream << "    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();\n";
            stream << "\n";
            stream << str;
        }

        stream << "}\n";
        stream << "\n";
        stream << "void " << stepName << "::compute()\n";
        stream << "{\n";
        stream << "    // DONT'T FORGET TO ADD THIS STEP TO THE STEPPLUGINMANAGER !!!!!\n";
        stream << "\n";
        stream << "\n";
        stream << "    // --------------------------\n";
        stream << "    // Gets IN results and models\n";

        str = _inModelDialog->getInComputeBeginning();
        if (str != "")
        {
            stream << str;
            stream << "\n";
            stream << "\n";
        }

        stream << "    // ---------------------------\n";
        stream << "    // Gets OUT results and models\n";

        str = _outModelDialog->getOutComputeBeginning(_inModelDialog->getNumberOfCopyResults());
        if (str != "")
        {
            stream << str;
            stream << "\n";
            stream << "\n";
        }


        str = _copyModelDialog->getCopyComputeLoops();
        if (str != "")
        {
            stream << "    // ---------------------------------\n";
            stream << "    // Goes through copied IN results structure\n";
            stream << str;
            stream << "\n";
            stream << "\n";
        }


        str = _inModelDialog->getInComputeLoops();
        if (str != "")
        {
            stream << "    // ---------------------------------\n";
            stream << "    // Goes through IN results structure\n";
            stream << str;
            stream << "\n";
            stream << "\n";
        }

        str = _outModelDialog->getOutComputeItemsCreations();
        if (str != "")
        {
            stream << "    // ------------------------------\n";
            stream << "    // Create OUT groups and items\n";
            stream << str;
            stream << "\n";
        }

        stream << "}\n";



        stepFilecpp.close();
    } else {return false;}

    return true;
}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}

void MainWindow::on_modelin_clicked()
{
    _inModelDialog->show();
}

void MainWindow::on_modelout_clicked()
{
    _outModelDialog->show();
}

void MainWindow::on_modelcopy_clicked()
{
    if (_inModelDialog->hasBeenModified())
    {
        _copyModelDialog->init();
        _inModelDialog->setModified(false);
    }
    _copyModelDialog->show();
}

void MainWindow::on_parameters_clicked()
{
    _paramModelDialog->show();
}
