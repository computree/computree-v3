#include "gcitationdialog.h"
#include "ui_gcitationdialog.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>

GCitationDialog::GCitationDialog(QList<CDM_CitationInfo::StepCitationInfo> stepInfos,
                                 QString pluginsAndStepsText,
                                 QString risText,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GCitationDialog)
{
    ui->setupUi(this);

    _stepInfos = stepInfos;
    _risText = risText;

    for (int i = 0 ; i < stepInfos.size() ; i++)
    {
        const CDM_CitationInfo::StepCitationInfo& info = stepInfos.at(i);

        ui->tw_script->insertRow(i);
        ui->tw_script->setItem(i,0, new QTableWidgetItem(QString("%1").arg(info._num)));
        ui->tw_script->setItem(i,1, new QTableWidgetItem(info._pluginName));
        ui->tw_script->setItem(i,2, new QTableWidgetItem(info._stepName));
        ui->tw_script->setItem(i,3, new QTableWidgetItem(info._stepDescription));
    }
    ui->tw_script->resizeColumnsToContents();

    ui->te_citations->setText(pluginsAndStepsText);

    ui->te_RIS->setText(risText);
}

GCitationDialog::~GCitationDialog()
{
    delete ui;
}

void GCitationDialog::on_pb_exportScriptSummary_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Choose export file"), "", tr("Text file (*.txt)"));

    QFile exportFile(filename);
    if (exportFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&exportFile);

        stream << tr("Num\t") << tr("Plugin\t") << tr("Step Name\t") << tr("Step Description\n");

        for (int i = 0 ; i < _stepInfos.size() ; i++)
        {
            const CDM_CitationInfo::StepCitationInfo& info = _stepInfos.at(i);

            stream << QString("%1").arg(info._num) << "\t";
            stream << info._pluginName << "\t";
            stream << info._stepName << "\t";
            stream << info._stepDescription << "\n";
        }

        exportFile.close();
    }
}

void GCitationDialog::on_pb_RISFile_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Choose export file"), "", tr("RIS file (*.ris)"));

    QFile exportFile(filename);
    if (exportFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&exportFile);
        stream << _risText;
        exportFile.close();
    }

}
