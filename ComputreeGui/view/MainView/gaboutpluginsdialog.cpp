#include "gaboutpluginsdialog.h"
#include "ui_gaboutpluginsdialog.h"

#include "view/MainView/gaboutstepdialog.h"
#include "view/MainView/gredmineparametersdialog.h"

#include "cdm_tools.h"
#include "dm_guimanager.h"

#include "ct_stepseparator.h"
#include "ct_steploadfileseparator.h"
#include "ct_stepcanbeaddedfirstseparator.h"
#include "ct_actions/ct_actionsseparator.h"
#include "ct_actions/abstract/ct_abstractaction.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_abstractstepplugin.h"

#include <QListWidgetItem>
#include <QFileDialog>
#include <QDebug>

#define EOL "&#13;&#10;"

GAboutPluginsDialog::GAboutPluginsDialog(const CDM_PluginManager &pManager,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GAboutPluginsDialog)
{
    ui->setupUi(this);

    _pManager = (CDM_PluginManager*)&pManager;
    _mustReloadStep = false;

    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(close()));

    initView();
}

GAboutPluginsDialog::~GAboutPluginsDialog()
{
    delete ui;
    _stepList.clear();
}

bool GAboutPluginsDialog::mustReloadStepPlugins() const
{
    return _mustReloadStep;
}

void GAboutPluginsDialog::initView()
{
    ui->treeWidget->clear();

    int n = _pManager->countPluginLoaded();

    // plugins d'étapes
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget);
    rootItem->setData(0, Qt::DisplayRole, trUtf8("Plugins d'\xc3\xa9""tapes"));

    for(int i=0; i<n; ++i)
    {
        QString pluginName = _pManager->getPluginName(i);
        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(rootItem);
        item->setData(0, Qt::DisplayRole, pluginName);
        item->setData(0, Qt::UserRole, I_Step);
        item->setData(0, Qt::UserRole+1, i);

        CT_AbstractStepPlugin *sPManager = _pManager->getPlugin(i);

        // Etape de chargement de fichiers
        QList<CT_StepLoadFileSeparator*> l0 = sPManager->getOpenFileStepAvailable();
        QListIterator<CT_StepLoadFileSeparator*> it0(l0);

        if(it0.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Load File Step"));

            while(it0.hasNext())
            {
                QList<CT_AbstractStepLoadFile*> l00 = it0.next()->getStepList();

                while(!l00.isEmpty())
                    createItemsForStep(itemF, l00.takeFirst());
            }
        }

        // Etape pouvant être ajouté en premier
        QList<CT_StepCanBeAddedFirstSeparator*> l1 = sPManager->getCanBeAddedFirstStepAvailable();
        QListIterator<CT_StepCanBeAddedFirstSeparator*> it1(l1);

        if(it1.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Can be added first Step"));

            while(it1.hasNext())
            {
                QList<CT_AbstractStepCanBeAddedFirst*> l11 = it1.next()->getStepList();

                while(!l11.isEmpty())
                    createItemsForStep(itemF, l11.takeFirst());
            }
        }

        // Etape générique
        QList<CT_StepSeparator*> l2 = sPManager->getGenericsStepAvailable();
        QListIterator<CT_StepSeparator*> it2(l2);

        if(it2.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Normal Step"));

            while(it2.hasNext())
            {
                QList<CT_VirtualAbstractStep*> l22 = it2.next()->getStepList();

                while(!l22.isEmpty())
                    createItemsForStep(itemF, l22.takeFirst());
            }
        }

        // Actions
        QList<CT_ActionsSeparator*> l3 = sPManager->getActionsAvailable();
        QListIterator<CT_ActionsSeparator*> it3(l3);

        if(it3.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Action"));

            while(it3.hasNext())
            {
                QList<CT_AbstractAction*> l33 = it3.next()->actions();

                while(!l33.isEmpty())
                    createItemsForAction(itemF, l33.takeFirst());
            }
        }

        // Exporters
        QList<CT_StandardExporterSeparator*> l4 = sPManager->getExportersAvailable();
        QListIterator<CT_StandardExporterSeparator*> it4(l4);

        if(it4.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Exporter"));

            while(it4.hasNext())
            {
                QList<CT_AbstractExporter*> l44 = it4.next()->exporters();

                while(!l44.isEmpty())
                    createItemsForExporter(itemF, l44.takeFirst());
            }
        }
    }

    ui->treeWidget->insertTopLevelItem(0, rootItem);
}

