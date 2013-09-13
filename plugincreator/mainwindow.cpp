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
    QString s = QFileDialog::getExistingDirectory(this, "Dossier du plugin", "");
    ui->directory->setText(s);
}

void MainWindow::on_pushButton_2_clicked()
{
    if ((ui->directory->text()=="") || (!QDir(ui->directory->text()).exists())) {ui->statusBar->showMessage("Vous devez choisir un répertoire valide pour le plugin");return;}
    if (ui->code->text().size() < 2) {ui->statusBar->showMessage("Vous devez définir un code (2 à 4 lettres) pour le plugin");return;}
    if (ui->name->text().size() < 4) {ui->statusBar->showMessage("Vous devez choisir un nom de plugin compris de 4 à 20 caractères");return;}

    if (createFiles(ui->directory->text(), ui->code->text().toUpper(), ui->name->text()))
    {
        ui->statusBar->showMessage("CREATION REUSSIE !!!");
    } else {
        ui->statusBar->showMessage("LA CREATION DES FICHIERS DU PLUGIN A ECHOUE");
    }
}

void MainWindow::on_code_textChanged(const QString &arg1)
{
    ui->code->setText(arg1.toUpper());
}

bool MainWindow::createFiles(QString directory, QString code, QString name)
{
    QString target = QString("plug_%1").arg(name.toLower());
    QString entryName = QString("%1_PluginEntry").arg(code);
    QString managerName = QString("%1_StepPluginManager").arg(code);
    QString proName = QString("plugin%1").arg(name.toLower());

    QDir dir(directory);
    dir.mkdir(proName);

    QFile proFile(QString("%1/%3/%2.pro").arg(directory).arg(proName).arg(proName));
    QFile entryFileh(QString("%1/%3/%2.h").arg(directory).arg(entryName.toLower()).arg(proName));
    QFile entryFilecpp(QString("%1/%3/%2.cpp").arg(directory).arg(entryName.toLower()).arg(proName));
    QFile managerFileh(QString("%1/%3/%2.h").arg(directory).arg(managerName.toLower()).arg(proName));
    QFile managerFilecpp(QString("%1/%3/%2.cpp").arg(directory).arg(managerName.toLower()).arg(proName));

    if (proFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&proFile);

        stream << "include(../../pluginsharedv2/shared.pri)\n";
        stream << "include($${PLUGIN_SHARED_DIR}/include.pri)\n";
        stream << "\n";
        stream << "TARGET = " << target << "\n";
        stream << "\n";
        stream << "HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \\\n";
        stream << "    " << entryName.toLower() << ".h \\\n";
        stream << "    " << managerName.toLower() << ".h\n";
        stream << "SOURCES += \\\n";
        stream << "    " << entryName.toLower() << ".cpp \\\n";
        stream << "    " << managerName.toLower() << ".cpp\n";
        proFile.close();
    } else {return false;}

    if (entryFileh.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&entryFileh);

        stream << "#ifndef " << code << "_PLUGINENTRY_H\n";
        stream << "#define " << code << "_PLUGINENTRY_H\n";
        stream << "\n";
        stream << "#include \"interfaces.h\"\n";
        stream << "\n";
        stream << "class " << code << "_StepPluginManager;\n";
        stream << "\n";
        stream << "class " << code << "_PluginEntry : public PluginInterface\n";
        stream << "{\n";
        stream << "    Q_OBJECT\n";
        stream << "\n";
        stream << "#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)\n";
        stream << "    Q_PLUGIN_METADATA(IID PluginInterface_iid)\n";
        stream << "#endif\n";
        stream << "\n";
        stream << "    Q_INTERFACES(PluginInterface)\n";
        stream << "\n";
        stream << "public:\n";
        stream << "    " << code << "_PluginEntry();\n";
        stream << "    ~" << code << "_PluginEntry();\n";
        stream << "\n";
        stream << "    QString getVersion() const;\n";
        stream << "    StepPluginInterface* getStepPluginManager();\n";
        stream << "\n";
        stream << "private:\n";
        stream << "    " << code << "_StepPluginManager *_stepPluginManager;\n";
        stream << "};\n";
        stream << "\n";
        stream << "#endif // " << code << "_PLUGINENTRY_H";

        entryFileh.close();
    } else {return false;}

    if (entryFilecpp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&entryFilecpp);

        stream << "#include \"" << code.toLower() << "_pluginentry.h\"\n";
        stream << "#include \"" << code.toLower() << "_steppluginmanager.h\"\n";
        stream << "\n";
        stream << code << "_PluginEntry::" << code << "_PluginEntry()\n";
        stream << "{\n";
        stream << "    _stepPluginManager = new " << code << "_StepPluginManager();\n";
        stream << "    _stepPluginManager->init();\n";
        stream << "}\n";
        stream << "\n";
        stream << code << "_PluginEntry::~" << code << "_PluginEntry()\n";
        stream << "{\n";
        stream << "    delete _stepPluginManager;\n";
        stream << "}\n";
        stream << "\n";
        stream << "QString " << code << "_PluginEntry::getVersion() const\n";
        stream << "{\n";
        stream << "    return \"1.0\";\n";
        stream << "}\n";
        stream << "\n";
        stream << "StepPluginInterface* " << code << "_PluginEntry::getStepPluginManager()\n";
        stream << "{\n";
        stream << "    return _stepPluginManager;\n";
        stream << "}\n";
        stream << "\n";
        stream << "#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)\n";
        stream << "    Q_EXPORT_PLUGIN2(" << target << ", " << code << "_PluginEntry)\n";
        stream << "#endif\n";


        entryFilecpp.close();
    } else {return false;}

    if (managerFileh.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&managerFileh);

        stream << "#ifndef " << code << "_STEPPLUGINMANAGER_H\n";
        stream << "#define " << code << "_STEPPLUGINMANAGER_H\n";
        stream << "\n";
        stream << "#include \"ct_abstractstepplugin.h\"\n";
        stream << "\n";
        stream << "class " << code << "_StepPluginManager : public CT_AbstractStepPlugin\n";
        stream << "{\n";
        stream << "public:\n";
        stream << "    " << code << "_StepPluginManager();\n";
        stream << "    ~" << code << "_StepPluginManager();\n";
        stream << "\n";
        stream << "protected:\n";
        stream << "\n";
        stream << "    bool loadGenericsStep();\n";
        stream << "    bool loadOpenFileStep();\n";
        stream << "    bool loadCanBeAddedFirstStep();\n";
        stream << "\n";
        stream << "};\n";
        stream << "\n";
        stream << "#endif // " << code << "_STEPPLUGINMANAGER_H\n";

        managerFileh.close();
    } else {return false;}

    if (managerFilecpp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&managerFilecpp);

        stream << "#include \"" << code.toLower() << "_steppluginmanager.h\"\n";
        stream << "#include \"ct_stepseparator.h\"\n";
        stream << "#include \"ct_steploadfileseparator.h\"\n";
        stream << "#include \"ct_stepcanbeaddedfirstseparator.h\"\n";
        stream << "// Inclure ici les entetes des classes definissant des etapes dans le plugin\n";
        stream << "\n";
        stream << code << "_StepPluginManager::" << code << "_StepPluginManager() : CT_AbstractStepPlugin()\n";
        stream << "{\n";
        stream << "}\n";
        stream << "\n";
        stream << code << "_StepPluginManager::~" << code << "_StepPluginManager()\n";
        stream << "{\n";
        stream << "}\n";
        stream << "\n";
        stream << "bool " << code << "_StepPluginManager::loadGenericsStep()\n";
        stream << "{\n";
        stream << "    CT_StepSeparator *sep = addNewSeparator(new CT_StepSeparator());\n";
        stream << "    // Ajouter ici les etapes\n";
        stream << "    //sep->addStep(new NomDeLEtape(*createNewStepInitializeData(NULL)));\n";
        stream << "\n";
        stream << "    return true;\n";
        stream << "}\n";
        stream << "\n";
        stream << "bool " << code << "_StepPluginManager::loadOpenFileStep()\n";
        stream << "{\n";
        stream << "    return true;\n";
        stream << "}\n";
        stream << "\n";
        stream << "bool " << code << "_StepPluginManager::loadCanBeAddedFirstStep()\n";
        stream << "{\n";
        stream << "    return true;\n";
        stream << "}\n";
        stream << "\n";

        managerFilecpp.close();
    } else {return false;}

    return true;
}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}
