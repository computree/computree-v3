#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qfiledialog.h"
#include "qfile.h"
#include "qtextstream.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    QFile stepFileh(QString("%1/%2.h").arg(directory).arg(stepName.toLower()));
    QFile stepFilecpp(QString("%1/%2.cpp").arg(directory).arg(stepName.toLower()));

    QStringList splitted = stepName.split("_");

    QString parentClass = "CT_AbstractStep";

    if (stepFileh.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&stepFileh);

        stream << "#ifndef " << stepName.toUpper() << "_H\n";
        stream << "#define " << stepName.toUpper() << "_H\n";
        stream << "\n";
        stream << "#include \"ct_step/abstract/" << parentClass.toLower() << ".h\n";
        stream << "\n";
        stream << "    /*!\n";
        stream << "     * \\class " << stepName << "\n";
        stream << "     * \\ingroup Steps_" << splitted.at(0) << "\n";
        stream << "     * \\brief <b>Short description of the step.</b>\n";
        stream << "     *\n";
        stream << "     * Detailed decription of step purpose.\n";
        stream << "     * Please also give a general view of the algorithm.\n";
        stream << "     *\n";
        stream << "     * \\param ParameterName Description of the parameter to give in the DialogBox\n";
        stream << "     *\n";
        stream << "     *\n";
        stream << "     * <b>Input Models:</b>\n";
        stream << "     *\n";
        stream << "     *  - CT_ResultGroup \\n\n";
        stream << "     *      - CT_StandardItemGroup (name)... \\n\n";
        stream << "     *          - CT_ItemDrawable (name) \\n\n";
        stream << "     *          - CT_ItemDrawable (name) \\n\n";
        stream << "     *\n";
        stream << "     * <b>Output Models:</b>\n";
        stream << "     *\n";
        stream << "     *  - CT_ResultGroup \\n\n";
        stream << "     *      - <em>cpy CT_StandardItemGroup (Section)...</em> \\n\n";
        stream << "     *          - <em>cpy CT_ItemDrawable (name)</em> \\n\n";
        stream << "     *          - <em>cpy+ CT_ItemDrawable (name)</em> \\n\n";
        stream << "     *\n";
        stream << "     */\n";
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
        stream << "\n";
        stream << "    // Step parameters\n";
        stream << "    double _parametre;   /*!< parameter description */\n";
        stream << "\n";
        stream << "};\n";
        stream << "\n";
        stream << "#endif // " << stepName.toUpper() << "_H\n";

        stepFileh.close();
    } else {return false;}

    if (stepFilecpp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&stepFilecpp);

        stream << "#include \"" << stepName.toLower() << ".h\"\n";
        stream << "\n";
        stream << "// Inclusion of in and out results models\n";
        stream << "#include \"ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h\"\n";
        stream << "#include \"ct_result/model/inModel/ct_inresultmodelgroup.h\"\n";
        stream << "#include \"ct_result/model/outModel/ct_outresultmodelgroup.h\"\n";
        stream << "#include \"ct_result/model/inModel/ct_inresultmodelgrouptocopy.h\"\n";
        stream << "#include \"ct_result/model/outModel/ct_outresultmodelgroupcopy.h\"\n";
        stream << "\n";
        stream << "// Inclusion of in and out groups models\n";
        stream << "#include \"ct_itemdrawable/model/inModel/ct_inoneormoregroupmodel.h\"\n";
        stream << "#include \"ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h\"\n";
        stream << "#include \"ct_itemdrawable/model/inModel/ct_instandardgroupmodel.h\"\n";
        stream << "#include \"ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h\"\n";
        stream << "\n";
        stream << "// Inclusion of in and out items models\n";
        stream << "#include \"ct_itemdrawable/model/inModel/ct_instandarditemdrawablemodel.h\"\n";
        stream << "#include \"ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h\"\n";
        stream << "\n";
        stream << "// Inclusion of standard result class\n";
        stream << "#include \"ct_result/ct_resultgroup.h\"\n";
        stream << "\n";
        stream << "// Alias for indexing in and out models\n";
        stream << "#define DEF_SearchInResult  \"r\"\n";
        stream << "#define DEF_SearchOutResult \"r\"\n";
        stream << "\n";
        stream << "// Constructor : initialization of parameters\n";
        stream << stepName << "::" << stepName << "(CT_StepInitializeData &dataInit) : " << parentClass << "(dataInit)\n";
        stream << "{\n";
        stream << "    _parametre = 0;\n";
        stream << "}\n";
        stream << "\n";
        stream << "// Step description (tooltip of contextual menu)\n";
        stream << "QString " << stepName << "::getStepDescription() const\n";
        stream << "{\n";
        stream << "    return \"Description\";\n";
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
        stream << "}\n";
        stream << "\n";
        stream << "// Creation and affiliation of OUT models\n";
        stream << "void " << stepName << "::createOutResultModelListProtected()\n";
        stream << "{\n";
        stream << "}\n";
        stream << "\n";
        stream << "// Semi-automatic creation of step parameters DialogBox\n";
        stream << "void " << stepName << "::createPostConfigurationDialog()\n";
        stream << "{\n";
        stream << "    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();\n";
        stream << "\n";
        stream << "    configDialog->addDouble(\"Parameter value\", \"unit\", 0, 1000, 2, _parametre);\n";
        stream << "}\n";
        stream << "\n";
        stream << "void " << stepName << "::compute()\n";
        stream << "{\n";
        stream << "    // DONT'T FORGET TO ADD THIS STEP TO THE STEPPLUGINMANAGER !!!!!\n";
        stream << "}\n";

        stepFilecpp.close();
    } else {return false;}

    return true;
}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}
