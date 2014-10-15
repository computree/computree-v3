#ifndef CT_GDEFAULTCOORDINATESYSTEM_H
#define CT_GDEFAULTCOORDINATESYSTEM_H

#include <QDialog>

#include "ct_coordinates/ct_defaultcoordinatesystem.h"

namespace Ui {
class CT_GDefaultCoordinateSystem;
}

class CT_GDefaultCoordinateSystem : public QDialog
{
    Q_OBJECT

public:
    explicit CT_GDefaultCoordinateSystem(QWidget *parent = 0);
    ~CT_GDefaultCoordinateSystem();

    /**
     * @brief Set the coordinate system to modify
     */
    void setCoordinateSystem(const CT_DefaultCoordinateSystem *cs);

    /**
     * @brief Set read only
     */
    void setReadOnly(bool val);

private:
    Ui::CT_GDefaultCoordinateSystem *ui;
    CT_DefaultCoordinateSystem      *m_cs;

public slots:
    void accept();
};

#endif // CT_GDEFAULTCOORDINATESYSTEM_H
