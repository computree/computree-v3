#include "inresultwidget.h"
#include "ui_inresultwidget.h"

INResultWidget::INResultWidget(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::INResultWidget)
{
    ui->setupUi(this);
    ui->rb_standardresult->setChecked(true);
}

INResultWidget::~INResultWidget()
{
    delete ui;
}
