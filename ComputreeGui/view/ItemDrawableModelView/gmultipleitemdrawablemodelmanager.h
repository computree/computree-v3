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
    
    void addResult(const Result *res);
    void removeResult(const Result *res);

    Result* currentResult() const;

private:
    Ui::GMultipleItemDrawableModelManager   *ui;

    QList<Result*>                          _results;
    DM_ItemDrawableManagerOptions           _options;

    int indexOfResultInComboBox(const Result *res);
    void setEmptyComboBoxText();

private slots:

    void on_comboBoxResult_currentIndexChanged(int index);
    void on_pushButtonChooseColor_clicked();
    void resultDestroyedDirect(QObject *o);
    void resultDestroyedQueued(QObject *o);
};

#endif // GMULTIPLEITEMDRAWABLEMODELMANAGER_H
