#ifndef PB_ACTIONSLICEPOINTCLOUDOPTIONS_H
#define PB_ACTIONSLICEPOINTCLOUDOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionSlicePointCloud;

namespace Ui {
class PB_ActionSlicePointCloudOptions;
}

class PB_ActionSlicePointCloudOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionSlicePointCloudOptions(const PB_ActionSlicePointCloud *action);
    ~PB_ActionSlicePointCloudOptions();

    bool shouldDrawScenes();

private:
    Ui::PB_ActionSlicePointCloudOptions *ui;

signals:
    void parametersChanged();
    void buttonPushed();

private slots:
    void on_pb_exemple_clicked();
    void on_cb_drawScenes_clicked();
};

#endif // PB_ACTIONSLICEPOINTCLOUDOPTIONS_H
