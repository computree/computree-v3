#include "pbg_csvconfigurationdialog.h"
#include "ui_pbg_csvconfigurationdialog.h"

PBG_CSVConfigurationDialog::PBG_CSVConfigurationDialog(PB_CSVExporterConfiguration &configuration,
                                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PBG_CSVConfigurationDialog)
{
    ui->setupUi(this);

    ui->widgetPreview->setConfiguration(&configuration);

    connect(ui->widgetDataRefList, SIGNAL(attributeChoosed(QString)), ui->widgetPreview, SLOT(setItemAttribute(QString)));
}

PBG_CSVConfigurationDialog::~PBG_CSVConfigurationDialog()
{
    delete ui;
}

void PBG_CSVConfigurationDialog::setListOfDataRefList(const QList<const IItemDataRefList*> *list)
{
    ui->widgetDataRefList->setListOfDataRefList(list);
    ui->widgetPreview->setListOfDataRefList(list);
}

void PBG_CSVConfigurationDialog::accept()
{
    ui->widgetPreview->updateConfiguration();

    QDialog::accept();
}
