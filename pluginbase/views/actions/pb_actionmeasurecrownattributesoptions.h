#ifndef PB_ACTIONMEASURECROWNATTRIBUTESOPTIONS_H
#define PB_ACTIONMEASURECROWNATTRIBUTESOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionMeasureCrownAttributes;

namespace Ui {
class PB_ActionMeasureCrownAttributesOptions;
}

class PB_ActionMeasureCrownAttributesOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionMeasureCrownAttributesOptions(const PB_ActionMeasureCrownAttributes *action, float zmin, float zmax);
    ~PB_ActionMeasureCrownAttributesOptions();

    inline QMap<QString, float> getMeasures() {return _measures;}
    float getCurrentZValue();
    float getGroundZValue();

public slots:
    void increaseHValue();
    void decreaseHValue();
    void increaseSingleStep();
    void decreaseSingleStep();

private slots:

    void changeSingleStep(int button);

    void on_pb_measure_clicked();

    void on_pb_min_clicked();

    void on_pb_max_clicked();

    void on_dsb_offset_valueChanged(double arg1);

    void on_cb_variables_currentIndexChanged(const QString &arg1);

    void on_pb_print_clicked();

    void on_pb_computeCrownProjection_clicked();

signals:
    void redrawNeeded();
    void askForSideView();
    void askForCrownProjectionComputing(float zmin, float zmax);

private:
    Ui::PB_ActionMeasureCrownAttributesOptions *ui;

    QMap<QString, float>        _measures;
    float                       _zmin;
    float                       _zmax;
    float                       _offset;

};

#endif // PB_ACTIONMEASURECROWNATTRIBUTESOPTIONS_H
