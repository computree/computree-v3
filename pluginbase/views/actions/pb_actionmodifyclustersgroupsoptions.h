#ifndef PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H
#define PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"
#include "actions/pb_actionmodifyclustersgroups.h"

namespace Ui {
class PB_ActionModifyClustersGroupsOptions;
}

class PB_ActionModifyClustersGroupsOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionModifyClustersGroupsOptions(const PB_ActionModifyClustersGroups *action);
    ~PB_ActionModifyClustersGroupsOptions();

    GraphicsViewInterface::SelectionMode selectionMode() const;

    bool isAVisible() const;
    bool isBVisible() const;
    bool isTMPVisible() const;
    bool isTrashVisible() const;
    bool isOthersVisible() const;

    bool isInSceneMode() const;
    bool isInLimitMode() const;
    bool isInSplitMode() const;

    void selectColorA(QColor color);
    void selectColorB(QColor color);

    void selectSceneMode();
    void selectLimitMode();
    void selectSplitMode();

    void setMultiSelect(bool multi);
    void setDistance(int val);
    void setMaxDistance(int val);
protected:
    QColor  _colorA;
    QColor  _colorB;

private:
    Ui::PB_ActionModifyClustersGroupsOptions *ui;

private slots:
    void on_buttonGroupSelection_buttonReleased(int id);
    void on_buttonGroupType_buttonReleased(int id);
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

    void modeChanged(int button);
    void byChanged(int button);

    void on_sb_LengthOnAxis_valueChanged(int arg1);

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

    void visibilityChanged();

    void enterLimitMode();
    void distanceChanged(int val);

};

#endif // PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H
