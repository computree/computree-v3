#ifndef PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS02_H
#define PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS02_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"
#include "actions/pb_actionmodifyclustersgroups02.h"

namespace Ui {
class PB_ActionModifyClustersGroupsOptions02;
}

class PB_ActionModifyClustersGroupsOptions02 : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionModifyClustersGroupsOptions02(const PB_ActionModifyClustersGroups02 *action);
    ~PB_ActionModifyClustersGroupsOptions02();

    GraphicsViewInterface::SelectionMode selectionMode() const;

    bool isAVisible() const;
    bool isBVisible() const;
    bool isTMPVisible() const;
    bool isTrashVisible() const;
    bool isOthersVisible() const;
    bool isValidatedVisible() const;

    void selectColorA(QColor color);
    void selectColorB(QColor color);
    void setMultiSelect(bool multi);

    void toggleOthersVisible();
protected:
    QColor  _colorA;
    QColor  _colorB;

private:
    Ui::PB_ActionModifyClustersGroupsOptions02 *ui;

private slots:
    void on_buttonGroupSelection_buttonReleased(int id);
    void on_buttonGroupMode_buttonReleased(int id);

    void on_pb_SetSceneA_clicked();
    void on_pb_SetSceneB_clicked();

    void on_pb_ColorA_clicked();
    void on_pb_ColorB_clicked();

    void on_pb_toA_clicked();
    void on_pb_toB_clicked();
    void on_pb_toTmp_clicked();
    void on_pb_toTrash_clicked();

    void on_cb_Avisible_toggled(bool checked);
    void on_cb_Bvisible_toggled(bool checked);
    void on_cb_othersVisible_toggled(bool checked);
    void on_cb_tmpVisible_toggled(bool checked);
    void on_cb_trashVisible_toggled(bool checked);

    void on_pb_extend_clicked();

    void on_pb_validate_clicked();

    void on_cb_showValidated_toggled(bool checked);

public slots:

    void setSelectionMode(GraphicsViewInterface::SelectionMode mode);

signals:

    void selectionModeChanged(GraphicsViewInterface::SelectionMode mode);
    void selectPositionA();
    void selectPositionB();
    void setColorA(QColor color);
    void setColorB(QColor color);

    void affectClusterToA();
    void affectClusterToB();
    void affectClusterToTMP();
    void affectClusterToTrash();

    void extend();
    void validatePosition();

    void visibilityChanged();

    void enterLimitMode();
    void distanceChanged(int val);

};

#endif // PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS02_H
