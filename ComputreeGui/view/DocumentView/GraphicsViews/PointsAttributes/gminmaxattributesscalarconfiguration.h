#ifndef GMINMAXATTRIBUTESSCALARCONFIGURATION_H
#define GMINMAXATTRIBUTESSCALARCONFIGURATION_H

#include <QDialog>

#include "tools/attributes/worker/abstract/dm_abstractattributesscalar.h"

namespace Ui {
class GMinMaxAttributesScalarConfiguration;
}

class GMinMaxAttributesScalarConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit GMinMaxAttributesScalarConfiguration(QWidget *parent = 0);
    ~GMinMaxAttributesScalarConfiguration();

    void setAttributes(const DM_AbstractAttributesScalar *pa);

private:
    Ui::GMinMaxAttributesScalarConfiguration    *ui;
    DM_AbstractAttributesScalar                 *m_pa;

public slots:
    void accept();

private slots:
    void on_doubleSpinBoxMin_valueChanged(double d);
    void on_doubleSpinBoxMax_valueChanged(double d);
};

#endif // GMINMAXATTRIBUTESSCALARCONFIGURATION_H
