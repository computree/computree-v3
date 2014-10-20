#ifndef GMULTIPLEITEMDRAWABLEMODELMANAGER_H
#define GMULTIPLEITEMDRAWABLEMODELMANAGER_H

#include <QWidget>

#include "dm_multipleitemdrawablemodelmanager.h"
#include "dm_itemdrawablemanageroptions.h"

class GDocumentManagerView;

namespace Ui {
class GMultipleItemDrawableModelManager;
}

class GMultipleItemDrawableModelManager : public QWidget, public DM_MultipleItemDrawableModelManager
{
    Q_OBJECT
    
public:
    explicit GMultipleItemDrawableModelManager(QWidget *parent = 0);
    ~GMultipleItemDrawableModelManager();

    void setDocumentManagerView(const GDocumentManagerView *docManagerView);
    
    void addResult(const CT_AbstractResult *res);
    void removeResult(const CT_AbstractResult *res);

    CT_AbstractResult* currentResult() const;

    void reconstruct();

private:
    Ui::GMultipleItemDrawableModelManager   *ui;

    QList<CT_AbstractResult*>                          _results;
    DM_ItemDrawableManagerOptions           _options;

    int indexOfResultInComboBox(const CT_AbstractResult *res);
    void setEmptyComboBoxText();

private slots:

    void on_comboBoxResult_currentIndexChanged(int index);
    void on_pushButtonChooseColor_clicked();
    void resultDestroyedDirect(QObject *o);
    void resultDestroyedQueued(QObject *o);
};

#endif // GMULTIPLEITEMDRAWABLEMODELMANAGER_H
