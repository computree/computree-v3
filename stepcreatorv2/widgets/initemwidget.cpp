#include "initemwidget.h"
#include "ui_initemwidget.h"

INItemWidget::INItemWidget(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::INItemWidget)
{
    ui->setupUi(this);

    ui->cb_finderMode->addItem("Obligatory");
    ui->cb_finderMode->addItem("Optional");
    ui->cb_finderMode->setCurrentIndex(0);

    ui->cb_choiceMode->addItem("ChooseOneIfMultiple");
    ui->cb_choiceMode->addItem("ChooseMultipleIfMultiple");
    ui->cb_choiceMode->setCurrentIndex(0);
}

INItemWidget::~INItemWidget()
{
    delete ui;
}
