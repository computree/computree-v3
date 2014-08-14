#include "gcameracoordinatesoptions.h"
#include "ui_gcameracoordinatesoptions.h"

GCameraCoordinatesOptions::GCameraCoordinatesOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCameraCoordinatesOptions)
{
    ui->setupUi(this);

    _camController = NULL;
}

GCameraCoordinatesOptions::~GCameraCoordinatesOptions()
{
    delete ui;
}

void GCameraCoordinatesOptions::setCameraController(const DM_GraphicsViewCamera *camController)
{
    disconnectCamController(_camController);

    _camController = (DM_GraphicsViewCamera*)camController;

    updateView(_camController);

    connectCamController(_camController);
}

void GCameraCoordinatesOptions::showEvent(QShowEvent *ev)
{
    updateView();

    QWidget::showEvent(ev);
}

void GCameraCoordinatesOptions::disconnectCamController(DM_GraphicsViewCamera *cc)
{
    if(cc != NULL)
        disconnect(cc, NULL, this, NULL);
}

void GCameraCoordinatesOptions::connectCamController(DM_GraphicsViewCamera *cc)
{
    if(cc != NULL)
    {
        connect(cc, SIGNAL(coordinatesChanged()), this, SLOT(updateView()), Qt::DirectConnection);
    }
}

void GCameraCoordinatesOptions::updateView(DM_GraphicsViewCamera *cc)
{
    if(cc != NULL)
    {
        ui->doubleSpinBoxX->setValue(cc->x());
        ui->doubleSpinBoxY->setValue(cc->y());
        ui->doubleSpinBoxZ->setValue(cc->z());
        ui->doubleSpinBoxCX->setValue(cc->cx());
        ui->doubleSpinBoxCY->setValue(cc->cy());
        ui->doubleSpinBoxCZ->setValue(cc->cz());
        ui->doubleSpinBoxRX->setValue(cc->rx());
        ui->doubleSpinBoxRY->setValue(cc->ry());
        ui->doubleSpinBoxRZ->setValue(cc->rz());
    }
}

void GCameraCoordinatesOptions::updateView()
{
    if(isVisible())
        updateView(_camController);
}

void GCameraCoordinatesOptions::on_pushButtonValid_clicked()
{
    if(_camController != NULL)
    {
        _camController->setPosition(ui->doubleSpinBoxX->value(),
                                    ui->doubleSpinBoxY->value(),
                                    ui->doubleSpinBoxZ->value(),
                                    false);

        _camController->setSceneCenter(ui->doubleSpinBoxCX->value(),
                                       ui->doubleSpinBoxCY->value(),
                                       ui->doubleSpinBoxCZ->value(),
                                       false);

        _camController->setViewDirection(ui->doubleSpinBoxRX->value(),
                                         ui->doubleSpinBoxRY->value(),
                                         ui->doubleSpinBoxRZ->value(),
                                         true);
    }
}
