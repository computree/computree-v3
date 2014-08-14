#ifndef PB_ACTIONDEFINEHEIGHTLAYEROPTIONS_H
#define PB_ACTIONDEFINEHEIGHTLAYEROPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionDefineHeightLayer;

namespace Ui {
class PB_ActionDefineHeightLayerOptions;
}

class PB_ActionDefineHeightLayerOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionDefineHeightLayerOptions(const PB_ActionDefineHeightLayer *action, float zmin, float zmax, float res, int threshold);
    ~PB_ActionDefineHeightLayerOptions();

    float getZmin();
    float getZmax();    

    void chooseZmin();
    void chooseZmax();
    int getThreshold();
    float getResolution();

public slots:
    void increaseHValue();
    void decreaseHValue();
    void increaseSingleStep();
    void decreaseSingleStep();

private slots:

    void changeSingleStep(int button);

    void on_dsb_zmin_valueChanged(double arg1);
    void on_pb_computeCrownProjection_clicked();

signals:
    void redrawNeeded();
    void zValuesChanged(float zmin, float zmax);
    void askForSideView();
    void askForCrownProjectionComputing();
    void updateResolution(double);
    void updateThreshold(int);

private:
    Ui::PB_ActionDefineHeightLayerOptions *ui;
};

#endif // PB_ACTIONDEFINEHEIGHTLAYEROPTIONS_H
