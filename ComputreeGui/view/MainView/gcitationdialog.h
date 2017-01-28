#ifndef GCITATIONDIALOG_H
#define GCITATIONDIALOG_H

#include "src/cdm_citationinfo.h"
#include <QDialog>

namespace Ui {
class GCitationDialog;
}

class GCitationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GCitationDialog(QList<CDM_CitationInfo::StepCitationInfo> stepInfos,
                             QString pluginsAndStepsText,
                             QString risText,
                             QWidget *parent = 0);
    ~GCitationDialog();

private slots:
    void on_pb_exportScriptSummary_clicked();

    void on_pb_RISFile_clicked();

private:
    Ui::GCitationDialog *ui;

    QString _risText;
    QList<CDM_CitationInfo::StepCitationInfo> _stepInfos;

    };

#endif // GCITATIONDIALOG_H
