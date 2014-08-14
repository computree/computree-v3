#ifndef GCAMERACOORDINATESOPTIONS_H
#define GCAMERACOORDINATESOPTIONS_H

#include <QWidget>

#include "dm_graphicsviewcamera.h"

namespace Ui {
class GCameraCoordinatesOptions;
}

class GCameraCoordinatesOptions : public QWidget
{
    Q_OBJECT
    
public:
    explicit GCameraCoordinatesOptions(QWidget *parent = 0);
    ~GCameraCoordinatesOptions();
    
    void setCameraController(const DM_GraphicsViewCamera *camController);

protected:
    void showEvent(QShowEvent * ev);

private:
    Ui::GCameraCoordinatesOptions   *ui;

    DM_GraphicsViewCamera           *_camController;

    void connectCamController(DM_GraphicsViewCamera *cc);
    void disconnectCamController(DM_GraphicsViewCamera *cc);

    void updateView(DM_GraphicsViewCamera *cc);

private slots:

    void updateView();

    void on_pushButtonValid_clicked();
};

#endif // GCAMERACOORDINATESOPTIONS_H
