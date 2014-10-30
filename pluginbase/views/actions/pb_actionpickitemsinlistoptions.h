#ifndef PB_ACTIONPICKITEMSINLISTOPTIONS_H
#define PB_ACTIONPICKITEMSINLISTOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionPickItemsInList;

namespace Ui {
class PB_ActionPickItemsInListOptions;
}

class PB_ActionPickItemsInListOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionPickItemsInListOptions(const PB_ActionPickItemsInList *action);
    ~PB_ActionPickItemsInListOptions();

    QColor getColor();
    bool isInAddMode();
    float getMaxDist();

private:
    Ui::PB_ActionPickItemsInListOptions *ui;

signals:
    void parametersChanged();

private slots:
    void on_pb_color_clicked();

    void on_dsb_maxDist_valueChanged(double arg1);

private:
    QColor _itemColor;
};

#endif // PB_ACTIONPICKITEMSINLISTOPTIONS_H