void GAboutPluginsDialog::createItemsForStep(QTreeWidgetItem *parent, CT_VirtualAbstractStep *step)
{
    QString stepName = step->getStepName();
    QString keyName = parent->text(0)+ "_" + stepName;

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, stepName);

    _stepList.insert(keyName, step);

    QTreeWidgetItem *itemIN = new QTreeWidgetItem(item);
    itemIN->setText(0, "IN");

    QList<const CT_InAbstractResultModel*> list = step->getInResultsModel();
    QListIterator<const CT_InAbstractResultModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForResultModel(itemIN, it.next());

    QTreeWidgetItem *itemOUT = new QTreeWidgetItem(item);
    itemOUT->setText(0, "OUT");

    QList<const CT_OutAbstractResultModel*> listO = step->getOutResultsModel();
    QListIterator<const CT_OutAbstractResultModel*> itO(listO);

    while(itO.hasNext())
        recursiveCreateItemsForResultModel(itemOUT, itO.next());
}

void GAboutPluginsDialog::createItemsForAction(QTreeWidgetItem *parent, CT_AbstractAction *act)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, act->title());
    item->setIcon(0, act->icon());
}

void GAboutPluginsDialog::createItemsForExporter(QTreeWidgetItem *parent, CT_AbstractExporter *ex)
{
    CDM_Tools tools(GUI_MANAGER->getPluginManager());

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, tools.createNameForExporter(ex));
    /*item->setData(0, Qt::DisplayRole, _eManager->getPluginName(i));
    item->setData(0, Qt::UserRole, I_Export);
    item->setData(0, Qt::UserRole+1, i);*/
}

void GAboutPluginsDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->resultName());

    QList<CT_AbstractModel*> list = rModel->childrens();
    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForModel(item, it.next());
}

void GAboutPluginsDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->displayableName());

    QList<CT_AbstractModel*> list = rModel->childrens();
    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForModel(item, it.next());
}

void GAboutPluginsDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_AbstractModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,rModel->displayableName());

    QList<CT_AbstractModel*> list = rModel->childrens();
    QListIterator<CT_AbstractModel*> it(list);

    while(it.hasNext())
        recursiveCreateItemsForModel(item, it.next());
}

void GAboutPluginsDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if(current != NULL)
    {
        bool ok;
        int from = current->data(0, Qt::UserRole).toInt(&ok);

        if(ok)
        {
            if(from == I_Step)
            {
                CT_AbstractStepPlugin *tinterface = _pManager->getPlugin(current->data(0, Qt::UserRole+1).toInt());
                ui->pushButtonConfigurer->setEnabled((tinterface != NULL)
                                                     && tinterface->canBeConfigured());
            }
        }
    }
}

void GAboutPluginsDialog::on_pushButtonConfigurer_clicked()
{
    QTreeWidgetItem *current = ui->treeWidget->currentItem();

    if(current != NULL)
    {
        bool ok;
        int from = current->data(0, Qt::UserRole).toInt(&ok);

        if(ok)
        {
            if(from == I_Step)
            {
                int currentRow = current->data(0, Qt::UserRole+1).toInt();
                CT_AbstractStepPlugin *tinterface = _pManager->getPlugin(currentRow);

                if(tinterface != NULL)
                    tinterface->configure();
            }
        }
    }
}

void GAboutPluginsDialog::on_pushButtonRecharger_clicked()
{
    _mustReloadStep = true;
    close();
}

