#ifndef PB_ACTIONSELECTCELLSINGRID3DCOLONIZEDIALOG_H
#define PB_ACTIONSELECTCELLSINGRID3DCOLONIZEDIALOG_H

#include <QDialog>

namespace Ui {
class PB_ActionSelectCellsInGrid3DColonizeDialog;
}

class PB_ActionSelectCellsInGrid3DColonizeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PB_ActionSelectCellsInGrid3DColonizeDialog(QWidget *parent = 0);
    ~PB_ActionSelectCellsInGrid3DColonizeDialog();

    void setXp(bool val);
    void setXm(bool val);
    void setYp(bool val);
    void setYm(bool val);
    void setZp(bool val);
    void setZm(bool val);

    bool getXp();
    bool getXm();
    bool getYp();
    bool getYm();
    bool getZp();
    bool getZm();

private:
    Ui::PB_ActionSelectCellsInGrid3DColonizeDialog *ui;

};

#endif // PB_ACTIONSELECTCELLSINGRID3DCOLONIZEDIALOG_H
