#ifndef PB_ACTIONMANUALINVENTORYOPTIONS_H
#define PB_ACTIONMANUALINVENTORYOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionManualInventory;

namespace Ui {
class PB_ActionManualInventoryOptions;
}

class PB_ActionManualInventoryOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    enum Mode {
        Mode_dbh,
        Mode_attributes
    };

    explicit PB_ActionManualInventoryOptions(const PB_ActionManualInventory *action);
    ~PB_ActionManualInventoryOptions();

    bool shouldAutoCenterCamera();
    bool shouldShowScenes();
    bool shouldShowData();

    PB_ActionManualInventoryOptions::Mode getMode();
    void toggleMode();

private:
    Ui::PB_ActionManualInventoryOptions *ui;

signals:
    void parametersChanged();

private slots:

    void on_rb_dbh_toggled(bool checked);
    void on_cb_showScenes_toggled(bool checked);
    void on_cb_showData_toggled(bool checked);
};

#endif // PB_ACTIONMANUALINVENTORYOPTIONS_H
