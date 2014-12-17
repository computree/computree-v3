#ifndef PB_ACTIONVALIDATEINVENTORYOPTIONS_H
#define PB_ACTIONVALIDATEINVENTORYOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionValidateInventory;

namespace Ui {
class PB_ActionValidateInventoryOptions;
}

class PB_ActionValidateInventoryOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    enum Mode {
        Mode_Item,
        Mode_attributes
    };

    explicit PB_ActionValidateInventoryOptions(const PB_ActionValidateInventory *action);
    ~PB_ActionValidateInventoryOptions();

    bool shouldAutoCenterCamera();
    bool shouldShowData();
    bool shouldShowCandidateItems();

    PB_ActionValidateInventoryOptions::Mode getMode();
    void toggleMode();

private:
    Ui::PB_ActionValidateInventoryOptions *ui;

signals:
    void parametersChanged();
    void pointsSizeChanged(int);

private slots:

    void on_rb_dbh_toggled(bool checked);
    void on_cb_showData_toggled(bool checked);
    void on_cb_showCandidateItems_toggled(bool checked);
};

#endif // PB_ActionValidateInventoryOptions_H
