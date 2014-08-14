#include "pb_actionmeasurecrownattributesoptions.h"
#include "ui_pb_actionmeasurecrownattributesoptions.h"

#include "ct_global/ct_context.h"

#include "actions/pb_actionmeasurecrownattributes.h"

PB_ActionMeasureCrownAttributesOptions::PB_ActionMeasureCrownAttributesOptions(const PB_ActionMeasureCrownAttributes *action, float zmin, float zmax) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionMeasureCrownAttributesOptions)
{
    ui->setupUi(this);

    _zmax = zmax;
    _zmin = zmin;

    ui->cb_variables->addItem("Htot");
    ui->cb_variables->addItem("Hbase");

    ui->rb_m->setChecked(true);

    _offset = 0.32;

    ui->dsb_offset->setValue(_offset);
    ui->dsb_z->setValue(zmax);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    ui->cb_variables->setCurrentIndex(ui->cb_variables->findText("Htot"));
#else
    ui->cb_variables->setCurrentText("Htot");
#endif

    // At the begining Htot = Zmax - Zmin + offset
    float htot = _zmax - _zmin + _offset;

    _measures.insert("Htot", htot);
    _measures.insert("Hbase", 0);

    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Zsol= %1").arg(_zmin));
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("offset= %1").arg(_offset));
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Htot= %1").arg(htot));
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Hbase= 0"));

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(changeSingleStep(int)));
    connect(ui->pb_plus, SIGNAL(clicked()), this, SLOT(increaseHValue()));
    connect(ui->pb_minus, SIGNAL(clicked()), this, SLOT(decreaseHValue()));
    connect(ui->pb_sideView, SIGNAL(clicked()), this, SIGNAL(askForSideView()));

    ui->pb_sideView->hide();

    ui->cb_minH->setModel(ui->cb_variables->model());
    ui->cb_maxH->setModel(ui->cb_variables->model());

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    ui->cb_minH->setCurrentIndex(ui->cb_minH->findText("Hbase"));
    ui->cb_maxH->setCurrentIndex(ui->cb_maxH->findText("Htot"));
#else
    ui->cb_minH->setCurrentText("Hbase");
    ui->cb_maxH->setCurrentText("Htot");
#endif
}

PB_ActionMeasureCrownAttributesOptions::~PB_ActionMeasureCrownAttributesOptions()
{
    delete ui;
}

float PB_ActionMeasureCrownAttributesOptions::getCurrentZValue()
{
    return ui->dsb_z->value();
}

float PB_ActionMeasureCrownAttributesOptions::getGroundZValue()
{
    return (_zmin - _offset);
}

void PB_ActionMeasureCrownAttributesOptions::on_pb_measure_clicked()
{
    float h = ui->dsb_z->value() - _zmin  + _offset;
    QString variableName = ui->cb_variables->currentText();

    _measures.insert(variableName, h);

    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Valeur de la variable %1 modifiée !").arg(variableName));
}

void PB_ActionMeasureCrownAttributesOptions::changeSingleStep(int button)
{
    Q_UNUSED(button);
    if (ui->rb_m->isChecked()) {ui->dsb_z->setSingleStep(1);}
    else if (ui->rb_dm->isChecked()) {ui->dsb_z->setSingleStep(0.1);}
    else if (ui->rb_cm->isChecked()) {ui->dsb_z->setSingleStep(0.01);}
    else if (ui->rb_mm->isChecked()) {ui->dsb_z->setSingleStep(0.001);}
}

void PB_ActionMeasureCrownAttributesOptions::increaseHValue()
{
    ui->dsb_z->setValue(ui->dsb_z->value() + ui->dsb_z->singleStep());
    emit redrawNeeded();
}

void PB_ActionMeasureCrownAttributesOptions::decreaseHValue()
{
    ui->dsb_z->setValue(ui->dsb_z->value() - ui->dsb_z->singleStep());
    emit redrawNeeded();
}

void PB_ActionMeasureCrownAttributesOptions::increaseSingleStep()
{
    if (ui->rb_mm->isChecked()) {ui->rb_cm->setChecked(true);}
    else if (ui->rb_cm->isChecked()) {ui->rb_dm->setChecked(true);}
    else if (ui->rb_dm->isChecked()) {ui->rb_m->setChecked(true);}

    changeSingleStep(0);
}

void PB_ActionMeasureCrownAttributesOptions::decreaseSingleStep()
{
    if (ui->rb_m->isChecked()) {ui->rb_dm->setChecked(true);}
    else if (ui->rb_dm->isChecked()) {ui->rb_cm->setChecked(true);}
    else if (ui->rb_cm->isChecked()) {ui->rb_mm->setChecked(true);}

    changeSingleStep(0);
}

void PB_ActionMeasureCrownAttributesOptions::on_pb_min_clicked()
{
    ui->dsb_z->setValue(_zmin);
    emit redrawNeeded();
}

void PB_ActionMeasureCrownAttributesOptions::on_pb_max_clicked()
{
    ui->dsb_z->setValue(_zmax);
    emit redrawNeeded();
}

void PB_ActionMeasureCrownAttributesOptions::on_dsb_offset_valueChanged(double arg1)
{
    float delta = arg1 - _offset;
    _offset = arg1;

    // Update all measured values with the new offset
    QMutableMapIterator<QString, float> it(_measures);
    while (it.hasNext())
    {
        it.next();
        it.setValue(it.value() + delta);
    }
}

void PB_ActionMeasureCrownAttributesOptions::on_cb_variables_currentIndexChanged(const QString &arg1)
{
    if (_measures.contains(arg1))
    {
        float h = _measures.value(arg1);
        ui->dsb_z->setValue(h + _zmin - _offset);
    }
    emit redrawNeeded();
}

void PB_ActionMeasureCrownAttributesOptions::on_pb_print_clicked()
{
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("----- Mesures -----").arg(_zmin));
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("Zsol= %1").arg(_zmin));
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("offset= %1").arg(_offset));

    QMapIterator<QString, float> it(_measures);
    while (it.hasNext())
    {
        it.next();
        PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("%1= %2").arg(it.key()).arg(it.value()));
    }
    PS_LOG->addMessage(LogInterface::info, LogInterface::action, tr("-------------------").arg(_zmin));

}

void PB_ActionMeasureCrownAttributesOptions::on_pb_computeCrownProjection_clicked()
{
    QString minVariable = ui->cb_minH->currentText();
    QString maxVariable = ui->cb_maxH->currentText();

    float minH = _measures.value(minVariable);
    float maxH = _measures.value(maxVariable);

    float zmin = minH + _zmin - _offset;
    float zmax = maxH + _zmin - _offset;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    ui->cb_variables->setCurrentIndex(ui->cb_variables->findText(maxVariable));
#else
    ui->cb_variables->setCurrentText(maxVariable);
#endif
    on_cb_variables_currentIndexChanged(maxVariable);

    askForCrownProjectionComputing(zmin, zmax);
}
