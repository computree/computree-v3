#ifndef PB_ACTIONMODIFYPOSITIONS2DOPTIONS_H
#define PB_ACTIONMODIFYPOSITIONS2DOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionModifyPositions2D;

namespace Ui {
class PB_ActionModifyPositions2DOptions;
}

class PB_ActionModifyPositions2DOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionModifyPositions2DOptions(const PB_ActionModifyPositions2D *action);
    ~PB_ActionModifyPositions2DOptions();

    bool isFreeMoveSelected();
    bool isMovePositionSelected();
    bool isAddPositionSelected();
    bool isRemovePositionSelected();

    double getZValue();
    bool isDrawPlaneSelected();

private:
    Ui::PB_ActionModifyPositions2DOptions *ui;

signals:
    void parametersChanged();

private slots:
    void on_pb_freeMove_clicked();
    void on_pb_move_clicked();
    void on_pb_add_clicked();
    void on_pb_remove_clicked();
    void on_dsb_zval_valueChanged(double arg1);
    void singleStepChanged(int button);
    void on_cb_drawPlane_toggled(bool checked);
};

#endif // PB_ACTIONMODIFYPOSITIONS2DOPTIONS_H
