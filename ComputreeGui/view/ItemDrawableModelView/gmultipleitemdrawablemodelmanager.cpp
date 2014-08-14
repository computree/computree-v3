#include "gmultipleitemdrawablemodelmanager.h"
#include "ui_gmultipleitemdrawablemodelmanager.h"

#include "gstepmanager.h"
#include "gitemdrawablemanageroptionscolor.h"

#include <QLineEdit>

GMultipleItemDrawableModelManager::GMultipleItemDrawableModelManager(QWidget *parent) :
    QWidget(parent), DM_MultipleItemDrawableModelManager(),
    ui(new Ui::GMultipleItemDrawableModelManager)
{
    ui->setupUi(this);

    _options.load();

    ui->widgetModelManager->setColorOptions(_options);

    setEmptyComboBoxText();
}

GMultipleItemDrawableModelManager::~GMultipleItemDrawableModelManager()
{
    delete ui;
}

void GMultipleItemDrawableModelManager::setDocumentManagerView(const GDocumentManagerView *docManagerView)
{
    ui->widgetModelManager->setDocumentManagerView(docManagerView);
}

void GMultipleItemDrawableModelManager::addResult(const Result *res)
{
    int indexOf = indexOfResultInComboBox(res);

    if(indexOf == -1)
    {
        if(ui->comboBoxResult->itemData(0).value<void*>() == NULL)
        {
            ui->comboBoxResult->clear();
            ui->comboBoxResult->setEditable(false);
        }

        connect((Result*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedDirect(QObject*)), Qt::DirectConnection);
        connect((Result*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedQueued(QObject*)), Qt::QueuedConnection);

        _results.append((Result*)res);
        ui->comboBoxResult->addItem(res->getName() + " (" + GStepManager::staticGetStepName(*res->parentStep()) + ")", qVariantFromValue((void*)res));
        ui->comboBoxResult->setCurrentIndex(ui->comboBoxResult->count()-1);
    }
    else
    {
        ui->comboBoxResult->setCurrentIndex(indexOf);
    }
}

void GMultipleItemDrawableModelManager::removeResult(const Result *res)
{
    int index = indexOfResultInComboBox(res);

    if(index != -1)
    {
        disconnect((Result*)res, NULL, this, NULL);

        _results.removeOne((Result*)res);
        ui->comboBoxResult->removeItem(index);
    }

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();
}

Result* GMultipleItemDrawableModelManager::currentResult() const
{
    return ui->widgetModelManager->result();
}

int GMultipleItemDrawableModelManager::indexOfResultInComboBox(const Result *res)
{
    int count = ui->comboBoxResult->count();

    for(int i=0; i<count; ++i)
    {
        if(ui->comboBoxResult->itemData(i).value<void*>() == res)
            return i;
    }

    return -1;
}

void GMultipleItemDrawableModelManager::setEmptyComboBoxText()
{
    ui->comboBoxResult->clear();
    ui->comboBoxResult->addItem(tr("........... Aucun element configurable ..........."), qVariantFromValue(0));
    ui->comboBoxResult->setEditable(true);
    ui->comboBoxResult->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->comboBoxResult->lineEdit()->setReadOnly(true);
}

void GMultipleItemDrawableModelManager::on_comboBoxResult_currentIndexChanged(int index)
{
    ui->widgetModelManager->setResult(NULL);
    int indexOf = _results.indexOf((Result*)ui->comboBoxResult->itemData(index).value<void*>());

    if((index >= 0)
            && (index < ui->comboBoxResult->count())
            && (indexOf != -1))
        ui->widgetModelManager->setResult((Result*)_results.at(indexOf));
}

void GMultipleItemDrawableModelManager::on_pushButtonChooseColor_clicked()
{
    GItemDrawableManagerOptionsColor dialog(this);

    dialog.setOptions(_options);

    if(dialog.exec())
    {
        _options.setColorList(dialog.getColorList());

        ui->widgetModelManager->setColorOptions(_options);
    }
}

void GMultipleItemDrawableModelManager::resultDestroyedDirect(QObject *o)
{
    _results.removeOne((Result*)o);
}

void GMultipleItemDrawableModelManager::resultDestroyedQueued(QObject *o)
{
    int index = indexOfResultInComboBox((Result*)o);

    if(index != -1)
        ui->comboBoxResult->removeItem(index);

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();
}