void GAboutPluginsDialog::on_pb_redmineExport_clicked()
{
    GRedmineParametersDialog dialog;
    dialog.exec();

    QString s = dialog.getDirectory();

    QString languageId = "Fr";

    QString quote = "\"";
    QString extension = "bat";
    if (dialog.getSystemType() == GRedmineParametersDialog::Linux)
    {
        quote = "'";
        extension = "sh";
    }

    if (QDir(s).exists())
    {
        int n = _pManager->countPluginLoaded();

        QFile scriptFile(QString("%1/uploads_plugins_wikis.%2").arg(s).arg(extension));

        if (scriptFile.open(QFile::WriteOnly))
        {
            QTextStream scriptStream(&scriptFile);

            for(int i = 0 ; i < n ; ++i)
            {
                CT_AbstractStepPlugin *sPManager = _pManager->getPlugin(i);

                QString xmlFileName = QString("%1_Steps_%2.xml").arg(languageId).arg(_pManager->getPluginName(i));
                QString xmlFilePath = QString("%1/%2").arg(s).arg(xmlFileName);

                scriptStream << "curl -u " << quote << dialog.getId() << ":" << dialog.getPassword() << quote;
                scriptStream << " -v -H \"Content-Type: application/xml\" -X PUT --data \"@" << xmlFilePath << "\" ";
                scriptStream << sPManager->getPluginURL() << "/wiki/" << xmlFileName;
                if (dialog.isProxyActivated())
                {
                    scriptStream << " --proxy " << dialog.getProxyId() << ":" << dialog.getProxyPassword();
                    scriptStream << "@" << dialog.getProxyAdress();
                }
                scriptStream << "\n";

                QFile file(xmlFilePath);
                if(file.open(QFile::WriteOnly))
                {
                    QTextStream stream(&file);
                    stream.setCodec("UTF-8");

                    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                    stream << "<wiki_page>\n";
                    stream << "<text>\n";

                    stream << tr("h1. Etapes du plugin ") << _pManager->getPluginName(i) << EOL << EOL;
                    stream << "_Sommaire_" << EOL << EOL;
                    stream << "{{toc}}" << EOL << EOL;

                    // Etape de chargement de fichiers
                    QList<CT_StepLoadFileSeparator*> l0 = sPManager->getOpenFileStepAvailable();
                    QListIterator<CT_StepLoadFileSeparator*> it0(l0);

                    if(it0.hasNext())
                    {
                        stream << tr("h1. Etapes de chargement") << EOL << EOL;

                        while(it0.hasNext())
                        {
                            QList<CT_AbstractStepLoadFile*> l00 = it0.next()->getStepList();

                            while(!l00.isEmpty())
                            {
                                CT_VirtualAbstractStep* step = l00.takeFirst();
                                exportStepData(stream, step);
                            }
                        }
                    }


                    // Etape pouvant être ajoutées en premier
                    QList<CT_StepCanBeAddedFirstSeparator*> l1 = sPManager->getCanBeAddedFirstStepAvailable();
                    QListIterator<CT_StepCanBeAddedFirstSeparator*> it1(l1);

                    if(it1.hasNext())
                    {
                        stream << EOL;
                        stream << tr("h1. Etapes pouvant être ajoutées en premier") << EOL << EOL;

                        while(it1.hasNext())
                        {
                            QList<CT_AbstractStepCanBeAddedFirst*> l11 = it1.next()->getStepList();

                            while(!l11.isEmpty())
                            {
                                CT_VirtualAbstractStep* step = l11.takeFirst();
                                exportStepData(stream, step);
                            }
                        }
                    }


                    // Etape générique
                    QList<CT_StepSeparator*> l2 = sPManager->getGenericsStepAvailable();
                    QListIterator<CT_StepSeparator*> it2(l2);

                    if(it2.hasNext())
                    {
                        stream << EOL;
                        stream << tr("h1. Etapes standard") << EOL << EOL;

                        while(it2.hasNext())
                        {
                            QList<CT_VirtualAbstractStep*> l22 = it2.next()->getStepList();

                            while(!l22.isEmpty())
                            {
                                CT_VirtualAbstractStep* step = l22.takeFirst();
                                exportStepData(stream, step);
                            }
                        }
                    }

                    stream << "</text>\n";
                    stream << "</wiki_page>";

                    file.close();
                }
            }
            scriptFile.close();
        }
    }
}

void GAboutPluginsDialog::exportStepData(QTextStream &stream, CT_VirtualAbstractStep *step)
{
    QString htmlDescr = step->getStepDetailledDescription();
    convertHTMLtoTextile(htmlDescr);

    stream << "h2. " << step->getStepName() << EOL << EOL;
    stream << tr("*_Description courte_* : *") << step->getStepDescription() << "*" << EOL << EOL;
    stream << tr("*_Description détaillée_* : ");
    stream << htmlDescr;
    stream << EOL << EOL;

}

void GAboutPluginsDialog::convertHTMLtoTextile(QString &input)
{
    input.replace("<br>", EOL);
    input.replace("<ul>", EOL);
    input.replace("</ul>", EOL);
    input.replace("<li>", QString("%1* ").arg(EOL));
    input.replace("</li>", EOL);
    input.replace("<ol>", QString("%1# ").arg(EOL));
    input.replace("</ol>", EOL);
    input.replace("<b>", "*");
    input.replace("</b>", "*");
    input.replace("<i>", "_");
    input.replace("</i>", "_");
    input.replace("<em>", "_");
    input.replace("</em>", "_");
}

void GAboutPluginsDialog::on_pb_stepInfo_clicked()
{
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();

    if (currentItem != NULL)
    {
        QString stepName = currentItem->text(0);
        QTreeWidgetItem* parentItem = currentItem->parent();

        if (parentItem != NULL)
        {
            QString keyName = parentItem->text(0)+ "_" + stepName;

            if (_stepList.contains(keyName))
            {
                CT_VirtualAbstractStep *step = _stepList.value(keyName);
                GAboutStepDialog dialog(step);
                dialog.exec();
            }
        }
    }
}
