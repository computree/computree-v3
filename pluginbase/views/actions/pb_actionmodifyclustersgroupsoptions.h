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

//    PB_ActionModifyClustersGroups::SelectionDrawMode drawMode() const;

private:
    Ui::PB_ActionModifyClustersGroupsOptions *ui;

private slots:
    void on_buttonGroupSelection_buttonReleased(int id);
    void on_buttonGroupType_buttonReleased(int id);
    void on_buttonGroupMode_buttonReleased(int id);

public slots:

    void setSelectionMode(GraphicsViewInterface::SelectionMode mode);

signals:

    void selectionModeChanged(GraphicsViewInterface::SelectionMode mode);
};

#endif // PB_ACTIONMODIFYCLUSTERSGROUPSOPTIONS_H
