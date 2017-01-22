#ifndef GCITATIONDIALOG_H
#define GCITATIONDIALOG_H

#include <QDialog>

namespace Ui {
class GCitationDialog;
}

class GCitationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GCitationDialog(QString pluginsAndStepsText, QString bibTexText, QWidget *parent = 0);
    ~GCitationDialog();

private:
    Ui::GCitationDialog *ui;
    };

#endif // GCITATIONDIALOG_H
