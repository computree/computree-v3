#ifndef PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H
#define PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H

#include <QDialog>

namespace Ui {
class PB_ActionManualInventoryAttributesDialog;
}

class PB_ActionManualInventoryAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PB_ActionManualInventoryAttributesDialog(const QStringList &speciesList, const QString &speciesValue, const QString &idValue, QWidget *parent = 0);
    ~PB_ActionManualInventoryAttributesDialog();

    QString getSpecies();
    QString getId();

private:
    Ui::PB_ActionManualInventoryAttributesDialog *ui;
};

#endif // PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H
