#include "gstepsdockwidget.h"
#include "ui_gstepsdockwidget.h"

#include "dm_guimanager.h"

GStepsDockWidget::GStepsDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::GStepsDockWidget)
{
    ui->setupUi(this);
}

GStepsDockWidget::~GStepsDockWidget()
{
    delete ui;
}

void GStepsDockWidget::init()
{
    ui->stepsView->init(*GUI_MANAGER->getPluginManager());
}
