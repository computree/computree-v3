#include "gmultipleitemdrawablemodelmanager.h"
#include "ui_gmultipleitemdrawablemodelmanager.h"

#include "gstepmanager.h"
#include "gitemdrawablemanageroptionscolor.h"

#include <QLineEdit>

#include "ct_result/abstract/ct_abstractresult.h"

GMultipleItemDrawableModelManager::GMultipleItemDrawableModelManager(QWidget *parent) :
    QWidget(parent), DM_MultipleItemDrawableModelManager(),
    ui(new Ui::GMultipleItemDrawableModelManager)
{
    ui->setupUi(this);

    _options.load();

    ui->widgetModelManager->setColorOptions(_options);

    connect(ui->gradientManagerView, SIGNAL(newGradientSelected(QLinearGradient)), ui->widgetModelManager->contextMenuColouristAdder(), SLOT(setLinearGradientToUseForColorization(QLinearGradient)));
    ui->widgetModelManager->contextMenuColouristAdder()->setGradientToUseForColorization(ui->gradientManagerView->gradientSelected());

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

void GMultipleItemDrawableModelManager::addResult(const CT_AbstractResult *res)
{
    int indexOf = indexOfResultInComboBox(res);

    if(indexOf == -1)
    {
        if(ui->comboBoxResult->itemData(0).value<void*>() == NULL)
        {
            ui->comboBoxResult->clear();
            ui->comboBoxResult->setEditable(false);
        }

        connect((CT_AbstractResult*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedDirect(QObject*)), Qt::DirectConnection);
        connect((CT_AbstractResult*)res, SIGNAL(destroyed(QObject*)), this, SLOT(resultDestroyedQueued(QObject*)), Qt::QueuedConnection);

        _results.append((CT_AbstractResult*)res);
        ui->comboBoxResult->addItem(res->getName() + " (" + GStepManager::staticGetStepName(*res->parentStep()) + ")", qVariantFromValue((void*)res));
        ui->comboBoxResult->setCurrentIndex(ui->comboBoxResult->count()-1);
    }
    else
    {
        ui->comboBoxResult->setCurrentIndex(indexOf);
    }
}

void GMultipleItemDrawableModelManager::removeResult(const CT_AbstractResult *res)
{
    int index = indexOfResultInComboBox(res);

    if(index != -1)
    {
        disconnect((CT_AbstractResult*)res, NULL, this, NULL);

        _results.removeOne((CT_AbstractResult*)res);
        ui->comboBoxResult->removeItem(index);
    }

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();
}

CT_AbstractResult* GMultipleItemDrawableModelManager::currentResult() const
{
    return ui->widgetModelManager->result();
}

void GMultipleItemDrawableModelManager::reconstruct()
{
    ui->widgetModelManager->reconstruct();
}

int GMultipleItemDrawableModelManager::indexOfResultInComboBox(const CT_AbstractResult *res)
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
    int indexOf = _results.indexOf((CT_AbstractResult*)ui->comboBoxResult->itemData(index).value<void*>());

    if((index >= 0)
            && (index < ui->comboBoxResult->count())
            && (indexOf != -1))
        ui->widgetModelManager->setResult((CT_AbstractResult*)_results.at(indexOf));
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
    _results.removeOne((CT_AbstractResult*)o);
}

void GMultipleItemDrawableModelManager::resultDestroyedQueued(QObject *o)
{
    int index = indexOfResultInComboBox((CT_AbstractResult*)o);

    if(index != -1)
        ui->comboBoxResult->removeItem(index);

    if(ui->comboBoxResult->count() == 0)
        setEmptyComboBoxText();
}
