#ifndef PB_ACTIONVALIDATEINVENTORYATTRIBUTESDIALOG_H
#define PB_ACTIONVALIDATEINVENTORYATTRIBUTESDIALOG_H

#include <QDialog>

namespace Ui {
class PB_ActionValidateInventoryAttributesDialog;
}

class PB_ActionValidateInventoryAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PB_ActionValidateInventoryAttributesDialog(const QStringList &speciesList, const QString &speciesValue, const QString &idValue, QWidget *parent = 0);
    ~PB_ActionValidateInventoryAttributesDialog();

    QString getSpecies();
    QString getId();

private:
    Ui::PB_ActionValidateInventoryAttributesDialog *ui;
};

#endif // PB_ACTIONVALIDATEINVENTORYATTRIBUTESDIALOG_H
