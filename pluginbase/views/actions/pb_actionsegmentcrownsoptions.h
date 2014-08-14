#ifndef PB_ACTIONSEGMENTCROWNSOPTIONS_H
#define PB_ACTIONSEGMENTCROWNSOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionSegmentCrowns;

namespace Ui {
class PB_ActionSegmentCrownsOptions;
}

class PB_ActionSegmentCrownsOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    enum Mode {
        CHOOSECLUSTER,
        DRAWLIMITS,
        FILLAREA,
        FREEMOVE,
        CHANGECENTERCELL
    };

    enum DrawingGrid {
        DENSITY,
        HEIGHT,
        CLUSTERS
    };

    explicit PB_ActionSegmentCrownsOptions(const PB_ActionSegmentCrowns *action, float initZvalue);
    ~PB_ActionSegmentCrownsOptions();

    float getHeight();
    int getActiveCluster();
    bool getShowClustersOnly();
    double getHSingleStep();
    int getPencilSize();
    bool redForSelection();
    bool isFillModeFull();

    Mode getMode();
    DrawingGrid getDrawingGrid();

private:
    Ui::PB_ActionSegmentCrownsOptions *ui;

    Mode                _mode;
    DrawingGrid         _drawingGrid;

public slots:
    void setHeight(float h);
    void setActiveCluster(int cluster);
    void setClusterNumber(int nb);
    void setMode(Mode mode);
    void setDrawingGrid(DrawingGrid drawingGrid);
    void increaseHValue();
    void decreaseHValue();
    void increasePencilSizeValue();
    void decreasePencilSizeValue();
    void setActiveClusterColor(const QColor* color);
    void setUndoRedo(bool undo, bool redo);

signals:
    void parametersChanged();
    void activeClusterChanged();
    void askForClusterCreation();
    void undo();
    void redo();
    void askForMerging(int pixelNumber);

private slots:

    void on_pb_pickup_clicked();
    void on_pb_addCluster_clicked();
    void on_pb_drawLimit_clicked();
    void on_pb_fill_clicked();
    void on_pb_freemove_clicked();
    void on_pb_centerOnCell_clicked();
    void on_rb_clusters_clicked();
    void on_rb_mns_clicked();
    void on_rb_density_clicked();
    void on_sb_activeCluster_valueChanged(int arg1);
    void on_dsb_height_valueChanged(double arg1);
    void on_pb_showClusters_clicked();
    void on_sb_pencilSize_valueChanged(int arg1);
    void on_pb_activeClusterColor_toggled(bool checked);

    void changeSingleStep(int button);
    void on_pb_merge_clicked();
};

#endif // PB_ACTIONSEGMENTCROWNSOPTIONS_H
